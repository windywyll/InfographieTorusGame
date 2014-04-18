// rt3dObjLoader.h
// A simple OBJ loader
// (c) Daniel Livingstone 2013
// Please don't share - this was hacked together quite quickly and is for demonstration use only
//
// Limitations:
// Only works with triangulated OBJ models
// Does not support groups or multiple meshes per file
// Does not support anything other than very straightforward OBJ models
// Will not generate normals if the model is missing them - or any other missing data
#ifndef OBJ_LOADER
#define OBJ_LOADER

#include <GL/glew.h>
#include <vector>

#include "Mesh.h"

void loadObj(const char* filename, std::vector<GLfloat> &verts, std::vector<GLfloat> &norms, 
	std::vector<GLfloat> &texcoords, std::vector<GLuint> &indices);

void saveMesh(const char* filename,Mesh & mesh);

#endif