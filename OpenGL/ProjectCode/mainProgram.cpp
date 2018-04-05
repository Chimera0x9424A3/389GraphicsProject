#include "Import.h"

//enum Camera_Movement {
//    FORWARD,
//    BACKWARD,
//    LEFT,
//    RIGHT
//};
//
//enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
//int Axis = Xaxis;
short WIN_HEIGHT = 800;
short WIN_LENG = 1000;
//
//GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };

//globals from header file
int numVerts = 0;
int numNorms = 0;
int numFaces = 0;
int numLines = 0;
int numUVs = 0;
//GLuint tex;
float* verts; //just stores some verts as floats (not as faces)
int vertsSize = 0;

glm::vec3* colors; //do color calculation based on object in obj file
int colorsSize = 0;

glm::vec3* norms; //normals for some verts
int normsSize = 0;

glm::vec3* normals; //normals for all verts
int normalsSize = 0;

glm::vec2* uvs; //list of all uvs
int uvsSize = 0;

glm::vec2* textures; //list of texture coords (from uvs)
int texturesSize = 0;

glm::vec4* faces; //from verts to faces
int facesSize1 = 0;

glm::vec3* faces2; //from verts to faces
int facesSize2 = 0;


// Create a class for our application
class my_application {

    struct Vertex {
        glm::vec3 Position;
        //glm::vec3 Normal;
        //glm::vec2 TexCoords;
    };

    struct Camera {
        glm::vec3 Position;
        glm::vec3 Direction;
    };

private:
    GLuint rendering_program;
    GLuint myBuff1;
    GLuint myBuff2;
    GLuint myBuff3;
    GLuint myBuff4;
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
    GLint cube = 0;
    GLint rotate = 0;
    //std::list <glm::vec4> vertices;
    //std::list <glm::vec4>::iterator it = vertices.begin();


    void startup() {
        //faces = fillVerts("C:\\Users\\Cocoa\\source\\repos\\GraphicsProject\\GraphicsProject\\blender\\MuseumPartial.obj", faces);
        faces = fillVerts("blender\\MuseumPartial.obj", faces);
        if (faces == NULL) {
            return;
        }

        facesSize1 = numFaces * 3 * 16;
        //glEnable(GL_TEXTURE_2D);

        //faces2 = fillVerts("blender\\CubeSpinBig.obj", faces2);
        //facesSize2 = numFaces * 9 * 4;

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //buffers for faces and verts
        glCreateBuffers(1, &myBuff1);
        glCreateBuffers(1, &myBuff2);
        glCreateBuffers(1, &myBuff3);
        glCreateBuffers(1, &myBuff4);

        //each vert is a float which takes up 4 bytes!
        glNamedBufferStorage(myBuff1, facesSize1, faces, GL_MAP_READ_BIT | GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        //glNamedBufferStorage(myBuff2, facesSize2, faces2, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        glNamedBufferStorage(myBuff2, normsSize, normals, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

        glNamedBufferStorage(myBuff3, colorsSize, colors, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

        glNamedBufferStorage(myBuff4, texturesSize, textures, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        //glNamedBufferStorage(myBuff, vertsSize, verts, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        //glNamedBufferStorage(myBuff, sizeof(data5), data5, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
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
     
        //glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
        //glShadeModel(GL_SMOOTH);
        rendering_program = compile_shaders();
    }

    void shutdown() {
        glDisableVertexArrayAttrib(vao, 0);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &myBuff1);
        glDeleteBuffers(1, &myBuff2);
        glDeleteBuffers(1, &myBuff3);
        glDeleteBuffers(1, &myBuff4);
        glDeleteTextures(1, &tex);
        glDeleteProgram(rendering_program);
        glDeleteVertexArrays(1, &vao);
    }


    // Our rendering function
    //Includes fancier versions!
    void render(double currentTime) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_CULL_FACE);
        //glFrontFace(GL_CW);
        //glCullFace(GL_BACK);
        //glDepthRange(-1, 1);
        //glEnable(GL_DEPTH_TEST);
        glNamedBufferSubData(myBuff1, 0, facesSize1, faces);

        //Fancier one from the book which color cycles: violet to dark blue
        //changing the divisor of currentTime changes speed of color cycle
        //float winR = (float)sin(currentTime / 1100) * 0.25f + 0.3f;
        //float winG = 0.0f;
        //float winB = (float)sin(currentTime / 1100) * 0.05f + 0.55f;
        //const GLfloat Wincolor[] = { winR, winG, winB, 1.0f };
        //const GLfloat Wincolor[] = { 0, 0, 0, 1.0f };

        //glClearBufferfv(GL_COLOR, 0, Wincolor);

        // Use the program object we created earlier for rendering
        glUseProgram(rendering_program);

        //Draw stuff (for triangles)!
        //vec3 or vec4
        //glDrawArrays(GL_TRIANGLES, 0, sizeof(data5) / (sizeof(float) * 3));
        //glm::vec3 eye = glm::vec3(0.4f, 0.3f, 0.5f);
        //glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        //glm::mat4 look; //= glm::lookAt(eye, center, up);
        //glm::perspective(45.0f, 1.333f, 0.0f, 100.0f);
        //std::cout << gluGetString;
        //glTranslatef(0.5, 0.2, 0.3);


        GLfloat tripos[] = { (float)(sin(currentTime / 1100) * 0.75),
            (float)(cos(currentTime / 1100) * 0.6),
            0.0, 1.0f };

        //glVertexAttribI1i(2, rotate);
        glVertexAttrib4fv(3, tripos);
        //glVertexAttrib4fv(5, look);

        //"Dynamic" drawing!!
        glDrawArrays(GL_TRIANGLES, 0, facesSize1 / sizeof(glm::vec4));

        //rotate = 0;
        //Draw a BIG point! (needed to draw a point)
        //glPointSize(15.0f);
    }


    // Shader compiler
    GLuint compile_shaders(void) {
        // Source code for vertex shader
        // Can play with triangle position using offset variable (declared as in)!
        // Also adjusting triangle color; uses interface block for output
        static const GLchar * vertex_shader_source[] = {
            "#version 450 core                                                          \n"
            "                                                                           \n"
            " layout (location = 0) in vec4 position;                                   \n"
            " layout (location = 1) in vec3 normal;                                     \n"
            //" layout (location = 2) in int rotate;                                      \n"
            " layout (location = 3) in vec3 offset;                                     \n"
            " layout (location = 4) in vec3 color;                                      \n"
            " layout (location = 5) in vec2 tex;                                        \n"
            "out vec3 Normals;                                                          \n"
            "out vec3 Colors;                                                           \n"
            "out vec2 Texs;                                                             \n"
            "                                                                           \n"
            //" out vec4 vertexPos;                                                       \n"
            "                                                                           \n"
            "                                                                           \n"
            "void main(void)														    \n"
            "{                                                                          \n"
            //"   Normal = normalize(vec3(viewMatrix * modelMatrix * vec4(normal,0.0)));  \n"
            //"   gl_Position = projMatrix * viewMatrix * modelMatrix *                   \n"
            //"                 vec4(position,1.0);                                       \n"
            //"   if (cube != 0)                                                          \n"
            //"       gl_Position = vec4(position2 + offset, 1.0);                        \n"
            //"   else                                                                    \n"
            "       gl_Position = vec4(position);                                       \n"
            //"       gl_Position = vec4(position) * ;                                    \n"
            "                                                                           \n"
            "   Normals = normal;                                                       \n"
            "   Colors = color;                                                         \n"
            "   Texs = tex;                                                             \n"
            "}                                                                          \n"
        };



        // Source code for fragment shader
        static const GLchar * fragment_shader_source[] = {
            "#version 450 core                                  \n"
            "in vec3 Normals;                                   \n"
            "in vec3 Colors;                                    \n"
            "in vec2 Texs;                                      \n"
            "uniform sampler2D tex2d;                          \n"
            "out vec4 triColor;                                 \n"
            //"uniform vec3 lightPos;                             \n"
            "                                                   \n"
            "void main(void)                                    \n"
            "{                                                  \n"
            //"   triColor = vec4(gl_FragCoord.x * 0.075),    \n"
            //"                sin(gl_FragCoord.y * 0.025),       \n"
            //"                sin(gl_FragCoord.x * 0.05),        \n"
            //"                1.0);                              \n"
            //"   triColor = vec4(1-gl_FragCoord.x, 1-gl_FragCoord.y, 1-gl_FragCoord.z, 1.0); \n"
            //"     float r = abs(Normals.x) * 0.7 + gl_FragCoord.x * 0.3;                            \n"
            //"     float g = abs(Normals.y) * 0.7 + gl_FragCoord.y * 0.3;                            \n"
            //"     float b = abs(Normals.z) * 0.7 + gl_FragCoord.z * 0.3;                            \n"
            //"     float r = abs(Normals.x) * 0.7 + 0.3;                            \n"
            //"     float g = abs(Normals.y) * 0.7 + 0.3;                            \n"
            //"     float b = abs(Normals.z) * 0.7 + 0.3;                            \n"
            "     float r = Colors.x;                            \n"
            "     float g = Colors.y;                            \n"
            "     float b = Colors.z;                            \n"
            "     triColor = vec4(r, g, 1-b, 1.0);                  \n"
            //"     triColor = vec4(Texs, 0.5, 1.0) * vec4(1-r, 1-g, 1-b, 1.0);                               \n"
            //"     triColor = texture(tex2d, Texs) * vec4(Colors, 1.0);      \n"
            //"triColor = color;                                  \n"
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

        //glEnable(GL_DEPTH_TEST);
        //glClearColor(1.0, 1.0, 1.0, 1.0);

        return program;
    }
};

//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
//        Axis = Zaxis;
//    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//        Axis = Xaxis;
//    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
//        Axis = Yaxis;
//} // end mouse_button_callback()

int main() {
    my_application triangle;
    loadTextures();
    int a = 1;
    int wire = 0;
    glm::mat4 myMatrix;
    glm::mat4 forward;
    glm::mat4 backward;
    glm::mat4 lookLeft;
    glm::mat4 lookRight;
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_NEVER);
    //glDepthRange(5, -5);

    //glm::ortho(0.0f, (float)WIN_LENG, 0.0f, float(WIN_HEIGHT), 0.1f, 100.0f);

   // cout << glGetDoublev(GL_DEPTH_RANGE) << "\n";

    backward = glm::mat4(0.997, 0, 0, 0,
        0, 0.997, 0, 0,
        0, 0, 0.997, 0,
        0, 0, 0, 1);

    forward = glm::mat4(1.003, 0, 0, 0,
        0, 1.003, 0, 0,
        0, 0, 1.003, 0,
        0, 0, 0, 1);

    //backward = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -0.01f));

    //forward = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.01f));

    /*lookLeft = glm::mat4(0.999993, 0, 0.0037417, 0,
        0, 1, 0, 0,
        -0.0037417, 0, 0.999993, 0,
        0, 0, 0, 1);

    lookRight = glm::mat4(0.999993, 0, -0.0037417, 0,
        0, 1, 0, 0,
        0.0037417, 0, 0.999993, 0,
        0, 0, 0, 1);*/

    lookLeft = glm::mat4(0.993, 0, 0.118, 0,
        0, 1, 0, 0,
        -0.118, 0, 0.993, 0,
        0, 0, 0, 1);

    lookRight = glm::mat4(0.993, 0, -0.118, 0,
        0, 1, 0, 0,
        0.118, 0, 0.993, 0,
        0, 0, 0, 1);



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
    GLFWwindow* terminal = glfwCreateWindow(WIN_LENG, WIN_HEIGHT, "Coates_HutchisonTriangle", nullptr, nullptr);

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

    //glfwSetMouseButtonCallback(terminal, mouse_button_callback);

    do {
        // draw nothing
        triangle.render(clock());
        //Swap buffers
        glfwSwapBuffers(terminal);
        glfwPollEvents();

        //rotateStuff(faces, myMatrix);

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

        else if (glfwGetKey(terminal, GLFW_KEY_UP) == GLFW_PRESS) {
            rotateStuff(faces, forward);
        }

        else if (glfwGetKey(terminal, GLFW_KEY_DOWN) == GLFW_PRESS) {
            rotateStuff(faces, backward);
        }

        else if (glfwGetKey(terminal, GLFW_KEY_LEFT) == GLFW_PRESS) {
            myMatrix = glm::translate(glm::mat4(), glm::vec3(-0.0008f, 0.0f, 0.0f));
            rotateStuff(faces, myMatrix);
        }

        else if (glfwGetKey(terminal, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            myMatrix = glm::translate(glm::mat4(), glm::vec3(0.0008f, 0.0f, 0.0f));
            rotateStuff(faces, myMatrix);
        }

        /*else if (glfwGetKey(terminal, GLFW_KEY_UP) == GLFW_PRESS) {
            myMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.005f, 0.0f));
            rotateStuff(faces, myMatrix);
        }*/

        else if (glfwGetKey(terminal, GLFW_KEY_L) == GLFW_PRESS) {
            //myMatrix = glm::translate(glm::mat4(), glm::vec3(-0.005f, 0.0f, 0.0f));
            //rotateStuff(faces, myMatrix * lookLeft);
            rotateStuff(faces, lookLeft);
        }

        else if (glfwGetKey(terminal, GLFW_KEY_R) == GLFW_PRESS) {
            //myMatrix = glm::translate(glm::mat4(), glm::vec3(0.005f, 0.0f, 0.0f));
            //rotateStuff(faces, myMatrix);
            rotateStuff(faces, lookRight);
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
