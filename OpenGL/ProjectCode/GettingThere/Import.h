#pragma once

#ifndef IMPORT_H
#define IMPORT_H

#endif IMPORT_H

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
#include <sstream>

// Include GLEW.  Always include it bofore gl.h, and glfw.h ... its a little magic
#include <glew\include\GL\glew.h>

// include GLFW
#include <glfw\include\GLFW\glfw3.h>

// Include GLM.
// We don't need it yet, but it is a library for 3D maths
#include <glm\glm\glm.hpp>
#include <glm\glm\gtx\transform.hpp>

// include GLU
#include <Windows.h>
#include <gl\gl.h>
// include GLU
#include <gl\glu.h>

//include assimp stuff
#include <assimp-3.3.1\include\assimp\Importer.hpp>
#include <assimp-3.3.1\include\assimp\scene.h>
#include <assimp-3.3.1\include\assimp\postprocess.h>


extern int numVerts;
extern int numNorms;
extern int numFaces;
extern int numLines;

extern float* verts; //just stores some verts as floats (not as faces)
extern int vertsSize;

extern glm::vec3* colors; //do color calculation based on object in obj file
extern int colorsSize;

extern glm::vec3* norms; //normals for some verts
extern int normsSize;

extern glm::vec3* normals; //normals for all verts
extern int normalsSize;

extern glm::vec4* faces; //from verts to faces
extern int facesSize1;

extern glm::vec3* faces2; //from verts to faces
extern int facesSize2;

glm::vec4* fillVerts(const char* filename, glm::vec4* faces);
void setColor(float offset, int l);
void rotateStuff(glm::vec4* myfaces, glm::mat4 myMatrix);
