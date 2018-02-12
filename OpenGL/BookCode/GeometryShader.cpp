#include <iostream>
#include <string>
// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Include GLEW.  Always include it bofore gl.h, and glfw.h ... its a little magic
#include <glew\include\GL\glew.h>

// include GLFW
#include <glfw\include\GLFW\glfw3.h>

// Include GLM.
// We don't need it yet, but it is a library for 3D maths
#include <glm\glm\glm.hpp>


// Create a class for or application
class my_application {
private:
	GLuint rendering_program;
	GLuint vao;

public:
	GLuint vertex_shader;
    GLuint tess_control_shader;
    GLuint tess_eval_shader;
    GLuint geom_shader;
	GLuint fragment_shader;
	GLuint program;

	void startup() {
		rendering_program = compile_shaders();
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void shutdown() {
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(rendering_program);
		glDeleteVertexArrays(1, &vao);
	}

	// Our rendering function
	//Includes fancier versions!
	void render(double currentTime) {
		// Simply clear the window with red
		//const GLfloat color[] = { 1.0f, 0.0f, 0.0f, 0.0f };

		//Fancier one from the book which color cycles: violet to dark blue
		//changing the divisor of currentTime changes speed of color cycle
        float winR = (float)sin(currentTime / 1100) * 0.25f + 0.3f;
        float winG = 0.0f;
        float winB = (float)sin(currentTime / 1100) * 0.05f + 0.55f;
		const GLfloat Wincolor[] = { winR, winG, winB, 1.0f };

		glClearBufferfv(GL_COLOR, 0, Wincolor);

		// Use the program object we created earlier for rendering
		glUseProgram(rendering_program);

        //Makes the triangle move around the window!
        GLfloat tripos[] = { (float)(sin(currentTime / 1100) * 0.75),
                             (float)(cos(currentTime / 1100) * 0.6),
                             0.0, 1.0f };


        //color cycling triangle: gold to orange
        float triR = 1.0f;
        float triG = (float)cos(currentTime / 1500) * 0.2f + 0.6f;
        float triB = 0.0f;
        GLfloat tricolor[] = { triR, triG, triB, 1.0f };

        glVertexAttrib4fv(0, tripos);
        //glVertexAttrib4fv(1, tricolor);

        //Draw stuff (for triangles)!
		glDrawArrays(GL_PATCHES, 0, 3);


        //Draw stuff for (tessellations)!
        // 3 points per patch
        //glPatchParameteri(GL_PATCH_VERTICES, 3);
        //glDrawElements(GL_PATCHES, 3, GL_UNSIGNED_INT, );
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        //Draw a BIG point! (needed to draw a point)
		glPointSize(7.0f);
	}


	// Shader compiler
	GLuint compile_shaders(void) {
		// Source code for vertex shader
        // Can play with triangle position using offset variable (declared as in)!
        // Also adjusting triangle color; uses interface block for output
		static const GLchar * vertex_shader_source[] = {
			"#version 450 core                                                          \n"
			"                                                                           \n"
            "layout (location = 0) in vec4 offset;                                      \n"
            //"layout (location = 1) in vec4 tricolor;                                    \n"
            //"out T_OUT                                                                  \n"
            //"{                                                                          \n"
            //"   vec4 tcolor;                                                            \n"
            //"}  t_out;                                                                  \n"
            "                                                                           \n"
			"void main(void)														    \n"
			"{                                                                          \n"
            "   const vec4 vertices[3] = vec4[3](vec4(  0.75,  -0.75,  0.5,  1.0),      \n"
            "                                    vec4( -0.75,  -0.75,  0.5,  1.0),      \n"
            "                                    vec4(  0.75,   0.75,  0.5,  1.0));     \n"
            "   gl_Position = vertices[gl_VertexID] + offset;                           \n"
            //"   t_out.tcolor = tricolor;                                                \n"
			"}                                                                          \n"
		};


        // Source code for tesselation control shader
        // 5 control points per patch (as above)
        static const GLchar * tesselation_control_shader_source[] = {
            "#version 450 core                                  \n"
            "                                                   \n"
            "layout (vertices = 3) out;                         \n"
            "                                                   \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "   if (gl_InvocationID == 0) {                     \n"
            "       gl_TessLevelInner[0] = 7.0;                 \n"
            "       gl_TessLevelOuter[0] = 7.0;                 \n"
            "       gl_TessLevelOuter[1] = 7.0;                 \n"
            "       gl_TessLevelOuter[2] = 7.0;                 \n"
            "   }                                               \n"
            "   gl_out[gl_InvocationID].gl_Position =           \n"
            "       gl_in[gl_InvocationID].gl_Position;         \n"
            "}                                                  \n"
        };


        // Source code for tesselation evaluation
        static const GLchar * tesselation_eval_shader_source[] = {
            "#version 450 core                                  \n"
            "                                                   \n"
            "layout (triangles, equal_spacing, cw) in;          \n"
            "                                                   \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "   gl_Position =                                   \n"
            "       (gl_TessCoord.x * gl_in[0].gl_Position +    \n"
            "        gl_TessCoord.y * gl_in[1].gl_Position +    \n"
            "        gl_TessCoord.z * gl_in[2].gl_Position);    \n"
            "}                                                  \n"
        };


        // Source code for geometry shader
        // Will turn triangles into points
        static const GLchar * geometry_shader_source[] = {
            "#version 450 core                                  \n"
            "                                                   \n"
            "layout (triangles) in;                             \n"
            "layout (points, max_vertices = 3) out;             \n"
            "                                                   \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "   int i;                                          \n"
            "                                                   \n"
            "   for (i = 0; i< gl_in.length(); i++) {           \n"
            "       gl_Position = gl_in[i].gl_Position;         \n"
            "       EmitVertex();                               \n"
            "   }                                               \n"
            "}                                                  \n"
        };



		// Source code for fragment shader
        // Uses interface block from above
		static const GLchar * fragment_shader_source[] = {
			"#version 450 core                                  \n"
			"                                                   \n"
            //"in T_OUT                                           \n"
            //"{                                                  \n"
            //"   vec4 tcolor;                                    \n"
            //"}  t_in;                                           \n"
            "                                                   \n"
			"out vec4 color;                                    \n"
			"                                                   \n"
			"void main(void)                                    \n"
			"{                                                  \n"
            "   color = vec4(0.5,0.7,0.1, 1.0);                 \n"
			"}                                                  \n"
		};

		// create and compile vertex shader
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, nullptr);
		glCompileShader(vertex_shader);

        // create and compile tesselation control shader
        tess_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tess_control_shader, 1, tesselation_control_shader_source, nullptr);
        glCompileShader(tess_control_shader);

        // create and compile tesselation evaluation shader
        tess_eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tess_eval_shader, 1, tesselation_eval_shader_source, nullptr);
        glCompileShader(tess_eval_shader);

        // create and compile geometry shader
        geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geom_shader, 1, geometry_shader_source, nullptr);
        glCompileShader(geom_shader);

		// create and compile fragment shader
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, nullptr);
		glCompileShader(fragment_shader);


        //check to make sure that shaders compiled! (and if not why not)
        GLint compiled;
        GLint loglen = 500;
        GLsizei shadelen = 0;

        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            std::cout << "\nUh oh! Vertex shader didn't compile! \n";
            GLchar* error_log = (GLchar*)malloc(loglen);
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &loglen);
            glGetShaderInfoLog(vertex_shader, loglen, &shadelen, error_log);
            std::cout << "compiler_log: \n" << error_log;
            free(error_log);
        }

        glGetShaderiv(tess_control_shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            std::cout << "\nUh oh! Tessellation control shader didn't compile! \n";
            GLchar* error_log = (GLchar*)malloc(loglen);
            glGetShaderiv(tess_control_shader, GL_INFO_LOG_LENGTH, &loglen);
            glGetShaderInfoLog(tess_control_shader, loglen, &shadelen, error_log);
            std::cout << "compiler_log: \n" << error_log;
            free(error_log);
        }

        glGetShaderiv(tess_eval_shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            std::cout << "\nUh oh! Tessellation control shader didn't compile! \n";
            GLchar* error_log = (GLchar*)malloc(loglen);
            glGetShaderiv(tess_eval_shader, GL_INFO_LOG_LENGTH, &loglen);
            glGetShaderInfoLog(tess_eval_shader, loglen, &shadelen, error_log);
            std::cout << "compiler_log: \n" << error_log;
            free(error_log);
        }

        glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            std::cout << "\nUh oh! Geometry shader didn't compile! \n";
            GLchar* error_log = (GLchar*)malloc(loglen);
            glGetShaderiv(geom_shader, GL_INFO_LOG_LENGTH, &loglen);
            glGetShaderInfoLog(geom_shader, loglen, &shadelen, error_log);
            std::cout << "compiler_log: \n" << error_log;
            free(error_log);
        }

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            std::cout << "\nUh oh! Fragment shader didn't compile! \n";
            GLchar* error_log = (GLchar*)malloc(loglen);
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &loglen);
            glGetShaderInfoLog(fragment_shader, loglen, &shadelen, error_log);
            std::cout << "compiler_log: \n" << error_log;
            free(error_log);
        }


		// create program, attach shaders to it, and link it
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, tess_control_shader);
        glAttachShader(program, tess_eval_shader);
        glAttachShader(program, geom_shader);
        glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// Delete the shaders as the program has them now
		glDeleteShader(vertex_shader);
        glDeleteShader(tess_control_shader);
        glDeleteShader(tess_eval_shader);
        glDeleteShader(geom_shader);
		glDeleteShader(fragment_shader);
        
        //check to make sure that program is ok! (and if not why not)
        GLchar* infoLog = (GLchar*)malloc(loglen);
        GLsizei length = sizeof(infoLog);
        glGetProgramInfoLog(program, loglen, &length, infoLog);
        std::cout << "\nprogram_log: \n" << infoLog;
        free(infoLog);

		return program;
	}
};


int main() {

	my_application triangle;
	int a = 1;

	// First we need to initialize GLFW!
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	std::cout << "hello world!\n";

	// Now we can create our first OpenGL window!
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want old OpenGL
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Shouldn't need this ... try it with and without

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Open a window and create its OpenGL Context
	GLFWwindow* terminal = glfwCreateWindow(800, 600, "Coates_HutchisonTriangle", nullptr, nullptr);

	std::cout << terminal;

	if (terminal == nullptr) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(terminal); // Initialize GLEW
	glewExperimental = true; // Needed in the core profile

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW.\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(terminal, GLFW_STICKY_KEYS, GL_TRUE);

	triangle.startup();

    // uncomment this call to draw in wireframe polygons (or tessellations)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	do {
		// draw nothing
		triangle.render(clock());
		//Swap buffers
		glfwSwapBuffers(terminal);
		glfwPollEvents();

        //in case of errors
        GLenum glerr = glGetError();
        switch (glerr) {
        case GL_INVALID_ENUM:
            fprintf(stderr, "Error with enum!\n");
            break;
        case GL_INVALID_VALUE:
            fprintf(stderr, "Error with value!\n");
            break;
        case GL_INVALID_OPERATION:
            fprintf(stderr, "Error with operation!\n");
            break;
        case GL_STACK_OVERFLOW:
            fprintf(stderr, "Error with stack (overflow)!\n");
            break;
        case GL_STACK_UNDERFLOW:
            fprintf(stderr, "Error with stack (underflow)!\n");
            break;
        case GL_OUT_OF_MEMORY:
            fprintf(stderr, "Error with memory (no more left)!\n");
            break;
        default:
            break;
        }
           
	} // check if the ESC key was pressed or the window was closed
	while (glfwGetKey(terminal, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(terminal) == 0);

	return 0;
}
