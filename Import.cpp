#include "Import.h"

//this function is my own model loader :D
glm::vec4* fillVerts(const char* filename, glm::vec4* faces) {
    numVerts = 0;
    numNorms = 0;
    numFaces = 0;
    numLines = 0;
    numUVs = 0;
    fstream file;
    file.open(filename);

    if (!file.is_open()) {
        std::cerr << "Uh oh! There's a problem with this file!\n";
        return NULL;
    }

    string line;
    char car;
    int i = 0;      //for faces
    int j = 0;      //for verts
    int k = 0;      //for normals
    int l = 0;      //for colors
    int t = 0;      //for textures/UVs
    float x, y, z;
    string a, b, c;
    int a1, a2, a3, b1, b2, b3, c1, c2, c3;
    //ASSERT(file != NULL);
     while (getline(file, line)) {
        if (line[0] == 'v' && line[1] == ' ')
            numVerts++;

        else if (line[0] == 'v' && line[1] == 'n')
            numNorms++;

        else if (line[0] == 'v' && line[1] == 't')
            numUVs++;

        else if (line[0] == 'f')
            numFaces++;

        numLines++;
    }

    //each triangulated face has 9 floats in it (3 verts)
    //each vertex has a normal and a color
    verts = new float[numVerts * 3]();
    texmatData = new int[numFaces]();
    norms = new glm::vec3[numNorms]();  //storing norms (not normals)
    normals = new glm::vec3[numFaces * 9](); //normals later
    uvs = new glm::vec2[numUVs]();  //like norms for textures
    textures = new glm::vec2[numFaces * 6]();     //textures
    colors = new glm::vec3[numFaces * 9]();
    faces = new glm::vec4[numFaces * 9]();

    file.clear();
    file.seekg(0);

    i = 0;
    j = 0;
    int finder = 0;
    int finder2 = 0;

    while (getline(file, line)) {
        file >> car;
        if (car == 'v') {\
            //just a vert
            if (file.read(&car, 1) && car == ' ') {
                file >> x >> y >> z;
                verts[j] = x;
                verts[j + 1] = y;
                verts[j + 2] = z;
                //cout << '\n' << "vert" << x << y << z << '\n';
                j += 3;
            }

            //now a normal
            else if (car == 'n') {
                //cout << car;
                file >> x >> y >> z;
                norms[k].x = x;
                norms[k].y = y;
                norms[k].z = z;
                k++;
                //printf("\nfound a vert normal!\n");
                //cout << '\n' << "vert norm" << x << y << z << '\n';
            }

            //maybe a texture
            else if (car == 't') {
                //cout << car;
                file >> x >> y;
                uvs[t].x = x;
                uvs[t].y = y;
                //cout << '\n' << "vert uv" << x << y << '\n';
                t++;
            }

        }


        else if (car == 'f') {    //f for faces
                                  //cout << line << car << '\n';
            file >> a >> b >> c;

            finder = a.find("/");
            stringstream(a.substr(0, finder)) >> a1;
            finder2 = (a.substr(finder + 1, a.length())).find("/");
            stringstream(a.substr(finder + finder2, a.length())) >> a2;
            stringstream(a.substr(finder + 1, finder + finder2)) >> a3; //tex coord
            //cout << "\n tex coord" << a1;
           // cout << "\n tex coord" << a2;
            //cout << "\n tex coord" << a3;

            finder = b.find("/");
            stringstream(b.substr(0, finder)) >> b1;
            finder2 = (b.substr(finder + 1, b.length())).find("/");
            stringstream(b.substr(finder + finder2, b.length())) >> b2;
            stringstream(b.substr(finder + 1, finder + finder2)) >> b3; //tex coord
            //cout << "\n tex coord" << b3;


            finder = c.find("/");
            stringstream(c.substr(0, finder)) >> c1;
            finder2 = (c.substr(finder + 1, c.length())).find("/");
            stringstream(c.substr(finder + finder2, c.length())) >> c2;
            stringstream(c.substr(finder + 1, finder + finder2)) >> c3; //tex coord
            //cout << "\n tex coord" << c3;


            //cout << "\n" << a3 << " " << b3 << " " << c3 << "\n";

            //1st vert + normal
            faces[i].x = verts[(a1 - 1) * 3];
            faces[i].y = verts[(a1 - 1) * 3 + 1];
            faces[i].z = verts[(a1 - 1) * 3 + 2];
            faces[i].w = 1;
            normals[i].x = norms[(a2 - 1)].x;
            normals[i].y = norms[(a2 - 1)].y;
            normals[i].z = norms[(a2 - 1)].z;
            textures[i].x = uvs[(a3 - 1)].x;
            textures[i].y = uvs[(a3 - 1)].y;
            //cout << '\n' << i << '\n' << normals[i].x << '\n'
            //    << normals[i].y << '\n'
            //    << normals[i].z << '\n';
            //faces[i] = normalize(faces[i]);

            //2nd vert
            faces[i + 1].x = verts[(b1 - 1) * 3];
            faces[i + 1].y = verts[(b1 - 1) * 3 + 1];
            faces[i + 1].z = verts[(b1 - 1) * 3 + 2];
            faces[i + 1].w = 1;
            normals[i + 1].x = norms[(b2 - 1)].x;
            normals[i + 1].y = norms[(b2 - 1)].y;
            normals[i + 1].z = norms[(b2 - 1)].z;
            textures[i + 1].x = uvs[(b3 - 1)].x;
            textures[i + 1].y = uvs[(b3 - 1)].y;
            //cout << '\n' << i + 1 << '\n' << normals[i + 1].x << '\n'
            //    << normals[i + 1].y << '\n'
            //    << normals[i + 1].z << '\n';
            //faces[i + 1] = normalize(faces[i + 1]);

            //3rd vert
            faces[i + 2].x = verts[(c1 - 1) * 3];
            faces[i + 2].y = verts[(c1 - 1) * 3 + 1];
            faces[i + 2].z = verts[(c1 - 1) * 3 + 2];
            faces[i + 2].w = 1;
            normals[i + 2].x = norms[(c2 - 1)].x;
            normals[i + 2].y = norms[(c2 - 1)].y;
            normals[i + 2].z = norms[(c2 - 1)].z;
            textures[i + 2].x = uvs[(c3 - 1)].x;
            textures[i + 2].y = uvs[(c3 - 1)].y;
            //cout << '\n' << i + 2 << '\n' << normals[i + 2].x << '\n'
            //    << normals[i + 2].y << '\n'
            //    << normals[i + 2].z << '\n';
            //faces[i + 2] = normalize(faces[i + 2]);
            i += 3;

            //printf("found a face!\n");
        }

    }

    file.clear();
    file.seekg(0);

    //and now for some colors!
    char obj = '\0';
    i = 0;
    while (getline(file, line)) {
        if (line[0] == 'o') {    //o for objects: color is based on object
            obj = line[2];
            //texmatData[i] = obj - 'A';
        }
        else if (line[0] == 'f') {    //o for objects
            //texmatData[i] = obj;
            //cout << texmatData[i] << '\n';
            switch (obj) {
                case '1':
                    setColor(51, l);
                    break;
                case '2':
                    setColor(52, l);
                    break;
                case '3':
                    setColor(53, l);
                    break;
                case '4':
                    setColor(54, l);
                    break;
                case 'B':
                    setColor(1, l);
                    break;
                case 'C':
                    setColor(2, l);
                    break;
                case 'D':
                    setColor(3, l);
                    break;
                case 'F':
                    setColor(4, l);
                    break;
                case 'I':
                    setColor(5, l);
                    break;
                case 'L':
                case 'W':
                    setColor(10, l);
                    break;
                case 'M':
                    setColor(12, l);
                    break;
                case 'O':
                    setColor(15, l);
                    break;
                case 'P':
                    setColor(16, l);
                    break;
                case 'S':
                    setColor(25, l);
                    break;
                default:
                    setColor(0, l);
                    break;
                }
            l += 3;
            i++;
        }
        //cout << colorCount << "\n";
    }

    //four bytes per float!
    vertsSize = numVerts * 4 * 3;
    normsSize = numNorms * 4 * 9;
    colorsSize = numFaces * 4 * 9;
    texturesSize = numFaces * 4 * 6;
    //cout << numFaces << "\n";
    //facesSize = numFaces * 4 * 9;
    file.close();

    return faces;
}

void setColor(float offset, int l) {
    //cout << "l is "<< l << "\n";
    int i = l;
    //colors[i].x = offset + normals[l].x * (1 - offset);
    //colors[i].y = offset + normals[l].y * (1 - offset);
    //colors[i].z = offset + normals[l].z * (1 - offset);
    //colors[i + 1].x = offset + normals[l].x * (1 - offset);
    //colors[i + 1].y = offset + normals[l].y * (1 - offset);
    //colors[i + 1].z = offset + normals[l].z * (1 - offset);
    //colors[i + 2].x = offset + normals[l].x * (1 - offset);
    //colors[i + 2].y = offset + normals[l].y * (1 - offset);
    //colors[i + 2].z = offset + normals[l].z * (1 - offset);
    colors[i].x = offset;
    colors[i].y = offset;
    colors[i].z = offset;
    colors[i + 1].x = offset;
    colors[i + 1].y = offset;
    colors[i + 1].z = offset;
    colors[i + 2].x = offset;
    colors[i + 2].y = offset;
    colors[i + 2].z = offset;
    //cout << "i is " << i << "\n";
}

void rotateStuff(glm::vec4* myfaces, glm::mat4 myMatrix) {

    int i;
    for (i = 0; i < numFaces * 9; i++)
        myfaces[i] = myMatrix * myfaces[i];
}


void loadTextures(GLuint* myTextures, GLuint shader) {
    //textures!
    int i = 0;
    glGenTextures(15, myTextures);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt4BMP.bmp", "fbwall");

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt13BMP.bmp", "ceiling");

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt6BMP.bmp", "base");

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt7BMP.bmp", "lrwall");

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt12BMP.bmp", "floor");

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt1BMP.bmp", "doorway");

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt8BMP.bmp", "panel1");

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt3BMP.bmp", "panel4");

    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "WallArt9BMP.bmp", "panel3");

    return;
}


int soilMapping(GLuint* myTextures, GLuint shader, int i, const char* file, const char* name) {
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, name), i);

    int width, height;
    unsigned char* SpookyWalls1 = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGB);

    if (SpookyWalls1 == NULL) {
        cout << "Error with tex file\n";

    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, SpookyWalls1);
    glUniform1i(glGetUniformLocation(shader, name), i);
    SOIL_free_image_data(SpookyWalls1);
    i++;
    return i;
}
