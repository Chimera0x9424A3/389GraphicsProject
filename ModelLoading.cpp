#include <iostream>
#include <fstream>
using namespace std;
#include <string>
// Include standard headers
#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Include GLEW.  Always include it bofore gl.h, and glfw.h ... its a little magic
#include <glew\include\GL\glew.h>

// include GLFW
#include <glfw\include\GLFW\glfw3.h>

// Include GLM.
// We don't need it yet, but it is a library for 3D maths
#include <glm\glm\glm.hpp>

//include assimp stuff
#include <assimp-3.3.1\include\assimp\Importer.hpp>
#include <assimp-3.3.1\include\assimp\scene.h>
#include <assimp-3.3.1\include\assimp\postprocess.h>


static const float data5[] = {

    -0.480734, 0.401070, - 0.129824,
    - 0.137408, 0.003619, - 0.204321,
    - 0.360489, 0.003619, - 0.042385,

    - 0.183293, 0.401070, - 0.345739,
    0.138251, 0.003619, -0.204207,
    -0.137408, 0.003619, -0.204321,

    0.184257, 0.401070, - 0.345584,
    0.361201, 0.003619, -0.042087,
    0.138251, 0.003619, -0.204207,

    0.361201, 0.003619, -0.042087,
    -0.480734, 0.401070, -0.129824,
    -0.360489, 0.003619, -0.042385,

    - 0.480734, 0.401070, -0.129824,
    0.184257, 0.401070, -0.345584,
    -0.183293, 0.401070, -0.345739,

    - 0.360489, 0.003619, -0.042385,
    0.138251, 0.003619, -0.204207,
    0.361201, 0.003619, -0.042087,

    -0.480734, 0.401070, -0.129824,
    -0.183293, 0.401070, -0.345739,
    -0.137408, 0.003619, -0.204321,

    -0.183293, 0.401070, -0.345739,
    0.184257, 0.401070, -0.345584,
    0.138251, 0.003619, -0.204207,

    0.184257, 0.401070, -0.345584, 
    0.481520, 0.401070, -0.129424,
    0.361201, 0.003619, - 0.042087,

    0.361201, 0.003619, -0.042087,
    0.481520, 0.401070, -0.129424,
    -0.480734, 0.401070, -0.129824,

    -0.480734, 0.401070, -0.129824,
    0.481520, 0.401070, -0.129424,
    0.184257, 0.401070, -0.345584,

    -0.360489, 0.003619, -0.042385,
    -0.137408, 0.003619, -0.204321,
    0.138251, 0.003619, -0.204207


// - 0.360489, 0.003619, - 0.042385, 1
// - 0.480734, 0.401070, - 0.129824, 2
// - 0.137408, 0.003619, - 0.204321, 3
// - 0.183293, 0.401070, - 0.345739, 4
// 0.138251, 0.003619, -0.204207, 5
// 0.184257, 0.401070, - 0.345584, 6
// 0.361201, 0.003619, - 0.042087, 7
// 0.481520, 0.401070, - 0.129424 8

};



// Create a class for our application
class my_application {
    struct Vertex {
        glm::vec3 Position;
        //glm::vec3 Normal;
        //glm::vec2 TexCoords;
    };

private:
    GLuint rendering_program;
    GLuint vao;
    GLuint myBuff;
    Assimp::Importer importStuff;


public:
    GLuint vertex_shader;
    GLuint tess_control_shader;
    GLuint tess_eval_shader;
    GLuint geom_shader;
    GLuint fragment_shader;
    GLuint program;
    const aiScene* myscene;
    std::list <glm::vec4> vertices;
    std::list <glm::vec4>::iterator it = vertices.begin();
    //fill this with verts using a function
    //glm::vec3* verts = new glm::vec3[2000];
    float* verts = new float[2000];

    void startup() {
        verts = fillVerts("blender\\SImpleImport3.obj", verts);
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glCreateBuffers(1, &myBuff);
        //glNamedBufferStorage(myBuff, sizeof(verts), verts, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        glNamedBufferStorage(myBuff, sizeof(data5), data5, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, myBuff);

        glVertexArrayVertexBuffer(vao, 0, myBuff, 0, sizeof(glm::vec3));
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(vao, 0);

        rendering_program = compile_shaders();
    }

    void shutdown() {
        glDisableVertexArrayAttrib(vao, 0);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &myBuff);
        glDeleteProgram(rendering_program);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &myBuff);
    }

    //start working with assimp: create importer
    bool import(const char* pFile) {
        myscene = importStuff.ReadFile(pFile,
            //aiProcess_CalcTangentSpace,
            aiProcess_FlipUVs);
        //aiProcess_SortByPType);

        if (!myscene) {
            std::cout << "\nSomething weird happened on import!! \n" << importStuff.GetErrorString();
            return false;
        }

        return true;
    }

     void renderstuff(aiNode* node, const struct aiScene* scene) {
         // process all the node's meshes (if any)
         for (unsigned int i = 0; i < node->mNumMeshes; i++)
         {
             aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
             aiVector3D* aiVector = mesh->mVertices;
             
             for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                 aiVector3t<float> vec = aiVector[j];
                 glm::vec4 vertex = glm::vec4(vec[0], vec[1], vec[2], 1.0);
                 vertices.insert(it, vertex);
             }
         }
         // then do the same for each of its children
         for (unsigned int i = 0; i < node->mNumChildren; i++)
         {
             renderstuff(node->mChildren[i], scene);
         }

         printf("There are %d vertices in this whole mesh \n", vertices.size());
    }


     float* fillVerts(const char* filename, float* verts) {
         //const char* file = "blender\MuseumSubpanels.obj";

         /*if (!import(filename)) {
         fprintf(stderr, "Failed to import file\n");
         return;
         }*/
         ifstream file;
         file.open(filename);

         if (!file) {
             std::cerr << "Uh oh! There's a problem with this file!\n";
         }

         string line;
         char thisline[50];
         char* nextthing;
         size_t thissize = sizeof(thisline);
         int i = 0;
         int k = 0;
         char* y;
         char* z;
         getline(file, line);
         std::cout << line << "\n";
         getline(file, line);
         std::cout << line << "\n";
         getline(file, line);
         std::cout << line << "\n";

         while (getline(file, line) && (line.front() == 'v')) {
             //getline(file, line);
             //std::cout << line << "\n";
             line = line.substr(2, line.length());
             //std::cout << line << "\n";

             if (line.copy(thisline, line.length(), 0) != line.length()) {
                std::cerr << "Uh oh! There was a problem with this copy!\n";
            }

             thisline[line.length()] = '\0';

             verts[i] = strtof(thisline, &y);
             verts[i+1] = strtof(y, &z);
             verts[i+2] = strtof(z, NULL);
             //lines[i] = line.substr(2, line.length);
             //lines[i] = line.substr(2, line.length);

             //verts[k].Position.x = strtof(lines[0], NULL);
             //std::cout << verts[i].x << "\n";
             //verts[k+1].Position.y = strtof(lines[1], NULL);
             //std::cout << verts[i].y << "\n";
             //verts[k+2].Position.z = strtof(lines[2], NULL);
             //std::cout << verts[i].z << "\n";
             i+=3;
         }

         return verts;
     }


    // Our rendering function
    //Includes fancier versions!
    void render(double currentTime) {
        //Fancier one from the book which color cycles: violet to dark blue
        //changing the divisor of currentTime changes speed of color cycle
        float winR = (float)sin(currentTime / 1100) * 0.25f + 0.3f;
        float winG = 0.0f;
        float winB = (float)sin(currentTime / 1100) * 0.05f + 0.55f;
        const GLfloat Wincolor[] = { winR, winG, winB, 1.0f };

        glClearBufferfv(GL_COLOR, 0, Wincolor);

        // Use the program object we created earlier for rendering
        glUseProgram(rendering_program);

        //Draw stuff (for triangles)!
        //vec3 or vec4
        glDrawArrays(GL_TRIANGLES, 0, sizeof(data5) / (sizeof(float) * 3));
        //glDrawArrays(GL_POINTS, 0, sizeof(verts) / (sizeof(float) * 3));

        //for (glm::vec4 v : vertices) {
            //position[0] = v.x;
            //position[1] = v.y;
            //position[2] = v.z;
            //position[3] = 1.0;

            //glVertexAttrib4fv(0, myBuff);
        //}

        //Draw a BIG point! (needed to draw a point)
        glPointSize(12.0f);
    }


    // Shader compiler
    GLuint compile_shaders(void) {
        // Source code for vertex shader
        // Can play with triangle position using offset variable (declared as in)!
        // Also adjusting triangle color; uses interface block for output
        static const GLchar * vertex_shader_source[] = {
            "#version 450 core                                                          \n"
            "                                                                           \n"
            " layout (location = 0) in vec3 position;                                   \n"
            "                                                                           \n"
            //" out vec4 vertexPos;                                                       \n"
            "                                                                           \n"
            "                                                                           \n"
            "void main(void)														    \n"
            "{                                                                          \n"
            //"   Normal = normalize(vec3(viewMatrix * modelMatrix * vec4(normal,0.0)));  \n"
            //"   gl_Position = projMatrix * viewMatrix * modelMatrix *                   \n"
            //"                 vec4(position,1.0);                                       \n"
            "   gl_Position = vec4(position, 1.0);                                                 \n"
            "}                                                                          \n"
        };



        // Source code for fragment shader
        // Creates funny triangle colors based on the coordinates of each fragment
        static const GLchar * fragment_shader_source[] = {
            "#version 450 core                                  \n"
            "                                                   \n"
            "out vec4 triColor;                                 \n"
            "                                                   \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            "   triColor = vec4(sin(gl_FragCoord.x * 0.075),     \n"
            "                sin(gl_FragCoord.y * 0.025),        \n"
            "                sin(gl_FragCoord.x * 0.05),         \n"
            "                1.0);                              \n"
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
        //glBindAttribLocation(program, 0, "position");
        glLinkProgram(program);

        // Delete the shaders as the program has them now
        glDeleteShader(vertex_shader);
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
    //printf("%d\n", sizeof(data) / (sizeof(float) * 3));

    // First we need to initialize GLFW!
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    std::cout << "hello world!\n";
    //aiMesh::mVertices;
    //printf("\nThis scene has %d meshes \n", triangle.myscene->mNumMeshes);

    //triangle.renderstuff(triangle.myscene->mRootNode, triangle.myscene);


    // Now we can create our first OpenGL window!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want old OpenGL
                                                                   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Shouldn't need this ... try it with and without

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Open a window and create its OpenGL Context
    GLFWwindow* terminal = glfwCreateWindow(1000, 750, "Coates_HutchisonTriangle", nullptr, nullptr);

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
