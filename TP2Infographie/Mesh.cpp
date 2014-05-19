#include "Mesh.h"
#include "ObjLoader.h"
#include "tools.h"

#include <vector>
#include <math.h>

using namespace std;

Mesh::Mesh()
{
	m_vertices = nullptr;
	m_faces = nullptr;
	m_normals = nullptr;
	m_textCoord = nullptr;
	m_numFaces = 0;
	m_numVertices = 0;
	m_ID = 0;
}

Mesh::~Mesh()
{
if ( m_vertices != nullptr)
	delete m_vertices;
if ( m_normals != nullptr)
	delete m_normals;
if ( m_textCoord != nullptr)
	delete m_textCoord;
if ( m_faces != nullptr)
	delete m_faces;
}

void Mesh::init(GLuint numVerts, GLfloat* vertices, GLfloat* colours, GLfloat* normals, GLfloat* texcoords, GLuint indexCount, GLuint* indices, bool dynamic)
{
	m_numVertices = numVerts;
	m_numFaces = indexCount/3;
	if(vertices != nullptr)
	{
		if (m_vertices != nullptr)
			delete m_vertices;
		m_vertices = new GLfloat[numVerts*3];
		memcpy(m_vertices,vertices,numVerts*3*sizeof(GLfloat));
	}
	if(normals != nullptr)
	{
		if (m_normals != nullptr)
			delete m_normals;
		m_normals = new GLfloat[numVerts*3];
		memcpy(m_normals,normals,numVerts*3*sizeof(GLfloat));
	}

	if(texcoords != nullptr)
	{
		if (m_textCoord != nullptr)
			delete m_textCoord;
		m_textCoord = new GLfloat[numVerts*2];
		memcpy(m_textCoord,texcoords,numVerts*2*sizeof(GLfloat));
	}

	if(indices != nullptr)
	{
		if (m_faces != nullptr)
			delete m_faces;
		m_faces = new GLuint[indexCount];
		memcpy(m_faces,indices,indexCount*sizeof(GLuint));
	}
	if(m_ID == 0)
		m_ID = tools::createMesh(numVerts, vertices, colours, normals, texcoords, indexCount, indices, dynamic);
	else
	{
		if(vertices != nullptr)
			tools::updateMesh(m_ID,VERTEX,vertices,numVerts);
		if(colours != nullptr)
			tools::updateMesh(m_ID,COLOUR,colours,numVerts);
		if(normals != nullptr)
			tools::updateMesh(m_ID,NORMAL,normals,numVerts);
		if(texcoords != nullptr)
			tools::updateMesh(m_ID,TEXCOORD,texcoords,numVerts);
	}
}

void Mesh::init(GLuint numVerts, GLfloat* vertices, GLfloat* colours, GLfloat* normals,GLfloat* texcoords, bool dynamic)
{
	init(numVerts, vertices, colours, normals, texcoords, 0, nullptr,dynamic);
}

void Mesh::init(GLuint numVerts, GLfloat* vertices, bool dynamic)
{
	init(numVerts, vertices, nullptr, nullptr, nullptr, 0, nullptr, dynamic);
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
	init(verts.size()/3, verts.data(), nullptr, norms.data(), tex_coords.data(), indices.size(), indices.data(), false);
}

void Mesh::createTorus(GLfloat maxRadius, GLfloat minRadius, GLuint nbPointsMax, GLuint nbPointsMin)
{
	if(nbPointsMax > 3 && nbPointsMin > 3)
	{
		std::vector<GLfloat> vertices, normals;
		std::vector<GLuint> faces;
		for(GLuint i = 0; i < nbPointsMax; i ++)
		{
			double u = 2.0 * M_PI * i / nbPointsMax;
			for(GLuint j = 0; j < nbPointsMin; j++)
			{
				double v = 2.0 * M_PI * j / nbPointsMin;
				vertices.insert(vertices.end(),(maxRadius + minRadius*cos(v)) * cos(u));
				vertices.insert(vertices.end(),(maxRadius + minRadius*cos(v)) * sin(u));
				vertices.insert(vertices.end(),minRadius * sin(v));

				normals.insert(normals.end(),(minRadius*cos(v)) * cos(u));
				normals.insert(normals.end(),(minRadius*cos(v)) * sin(u));
				normals.insert(normals.end(),minRadius * sin(v));

				if( i < nbPointsMax -1 && j < nbPointsMin -1)
				{
					faces.insert(faces.end(),j+i*nbPointsMin);
					faces.insert(faces.end(),j+1+i*nbPointsMin);
					faces.insert(faces.end(),j+(i+1)*nbPointsMin);
					faces.insert(faces.end(),j+(i+1)*nbPointsMin);
					faces.insert(faces.end(),j+1+i*nbPointsMin);
					faces.insert(faces.end(),j+1+(i+1)*nbPointsMin);
				}
				else if (i == nbPointsMax -1 && j < nbPointsMin -1 )
				{
					faces.insert(faces.end(),j+i*nbPointsMin);
					faces.insert(faces.end(),j+1+i*nbPointsMin);
					faces.insert(faces.end(),j+0);
					faces.insert(faces.end(),j+0);
					faces.insert(faces.end(),j+1+i*nbPointsMin);
					faces.insert(faces.end(),j+1+0);
				}
				else if (i < nbPointsMax -1 && j == nbPointsMin -1 )
				{
					faces.insert(faces.end(),j+i*nbPointsMin);
					faces.insert(faces.end(),0+i*nbPointsMin);
					faces.insert(faces.end(),j+(i+1)*nbPointsMin);
					faces.insert(faces.end(),j+(i+1)*nbPointsMin);
					faces.insert(faces.end(),0+i*nbPointsMin);
					faces.insert(faces.end(),0+(i+1)*nbPointsMin);
				}
				else if (i == nbPointsMax -1 && j == nbPointsMin -1 )
				{
					faces.insert(faces.end(),j+i*nbPointsMin);
					faces.insert(faces.end(),0+i*nbPointsMin);
					faces.insert(faces.end(),j+0);
					faces.insert(faces.end(),j+0);
					faces.insert(faces.end(),0+i*nbPointsMin);
					faces.insert(faces.end(),0+0);
				}
			}
		}
		//std::cout << "nb faces : " << faces.size()/3 << " nb vertices : " << vertices.size()/3 << std::endl;
		init(vertices.size()/3, vertices.data(), nullptr, normals.data(), nullptr, faces.size(), faces.data(), true);
	}
}
