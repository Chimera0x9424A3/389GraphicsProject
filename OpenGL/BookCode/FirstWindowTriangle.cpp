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

		//Fancier one from the book which color cycles!!!
		//changing the divisor of currentTime changes speed of color cycle
		const GLfloat color[] = { (float)sin(currentTime/1500) * 0.25f + 0.5f, 0.0f,
                                  (float)cos(currentTime/1500) * 0.25f + 0.5f, 1.0f };

		glClearBufferfv(GL_COLOR, 0, color);

		// Use the program object we created earlier for rendering
		glUseProgram(rendering_program);

        //Makes the triangle move around the window!
        GLfloat attrib[] = { (float)sin(currentTime / 1500) * 0.7,
                             (float)cos(currentTime / 1500) * 0.3,
                             0.0, 1.0f };
        glVertexAttrib4fv(0, attrib);

        //Draw stuff!
		glDrawArrays(GL_TRIANGLES, 0, 3);
        //Draw a BIG point! (needed to draw a point)
		//glPointSize(25.0f);
	}


	// Shader compiler
	GLuint compile_shaders(void) {
		// Source code for vertex shader
        // Can play with triangle position using offset variable!
		static const GLchar * vertex_shader_source[] = {
			"#version 450 core                                                          \n"
			"                                                                           \n"
            "layout (location = 0) in vec4 offset;                                      \n"
			"void main(void)														    \n"
			"{                                                                          \n"
            "   const vec4 vertices[3] = vec4[3](vec4(  0.25,  -0.25,  0.5,  1.0),      \n"
            "                                    vec4( -0.25,  -0.25,  0.5,  1.0),      \n"
            "                                    vec4(  0.25,   0.25,  0.5,  1.0));     \n"
            "   gl_Position = vertices[gl_VertexID] + offset;                           \n"
			"}                                                                          \n"
		};

		// Source code for fragment shader
		static const GLchar * fragment_shader_source[] = {
			"#version 450 core                                  \n"
			"                                                   \n"
			"out vec4 color;                                    \n"
			"                                                   \n"
			"void main(void)                                    \n"
			"{                                                  \n"
			"   color = vec4(0.25f + 0.5f, 0.75f, 0.5f, 1.0f);  \n"
			"}                                                  \n"
		};

		// create and compile vertex shader
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, nullptr);
		glCompileShader(vertex_shader);

		// create and compile fragment shader
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, nullptr);
		glCompileShader(fragment_shader);


        //check to make sure that shaders compiled! (and if not why not)
        GLint compiled;
        GLint loglen = 0;
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
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// Delete the shaders as the program has them now
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
        
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
	GLFWwindow* terminal = glfwCreateWindow(800, 600, "Milagre Coates & Will Hutchison", nullptr, nullptr);

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

	do {
		// draw nothing
		triangle.render(clock());
		//Swap buffers
		glfwSwapBuffers(terminal);
		glfwPollEvents();
	} // check if the ESC key was pressed or the window was closed

	while (glfwGetKey(terminal, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(terminal) == 0);

	return 0;
}
