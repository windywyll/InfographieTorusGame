#ifndef MESH
#define MESH

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <vector>

class Mesh{
private:
	GLuint m_ID;
	unsigned int m_numFaces;
	unsigned int m_numVertices;
	GLfloat *m_vertices;
	GLuint *m_faces;
	GLfloat *m_normals;
	GLfloat *m_textCoord;

public:
	Mesh();
	~Mesh();

	void init(GLuint numVerts, GLfloat* vertices, GLfloat* colours, GLfloat* normals, GLfloat* texcoords, GLuint indexCount, GLuint* indices);
	void init(GLuint numVerts, GLfloat* vertices, GLfloat* colours, GLfloat* normals,GLfloat* texcoords);
	void init(GLuint numVerts, GLfloat* vertices);
	void loadFromObjFile(const char* filename);
	void draw();

	GLuint getNumFaces(){return m_numFaces;}
	GLuint getNumVertices(){return m_numVertices;}
	GLfloat *getVertices(){return m_vertices;}
	GLuint *getFaces(){return m_faces;}
	GLfloat *getNormals(){return m_normals;}
	GLfloat *getTexCoords(){return m_textCoord;}
	GLuint getID(){return m_ID;}
};
#endif