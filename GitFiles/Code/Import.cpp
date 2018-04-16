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

    //check file is okay
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

    //to store vertex, normal, or texture data (all floats) from file lines
    float x, y, z;

    //store face data in obj file as strings (3 per line)
    string a, b, c;

    //for splitting up faces data into verts and corresponding normals and tex coords
    int a1, a2, a3, b1, b2, b3, c1, c2, c3;

    //first runthrough of file: get needed sizes for arrays and total number of lines in file
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
    //each vertex has a normal and a color and a tex coord
    //sizes based on calculated stuff above
    verts = new float[numVerts * 3]();
    norms = new glm::vec3[numNorms]();  //storing norms (not normals)
    normals = new glm::vec3[numFaces * 9](); //normals now
    uvs = new glm::vec2[numUVs](); //storing uvs (not tex coords)
    textures = new glm::vec2[numFaces * 6](); //tex coords
    colors = new glm::vec3[numFaces * 9]();
    faces = new glm::vec4[numFaces * 9]();

    //reset file pointer cause we're running through it again to grab data
    file.clear();
    file.seekg(0);

    i = 0;
    j = 0;
    //used to skip over '/'s in face information
    int finder = 0;
    int finder2 = 0;
    //this will be used later to determine what part of the model this object is (and how it should be colored/textured)
    char obj = '\0';

    while (getline(file, line)) {
        //fill vert, normal, face, and texture data
        file >> car;
        //v could be v, vt, or vn....
        if (car == 'v') {\
            //just a vert
            if (file.read(&car, 1) && car == ' ') {
                file >> x >> y >> z;
                verts[j] = x;
                verts[j + 1] = y;
                verts[j + 2] = z;
                j += 3;
            }

            //now a normal
            else if (car == 'n') {
                file >> x >> y >> z;
                norms[k].x = x;
                norms[k].y = y;
                norms[k].z = z;
                k++;
            }

            //maybe a texture
            else if (car == 't') {
                file >> x >> y;
                uvs[t].x = x;
                uvs[t].y = y;
                t++;
            }

        }

        //f for faces
        else if (car == 'f') {
            file >> a >> b >> c;

            finder = a.find("/");
            stringstream(a.substr(0, finder)) >> a1;
            finder2 = (a.substr(finder + 1, a.length())).find("/");     //first vert index
            stringstream(a.substr(finder + finder2, a.length())) >> a2; //normal index
            stringstream(a.substr(finder + 1, finder + finder2)) >> a3; //tex coord index

            finder = b.find("/");
            stringstream(b.substr(0, finder)) >> b1;
            finder2 = (b.substr(finder + 1, b.length())).find("/");     //second vert index
            stringstream(b.substr(finder + finder2, b.length())) >> b2; //normal index
            stringstream(b.substr(finder + 1, finder + finder2)) >> b3; //tex coord index

            finder = c.find("/");
            stringstream(c.substr(0, finder)) >> c1;
            finder2 = (c.substr(finder + 1, c.length())).find("/");     //thrid vert index
            stringstream(c.substr(finder + finder2, c.length())) >> c2; //normal index
            stringstream(c.substr(finder + 1, finder + finder2)) >> c3; //tex coord index

            //now we can actually fill our final arrays (the ones for the vbos) with some data!!
            //this is all for one single face...
            //1st vert + normal + texture
            faces[i].x = verts[(a1 - 1) * 3];
            faces[i].y = verts[(a1 - 1) * 3 + 1];
            faces[i].z = verts[(a1 - 1) * 3 + 2];
            faces[i].w = 1;
            normals[i].x = norms[(a2 - 1)].x;
            normals[i].y = norms[(a2 - 1)].y;
            normals[i].z = norms[(a2 - 1)].z;
            textures[i].x = uvs[(a3 - 1)].x;
            textures[i].y = uvs[(a3 - 1)].y;
            //this distorts the model...just sized it in blender instead
            //faces[i] = normalize(faces[i]);

            //2nd vert + normal + texture
            faces[i + 1].x = verts[(b1 - 1) * 3];
            faces[i + 1].y = verts[(b1 - 1) * 3 + 1];
            faces[i + 1].z = verts[(b1 - 1) * 3 + 2];
            faces[i + 1].w = 1;
            normals[i + 1].x = norms[(b2 - 1)].x;
            normals[i + 1].y = norms[(b2 - 1)].y;
            normals[i + 1].z = norms[(b2 - 1)].z;
            textures[i + 1].x = uvs[(b3 - 1)].x;
            textures[i + 1].y = uvs[(b3 - 1)].y;
            //this distorts the model...just sized it in blender instead
            //faces[i + 2] = normalize(faces[i + 2]);

            //3rd vert + normal + texture
            faces[i + 2].x = verts[(c1 - 1) * 3];
            faces[i + 2].y = verts[(c1 - 1) * 3 + 1];
            faces[i + 2].z = verts[(c1 - 1) * 3 + 2];
            faces[i + 2].w = 1;
            normals[i + 2].x = norms[(c2 - 1)].x;
            normals[i + 2].y = norms[(c2 - 1)].y;
            normals[i + 2].z = norms[(c2 - 1)].z;
            textures[i + 2].x = uvs[(c3 - 1)].x;
            textures[i + 2].y = uvs[(c3 - 1)].y;
            //this distorts the model...just sized it in blender instead
            //faces[i + 3] = normalize(faces[i + 2]);


            //this will help set "color" (actually texture) data
            //checks obj (set when we see an object line) and colors faces accordingly
            //numbers are relatively random but work with frag shader switch to texture/color separate pieces
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
                case 'A':
                    setColor(15, l);
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
                case 'G':
                    setColor(5, l);
                    break;
                case 'I':
                    setColor(8, l);
                    break;
                case 'L':
                case 'W':
                    setColor(10, l);
                    break;
                case 'M':
                    setColor(12, l);
                    break;
                case 'P':
                    setColor(16, l);
                    break;
                case 'O':
                case 'T':
                    setColor(23, l);
                    break;
                case 'R':
                    setColor(24, l);
                    break;
                case 'S':
                    setColor(25, l);
                    break;
                default:
                    setColor(0, l);
                    break;
            }
            i += 3;
            l += 3;
        }

        //o for objects: color/texture is based on object
        else if (car == 'o') {
            file >> a;
            obj = a[0];
        }

    }

    //now to calcuate sizes...
    //four bytes per float; 3 floats per vert, 3 verts per face etc.
    vertsSize = numVerts * 4 * 3;
    normsSize = numNorms * 4 * 9;
    colorsSize = numFaces * 4 * 9;
    texturesSize = numFaces * 4 * 6;
    file.close();

    //return filled faces array
    return faces;
}

//set color for that face (l) based on input number (offset)
void setColor(float offset, int l) {
    int i = l;
    //a different color calculation (based on normals of faces)
    /*colors[i].x = offset + normals[l].x * (1 - offset);
    colors[i].y = offset + normals[l].y * (1 - offset);
    colors[i].z = offset + normals[l].z * (1 - offset);
    colors[i + 1].x = offset + normals[l].x * (1 - offset);
    colors[i + 1].y = offset + normals[l].y * (1 - offset);
    colors[i + 1].z = offset + normals[l].z * (1 - offset);
    colors[i + 2].x = offset + normals[l].x * (1 - offset);
    colors[i + 2].y = offset + normals[l].y * (1 - offset);
    colors[i + 2].z = offset + normals[l].z * (1 - offset);*/
    colors[i].x = offset;
    colors[i].y = offset;
    colors[i].z = offset;
    colors[i + 1].x = offset;
    colors[i + 1].y = offset;
    colors[i + 1].z = offset;
    colors[i + 2].x = offset;
    colors[i + 2].y = offset;
    colors[i + 2].z = offset;
}

//rotation function for user input stuff
//just need a matrix and face data
void rotateStuff(glm::vec4* myfaces, glm::mat4 myMatrix) {

    int i;
    for (i = 0; i < numFaces * 9; i++)
        myfaces[i] = myMatrix * myfaces[i];
}


//textures!
//uses helper function soilMapping (below)
//variable names corresponding to uniform sampler 2Ds in frag shader
//lots of textures...10 thus far (room for 15)
void loadTextures(GLuint* myTextures, GLuint shader) {
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

    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "BigRedBMP.bmp", "bigred");

    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, myTextures[i]);
    i = soilMapping(myTextures, shader, i, "FramesBMP.bmp", "frames");

    return;
}

//real workhorse of texture loading
//works with an array easily enough: i (array index) is incremented and returned each time
//uses soil; all textures are on repeat and linear filtering
//mipmaps are generated for all as well
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
