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

// include GLU
#include <Windows.h>
#include <gl\gl.h>
// include GLU
#include <gl\glu.h>

//include assimp stuff
#include <assimp-3.3.1\include\assimp\Importer.hpp>
#include <assimp-3.3.1\include\assimp\scene.h>
#include <assimp-3.3.1\include\assimp\postprocess.h>


static const float data3[] = {
    -0.360489, 0.003619, -0.042385,
    -0.480734, 0.401070, -0.129824,
    -0.137408, 0.003619, -0.204321,
    -0.183293, 0.401070, -0.345739,
    0.138251, 0.003619, -0.204207,
    0.184257, 0.401070, -0.345584,
    0.361201, 0.003619, -0.042087,
    0.481520, 0.401070, -0.129424
};

static const float data5[] = {
    -0.480734, 0.401070, -0.129824, //2 
    -0.137408, 0.003619, -0.204321, //3
    -0.360489, 0.003619, -0.042385, //1

    -0.183293, 0.401070, -0.345739, //4
    0.138251, 0.003619, -0.204207, //5
    -0.137408, 0.003619, -0.204321, //3

    0.184257, 0.401070, -0.345584, //6
    0.361201, 0.003619, -0.042087, //7
    0.138251, 0.003619, -0.204207, //5

    0.361201, 0.003619, -0.042087, //7
    -0.480734, 0.401070, -0.129824, //2
    -0.360489, 0.003619, -0.042385, //1

    -0.480734, 0.401070, -0.129824,
    0.184257, 0.401070, -0.345584,
    -0.183293, 0.401070, -0.345739,

    -0.360489, 0.003619, -0.042385,
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
    0.361201, 0.003619, -0.042087,

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
    GLuint myBuff1;
    GLuint myBuff2;
    GLuint vao;
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
    GLint cube = 0;
    int numVerts = 0;
    int numFaces = 0;
    int numLines = 0;

    float* verts; //just stores some verts as floats (not as faces)
    int vertsSize = 0;
    glm::vec3* faces1; //from verts to faces
    int facesSize1 = 0;

    glm::vec3* faces2; //from verts to faces
    int facesSize2 = 0;

    void startup() {
        //verts = new float[numLines];
        faces1 = fillVerts("blender\\SImpleImport3.obj", faces1);
        facesSize1 = numFaces * 9 * 4;
        faces2 = fillVerts("blender\\CubeSpin.obj", faces2);
        facesSize2 = numFaces * 9 * 4;
        //faces = buildFaces();

        //std::cout << numVerts << "\n";
        //std::cout << sizeof(data3) << "\n";

        //std::cout << verts[0] << "\n";
        //std::cout << verts[1] << "\n";
        //std::cout << verts[2] << "\n";
        //std::cout << verts[3] << "\n";
        //std::cout << verts[4] << "\n";


        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glCreateBuffers(1, &myBuff1);
        glCreateBuffers(1, &myBuff2);
        //each vert is a float which takes up 4 bytes!
        glNamedBufferStorage(myBuff1, facesSize1, faces1, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        glNamedBufferStorage(myBuff2, facesSize1, faces2, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        //glNamedBufferStorage(myBuff, vertsSize, verts, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        //glNamedBufferStorage(myBuff, sizeof(data5), data5, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        glBindBuffer(GL_ARRAY_BUFFER, myBuff1);
        glBindBuffer(GL_ARRAY_BUFFER, myBuff2);

        glVertexArrayVertexBuffer(vao, 0, myBuff1, 0, sizeof(glm::vec3));
        glVertexArrayVertexBuffer(vao, 1, myBuff2, 0, sizeof(glm::vec3));
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(vao, 0);
        glEnableVertexArrayAttrib(vao, 1);


        rendering_program = compile_shaders();
    }

    void shutdown() {
        glDisableVertexArrayAttrib(vao, 0);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &myBuff1);
        glDeleteBuffers(1, &myBuff2);
        glDeleteProgram(rendering_program);
        glDeleteVertexArrays(1, &vao);
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


    glm::vec3* fillVerts(const char* filename, glm::vec3* faces) {
        numVerts = 0;
        numFaces = 0;
        numLines = 0;
        //const char* file = "blender\MuseumSubpanels.obj";
        ifstream file;
        file.open(filename);

        if (!file) {
            std::cerr << "Uh oh! There's a problem with this file!\n";
            return NULL;
        }

        string line;
        char thisline[50];
        char car;
        char* nextthing;
        size_t thissize = sizeof(thisline);
        int i = 0;
        float x;
        float y;
        float z;
        int a;
        int b;
        int c;

        //grab/skip first 3-4 lines of obj file
        getline(file, line);
        getline(file, line);
        getline(file, line);

        while (getline(file, line)) {
            if (line[0] == 'v')
                numVerts++;

            if (line[0] == 'f')
                numFaces++;

            numLines++;
        }

        //std::cout << numLines << "\n";
        //each triangulated face has 9 floats in it (3 verts)
        verts = new float[numVerts * 3]();
        faces = new glm::vec3[numFaces * 9]();
        file.clear();
        file.seekg(0);

        getline(file, line);
        getline(file, line);
        getline(file, line);

        while ((file >> car) && (car == 'v')) {    //v for verts
            file >> x >> y >> z;
            verts[i] = x;
            verts[i + 1] = y;
            verts[i + 2] = z;
            i += 3;
        }


        i = 0;
        //skip a line
        getline(file, line);


        while ((file >> car) && (car == 'f')) {    //f for faces
                                                   //file >> c; //for extra v at start of input

            file >> a >> b >> c;

            //1st vert
            faces[i].x = verts[(a - 1) * 3];
            faces[i].y = verts[(a - 1) * 3 + 1];
            faces[i].z = verts[(a - 1) * 3 + 2];

            //2nd vert
            faces[i + 1].x = verts[(b - 1) * 3];
            faces[i + 1].y = verts[(b - 1) * 3 + 1];
            faces[i + 1].z = verts[(b - 1) * 3 + 2];

            //3rd vert
            faces[i + 2].x = verts[(c - 1) * 3];
            faces[i + 2].y = verts[(c - 1) * 3 + 1];
            faces[i + 2].z = verts[(c - 1) * 3 + 2];
            i += 3;
        }

        //for (int m = 0; m < numVerts * 3; m++) {
        //    if (verts[m] != data3[m]) {
        //        std::cout << "These are not the verts you're looking for\n";
        //        std::cout << data3[m] << " neq " << verts[m] << "\n";
        //    }
        //}
        //four bytes per float!
        vertsSize = numVerts * 4 * 3;
        //facesSize = numFaces * 4 * 9;
        file.close();

        return faces;
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
        //glDrawArrays(GL_TRIANGLES, 0, sizeof(data5) / (sizeof(float) * 3));
        //gluLookAt(0.4, 0.3, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        //gluPerspective(45, 1.333, 0, 100);
        //std::cout << gluGetString;
        //glTranslatef(0.5, 0.2, 0.3);


        GLfloat tripos[] = { (float)(sin(currentTime / 1100) * 0.75),
            (float)(cos(currentTime / 1100) * 0.6),
            0.0, 1.0f };

        glVertexAttribI1i(2, cube);
        glVertexAttrib4fv(3, tripos);

        //"Dynamic" drawing!!
        if (cube)
            glDrawArrays(GL_TRIANGLES, 0, facesSize2 / sizeof(glm::vec3));

        else
            glDrawArrays(GL_TRIANGLES, 0, facesSize1 / sizeof(glm::vec3));
        //glDrawArrays(GL_POINTS, 0, vertsSize / (sizeof(float)));

        //for (glm::vec4 v : vertices) {
        //position[0] = v.x;
        //position[1] = v.y;
        //position[2] = v.z;
        //position[3] = 1.0;

        //glVertexAttrib4fv(0, myBuff);
        //}

        //Draw a BIG point! (needed to draw a point)
        glPointSize(15.0f);
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
            " layout (location = 1) in vec3 position2;                                  \n"
            " layout (location = 2) in int cube;                                        \n"
            " layout (location = 3) in vec3 offset;                                     \n"
            "                                                                           \n"
            //" out vec4 vertexPos;                                                       \n"
            "                                                                           \n"
            "                                                                           \n"
            "void main(void)														    \n"
            "{                                                                          \n"
            //"   Normal = normalize(vec3(viewMatrix * modelMatrix * vec4(normal,0.0)));  \n"
            //"   gl_Position = projMatrix * viewMatrix * modelMatrix *                   \n"
            //"                 vec4(position,1.0);                                       \n"
            "   if (cube != 0)                                                          \n"
            "       gl_Position = vec4(position2 + offset, 1.0);                        \n"
            "   else                                                                    \n"
            "       gl_Position = vec4(position + offset, 1.0);                         \n"
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
    int wire = 0;
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



    do {
        // draw nothing
        triangle.render(clock());
        //Swap buffers
        glfwSwapBuffers(terminal);
        glfwPollEvents();

        if (glfwGetKey(terminal, GLFW_KEY_C) == GLFW_PRESS) {
            triangle.cube = 1;
        }
                
        else if (glfwGetKey(terminal, GLFW_KEY_V) == GLFW_PRESS) {
            triangle.cube = 0;
        }


        else if (glfwGetKey(terminal, GLFW_KEY_Z) == GLFW_PRESS) {
            // uncomment this call to draw in wireframe polygons (or tessellations)
            if (!wire) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wire = 1;
            }
        }

        else if (glfwGetKey(terminal, GLFW_KEY_X) == GLFW_PRESS) {
            // uncomment this call to draw in wireframe polygons (or tessellations)
            if (wire) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wire = 0;
            }
        }


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
