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
                //cout << '\n' << x << y << z << '\n';
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
                //cout << '\n' << x << y << z << '\n';
            }

            //maybe a texture
            else if (car == 't') {
                //cout << car;
                file >> x >> y;
                textures[t].x = x;
                textures[t].y = y;
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


            finder = b.find("/");
            stringstream(b.substr(0, finder)) >> b1;
            finder2 = (b.substr(finder + 1, b.length())).find("/");
            stringstream(b.substr(finder + finder2, b.length())) >> b2;
            stringstream(b.substr(finder + 1, finder + finder2)) >> b3; //tex coord

            finder = c.find("/");
            stringstream(c.substr(0, finder)) >> c1;
            finder2 = (c.substr(finder + 1, c.length())).find("/");
            stringstream(c.substr(finder + finder2, c.length())) >> c2;
            stringstream(c.substr(finder + 1, finder + finder2)) >> c3; //tex coord


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
            textures[i].x = uvs[(b3 - 1)].x;
            textures[i].y = uvs[(b3 - 1)].y;
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
            textures[i].x = uvs[(c3 - 1)].x;
            textures[i].y = uvs[(c3 - 1)].y;
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
    while (getline(file, line)) {
        if (line[0] == 'o') {    //o for objects: color is based on object
            obj = line[2];
        }
        else if (line[0] == 'f') {    //o for objects
            switch (obj) {
                case 'B':
                    setColor(0.25, l);
                    break;
                case 'C':
                    setColor(0.15, l);
                    break;
                case 'D':
                    setColor(0.35, l);
                    break;
                case 'I':
                    setColor(0.45, l);
                    break;
                case 'L':
                case 'W':
                    setColor(0.55, l);
                    break;
                case 'M':
                    setColor(0.65, l);
                    break;
                case 'O':
                    setColor(0.75, l);
                    break;
                case 'P':
                    setColor(0.85, l);
                    break;
                /*case 'W':
                    setColor(0.95, l);*/
                    break;
                default:
                    setColor(0, l);
                    break;
                }
            l += 3;
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
    colors[i].x = offset + normals[l].x * (1 - offset);
    colors[i].y = offset + normals[l].y * (1 - offset);
    colors[i].z = offset + normals[l].z * (1 - offset);
    colors[i + 1].x = offset + normals[l].x * (1 - offset);
    colors[i + 1].y = offset + normals[l].y * (1 - offset);
    colors[i + 1].z = offset + normals[l].z * (1 - offset);
    colors[i + 2].x = offset + normals[l].x * (1 - offset);
    colors[i + 2].y = offset + normals[l].y * (1 - offset);
    colors[i + 2].z = offset + normals[l].z * (1 - offset);
    //cout << "i is " << i << "\n";
}

void rotateStuff(glm::vec4* myfaces, glm::mat4 myMatrix) {
    glm::vec4* faces1 = new glm::vec4[numFaces * 9]();

    int i;
    //for (i = 0; i < numFaces * 3; i++)
    //    faces1[i] = myMatrix * myfaces[i];
    for (i = 0; i < numFaces * 3; i++)
        myfaces[i] = myMatrix * myfaces[i];
}


void loadTextures() {
    /*GLuint SpookyWalls1 =    //skulls and locks
        SOIL_load_OGL_texture("\blender\WallArt\WallArt4BMP.bmp",
            SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);*/

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
        //GL_UNSIGNED_BYTE, image);

    GLuint tex2d;
    glGenTextures(1, &tex2d);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex2d);

    int width, height;
    unsigned char* SpookyWalls1 = SOIL_load_image("WallArt4BMP.bmp", &width, &height, 0, SOIL_LOAD_RGB);

    if (SpookyWalls1 == NULL) {
        cout << "Error with tex file\n";

    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, SpookyWalls1);
    
    //float pixels[] = {
    //    0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    //    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    //};
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);

    /*glBindTexture(GL_TEXTURE_2D, SpookyWalls1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);*/

    //SOIL_free_image_data(SpookyWalls1);

    //SOIL_free_image_data(SpookyWalls1);

    return;
}
