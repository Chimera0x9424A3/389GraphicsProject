#include "Import.h"

//how big is our window?
short WIN_HEIGHT = 1200;
short WIN_LENG = 1500;

//globals from header file Import.h
int numVerts = 0;
int numNorms = 0;
int numFaces = 0;
int numLines = 0;
int numUVs = 0;

//array data and corresponding sizes
//sizes are are useful for when these are attached to vbos
float* verts; //just stores some verts as floats (not as faces)
int vertsSize = 0;

glm::vec3* colors; //do color calculation based on object name in obj file
int colorsSize = 0;

glm::vec3* norms; //list of all normals
int normsSize = 0;

glm::vec3* normals; //normals for faces (from norms)
int normalsSize = 0;

glm::vec2* uvs; //list of all uvs
int uvsSize = 0;

glm::vec2* textures; //list of texture coords (from uvs)
int texturesSize = 0;

glm::vec4* faces; //from verts to faces (3 verts per face)
glm::vec4* facesReset;  //for reset R case to reset model position/rotation
int facesSize1 = 0;

int* texmatData;

// Create a class for our application
class my_application {
private:
    GLuint rendering_program;
    //for entire museum: art and model and walls and errything
    GLuint myBuff1;
    GLuint myBuff2;
    GLuint myBuff3;
    GLuint myBuff4;
    //figure no more than 15 textures (10 so far)
    GLuint myTextures[15];
    GLuint vao;

//shader stuff, program, scene, art focus flag,
public:
    GLuint vertex_shader;
    GLuint tess_control_shader;
    GLuint tess_eval_shader;
    GLuint geom_shader;
    GLuint fragment_shader;
    GLuint program;
    const aiScene* myscene;
    GLint art = 0;

    void startup() {
        //load faces array as well as normals, colors, and textures with help from import.cpp
        faces = fillVerts("blender\\MuseumSolid.obj", faces);
        if (faces == NULL) {
            return;
        }

        //separate buffer set up in case user wants to "reset" model position and location
        //just fill with initial faces data
        facesReset = new glm::vec4[numFaces * 9]();
        for (int i = 0; i < numFaces * 9; i++)
            facesReset[i] = faces[i];

        //size of faces buffer in bytes
        facesSize1 = numFaces * 3 * 16;

        //need a vao
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //buffers for faces, normals, colors, and textures
        //make them, link them with our filled arrays, bind them, and provide them as shader input
        glCreateBuffers(1, &myBuff1);
        glCreateBuffers(1, &myBuff2);
        glCreateBuffers(1, &myBuff3);
        glCreateBuffers(1, &myBuff4);

        //each vert is a float which takes up 4 bytes!
        glNamedBufferStorage(myBuff1, facesSize1, faces, GL_MAP_READ_BIT | GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        glNamedBufferStorage(myBuff2, normsSize, normals, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        glNamedBufferStorage(myBuff3, colorsSize, colors, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        glNamedBufferStorage(myBuff4, texturesSize, textures, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, myBuff1);
        glBindBuffer(GL_ARRAY_BUFFER, myBuff2);
        glBindBuffer(GL_ARRAY_BUFFER, myBuff3);
        glBindBuffer(GL_ARRAY_BUFFER, myBuff4);

        glVertexArrayVertexBuffer(vao, 0, myBuff1, 0, sizeof(glm::vec4));
        glVertexArrayVertexBuffer(vao, 1, myBuff2, 0, sizeof(glm::vec3));
        glVertexArrayVertexBuffer(vao, 4, myBuff3, 0, sizeof(glm::vec3));
        glVertexArrayVertexBuffer(vao, 5, myBuff4, 0, sizeof(glm::vec2));

        glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(vao, 4, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(vao, 5, 2, GL_FLOAT, GL_FALSE, 0);

        glEnableVertexArrayAttrib(vao, 0);
        glEnableVertexArrayAttrib(vao, 1);
        glEnableVertexArrayAttrib(vao, 4);
        glEnableVertexArrayAttrib(vao, 5);
     
        //generate/link textures and shaders
        rendering_program = compile_shaders();
        loadTextures(myTextures, rendering_program);
    }

    void shutdown() {
        //free/delete used resources
        glDisableVertexArrayAttrib(vao, 0);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &myBuff1);
        glDeleteBuffers(1, &myBuff2);
        glDeleteBuffers(1, &myBuff3);
        glDeleteBuffers(1, &myBuff4);
        glDeleteTextures(15, myTextures);
        glDeleteProgram(rendering_program);
        glDeleteVertexArrays(1, &vao);
    }


    // Our rendering function
    //Includes fancier versions!
    void render(double currentTime) {
        //cull stuff and enable depth testing so model isn't seethru
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_TEST);

        glNamedBufferSubData(myBuff1, 0, facesSize1, faces);

        // Use the program object we created earlier for rendering
        glUseProgram(rendering_program);

        //use kind of like flag for art focus mode
        GLfloat artpos[] = {0.0f, 0.0f, 0.0f};

        if (art)
            artpos[2] = 1.0f;

        else
            artpos[2] = 0.0f;

        glVertexAttrib4fv(3, artpos);

        //Draw our stuff!
        glDrawArrays(GL_TRIANGLES, 0, facesSize1 / sizeof(glm::vec4));
    }


    // Shader compiler
    GLuint compile_shaders(void) {
        // Source code for vertex shader
        //switch is based on color setter switch in import.cpp
        //switch allows us to toggle display of different objects for "art focus" mode
        //each one is tagged with a number corresponding to the first letter of its "name"
        //art objects are displayed as long as artSelect is on (toggled with user input)
        static const GLchar * vertex_shader_source[] = {
            "#version 450 core                                  \n"
            "                                                   \n"
            " layout (location = 0) in vec4 position;           \n"
            " layout (location = 1) in vec3 normal;             \n"
            " layout (location = 3) in vec3 art;                \n"
            " layout (location = 4) in vec3 color;              \n"
            " layout (location = 5) in vec2 tex;                \n"
            "out vec3 Normals;                                  \n"
            "out vec3 Colors;                                   \n"
            "out vec2 Texs;                                     \n"
            "                                                   \n"
            "                                                   \n"
            "void main(void)									\n"
            "{                                                  \n"
            "   int textureSelect = int(color.x);               \n"
            "   int artSelect = int(art.z);                     \n"
            "   if (artSelect == 1) {                           \n"
            "       switch (textureSelect) {                    \n"
            "       case 2:                                     \n"
            "       case 4:                                     \n"
            "       case 23:                                    \n"
            "       case 24:                                    \n"
            "       case 52:                                    \n"
            "       case 51:                                    \n"
            "       case 53:                                    \n"
            "       case 54:                                    \n"
            "           gl_Position = vec4(position);           \n"
             "           break;                                 \n"
            "       default:                                    \n"
            "               break;                              \n"
            "       }                                           \n"
            "    }                                              \n"
            "   else {                                          \n"
            "       gl_Position = vec4(position);               \n"
            "    }                                              \n"
            "                                                   \n"
            "   Normals = normal;                               \n"
            "   Colors = color;                                 \n"
            "   Texs = tex;                                     \n"
            "}                                                  \n"
        };



        // Source code for fragment shader
        //each uniform sampler corresponds to vars from loadTextures function in import.cpp
        //switch is based on color setter switch in import.cpp
        //switch allows us to color/texture different objects in different ways
        //each one is tagged with a number corresponding to the first letter of its "name"
        static const GLchar * fragment_shader_source[] = {
            "#version 450 core                                  \n"
            "in vec3 Normals;                                   \n"
            "in vec3 Colors;                                    \n"
            "in vec2 Texs;                                      \n"
            "uniform sampler2D fbwall;                          \n"
            "uniform sampler2D ceiling;                         \n"
            "uniform sampler2D base;                            \n"
            "uniform sampler2D lrwall;                          \n"
            "uniform sampler2D doorway;                         \n"
            "uniform sampler2D panel1;                          \n"
            "uniform sampler2D panel3;                          \n"
            "uniform sampler2D panel4;                          \n"
            "uniform sampler2D floor;                           \n"
            "uniform sampler2D bigred;                          \n"
            "uniform sampler2D frames;                          \n"
            "out vec4 triColor;                                 \n"
            "                                                   \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            " vec4 col;                                         \n"
            " int textureSelect = int(Colors.x);                \n"
            "   switch (textureSelect) {                        \n"
            "       case 1:                                     \n"
            "       case 12:                                    \n"
            "           col = texture(base, Texs);              \n"
            "           break;                                  \n"
            "       case 2:                                     \n"
            "           col = texture(ceiling, Texs);           \n"
            "           break;                                  \n"
            "       case 52:                                    \n"
            "       case 3:                                     \n"
            "           col = texture(doorway, Texs);           \n"
            "           break;                                  \n"
            "       case 4:                                     \n"
            "           col = texture(frames, Texs);            \n"
            "           break;                                  \n"
            "       case 5:                                     \n"
            "       case 15:                                    \n"
            "           col = texture(floor, Texs);             \n"
            "           break;                                  \n"
            "       case 16:                                    \n"
            "       case 25:                                    \n"
            "           col = texture(lrwall, Texs);            \n"
            "           break;                                  \n"
            "       case 23:                                    \n"
            "           col = texture(bigred, Texs);            \n"
            "           break;                                  \n"
            "       case 24:                                    \n"
            "           col = vec4(0.6, 0.6, 0.6, 1.0);         \n"
            "           break;                                  \n"
            "       case 51:                                    \n"
            "           col = texture(panel1, Texs);            \n"
            "           break;                                  \n"
            "       case 53:                                    \n"
            "           col = texture(panel3, Texs);            \n"
            "           break;                                  \n"
            "       case 54:                                    \n"
            "           col = texture(panel4, Texs);            \n"
            "           break;                                  \n"
            "       default:                                    \n"
            "           col = texture(fbwall, Texs);            \n"
            "           break;                                  \n"
            "   }                                               \n"
            "     float r = Colors.x;                           \n"
            "     float g = Colors.y;                           \n"
            "     float b = Colors.z;                           \n"
            "   triColor = col;                                 \n"
            "}                                                  \n"
        };

        //create and compile vertex shader
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, vertex_shader_source, nullptr);
        glCompileShader(vertex_shader);

        //create and compile fragment shader
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, fragment_shader_source, nullptr);
        glCompileShader(fragment_shader);


        //check to make sure that shaders compiled! (and if not why not)
        GLint compiled;
        GLint loglen = 500;
        GLsizei shadelen = 0;

        //make SURE vert shader compiled
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

        //make SURE frag shader compiled
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

        //check to make sure that program is ok! (and if not why not)
        //updates with shaders to provides real-time debuggin info
        GLchar* infoLog = (GLchar*)malloc(loglen);
        GLsizei length = sizeof(infoLog);
        glGetProgramInfoLog(program, loglen, &length, infoLog);
        std::cout << "\nprogram_log: \n" << infoLog;
        free(infoLog);


        return program;
    }
};

int main() {
    //declare some matrices and necessary environment stuff
    my_application triangle;
    int a = 1;
    int wire = 0;
    glm::mat4 myMatrix;
    glm::mat4 forward;
    glm::mat4 backward;
    glm::mat4 spinUp;
    glm::mat4 spinDown;
    glm::mat4 lookLeft;
    glm::mat4 lookRight;
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CCW);

    //matrices for moving the model around based on user input
    forward = glm::mat4(1.007, 0, 0, 0,
        0, 1.007, 0, 0,
        0, 0, 1.007, 0,
        0, 0, 0, 1);

    backward = glm::mat4(0.993, 0, 0, 0,
        0, 0.993, 0, 0,
        0, 0, 0.993, 0,
        0, 0, 0, 1);

    spinUp = glm::mat4(1, 0, 0, 0,
        0, 0.993, 0.118, 0,
        0, -0.118, 0.993, 0,
        0, 0, 0, 1);

    spinDown = glm::mat4(1, 0, 0, 0,
        0, 0.993, -0.118, 0,
        0, 0.118, 0.993, 0,
        0, 0, 0, 1);

    lookLeft = glm::mat4(0.993, 0, 0.118, 0,
        0, 1, 0, 0,
        -0.118, 0, 0.993, 0,
        0, 0, 0, 1);

    lookRight = glm::mat4(0.993, 0, -0.118, 0,
        0, 1, 0, 0,
        0.118, 0, 0.993, 0,
        0, 0, 0, 1);

    // First we need to initialize GLFW!
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    //print welcome message
    std::cout << "Welcome to our AWESOME museum!!\n";

    // Now we can create our first OpenGL window!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want old OpenGL
                                                                   //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Shouldn't need this ... try it with and without

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Open a window and create its OpenGL Context
    GLFWwindow* terminal = glfwCreateWindow(WIN_LENG, WIN_HEIGHT, "Coates_HutchisonMuseum", nullptr, nullptr);

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
    //start program!
    triangle.startup();

    do {
        // draw nothing
        triangle.render(clock());
        //Swap buffers
        glfwSwapBuffers(terminal);
        glfwPollEvents();

        //do you just want to focus on our marvelous art??!!
        if (glfwGetKey(terminal, GLFW_KEY_Q) == GLFW_PRESS) {
            triangle.art = 1;
        }
        else if (glfwGetKey(terminal, GLFW_KEY_E) == GLFW_PRESS) {
            triangle.art = 0;
        }

        //wireframe! or solid. you DECIDE!
        else if (glfwGetKey(terminal, GLFW_KEY_Z) == GLFW_PRESS) {
            if (!wire) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wire = 1;
            }
        }
        else if (glfwGetKey(terminal, GLFW_KEY_C) == GLFW_PRESS) {
            if (wire) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wire = 0;
            }
        }

        //rotations (left, right, up, and down)
        //also R to reset the model
        else if (glfwGetKey(terminal, GLFW_KEY_W) == GLFW_PRESS) {
            rotateStuff(faces, spinUp);
        }
        else if (glfwGetKey(terminal, GLFW_KEY_S) == GLFW_PRESS) {
            rotateStuff(faces, spinDown);
        }
        else if (glfwGetKey(terminal, GLFW_KEY_A) == GLFW_PRESS) {
            rotateStuff(faces, lookLeft);
        }
        else if (glfwGetKey(terminal, GLFW_KEY_D) == GLFW_PRESS) {
            rotateStuff(faces, lookRight);
        }
        else if (glfwGetKey(terminal, GLFW_KEY_R) == GLFW_PRESS) {
            for (int i = 0; i < numFaces * 9; i++)
                faces[i] = facesReset[i];
        }

        //translations (left, right) and scalings (backward/forward)
        else if (glfwGetKey(terminal, GLFW_KEY_UP) == GLFW_PRESS) {
            rotateStuff(faces, forward);
        }
        else if (glfwGetKey(terminal, GLFW_KEY_DOWN) == GLFW_PRESS) {
            rotateStuff(faces, backward);
        }
        else if (glfwGetKey(terminal, GLFW_KEY_LEFT) == GLFW_PRESS) {
            myMatrix = glm::translate(glm::mat4(), glm::vec3(-0.004f, 0.0f, 0.0f));
            rotateStuff(faces, myMatrix);
        }
        else if (glfwGetKey(terminal, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            myMatrix = glm::translate(glm::mat4(), glm::vec3(0.004f, 0.0f, 0.0f));
            rotateStuff(faces, myMatrix);
        }


        //in case of errors, debugging info (check glerr)
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
