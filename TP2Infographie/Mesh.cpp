#include "Mesh.h"
#include "ObjLoader.h"
#include <vector>
#include "tools.h"

using namespace std;

Mesh::Mesh()
{
	m_vertices = nullptr;
	m_faces = nullptr;
	m_numFaces = 0;
	m_numVertices = 0;
}

Mesh::~Mesh()
{}

void Mesh::init(GLuint numVerts, GLfloat* vertices, GLfloat* colours, GLfloat* normals, GLfloat* texcoords, GLuint indexCount, GLuint* indices)
{
	m_numVertices = numVerts;
	m_numFaces = indexCount/3;
	if(vertices != nullptr)
	{
		m_vertices = new GLfloat[numVerts*3];
		memcpy(m_vertices,vertices,numVerts*3*sizeof(GLfloat));
	}
	if(normals != nullptr)
	{
		m_normals = new GLfloat[numVerts*3];
		memcpy(m_normals,normals,indexCount*sizeof(GLfloat));
	}

	if(texcoords != nullptr)
	{
		m_textCoord = new GLfloat[numVerts*2];
		memcpy(m_textCoord,texcoords,indexCount*sizeof(GLfloat));
	}

	if(indices != nullptr)
	{
		m_faces = new GLuint[indexCount];
		memcpy(m_faces,indices,indexCount*sizeof(GLuint));
	}
	
	m_ID = tools::createMesh(numVerts, vertices, colours, normals, texcoords, indexCount, indices);
}

void Mesh::init(GLuint numVerts, GLfloat* vertices, GLfloat* colours, GLfloat* normals,GLfloat* texcoords)
{
	init(numVerts, vertices, colours, normals, texcoords, 0, nullptr);
}

void Mesh::init(GLuint numVerts, GLfloat* vertices)
{
	init(numVerts, vertices, nullptr, nullptr, nullptr, 0, nullptr);
}

void Mesh::draw()
{
	if(m_faces != NULL)
		tools::drawIndexedMesh(m_ID, m_numFaces*3, GL_TRIANGLES);
	else
		tools::drawMesh(m_ID, m_numVertices, GL_TRIANGLES); 
}

void Mesh::loadFromObjFile(const char* filename)
{
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	loadObj(filename, verts, norms, tex_coords, indices);
	init(verts.size()/3, verts.data(), nullptr, norms.data(), tex_coords.data(), indices.size(), indices.data());
}
