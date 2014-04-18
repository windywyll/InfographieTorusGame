#include "BaseMeshRenderer.h"
#include "DemoMain.h"

using namespace std;

BaseMeshRenderer::BaseMeshRenderer()
{
	m_textureID= -1;
}


BaseMeshRenderer::~BaseMeshRenderer()
{

}

void BaseMeshRenderer::draw()
{
	if(m_mesh != nullptr)
	{
		//use the shaderProgram
		ShaderManager::useShader(m_shaderProgram);

		GLuint uniformIndex;

		//iterate through the map of 4x4 matrices uniform
		map<string,glm::mat4>::iterator it4mf;
		for(it4mf = m_uniformMat4.begin(); it4mf != m_uniformMat4.end(); it4mf++)
		{
			tools::setUniformMatrix4fv(m_shaderProgram,it4mf->first.c_str(), glm::value_ptr(it4mf->second));
		}
	
		map<string,glm::vec4>::iterator it4f;
		for(it4f = m_uniformVec4.begin(); it4f != m_uniformVec4.end(); it4f++)
		{
			uniformIndex = glGetUniformLocation(m_shaderProgram, it4f->first.c_str());
			glUniform4fv(uniformIndex, 1, glm::value_ptr(it4f->second));
		}

		map<string,float>::iterator it1f;
		for(it1f = m_uniform1f.begin(); it1f != m_uniform1f.end(); it1f++)
		{
			uniformIndex = glGetUniformLocation(m_shaderProgram, it1f->first.c_str());
			glUniform1f(uniformIndex, it1f->second);
		}
		glm::mat4 modelview = DemoMain::getInstance().getViewMatrix() * m_model;
		tools::setUniformMatrix4fv(m_shaderProgram,"modelview", glm::value_ptr(modelview));
		tools::setUniformMatrix4fv(m_shaderProgram,"projection", glm::value_ptr(DemoMain::getInstance().getProjectionMatrix()));
		tools::setMaterial(m_shaderProgram, m_material);
		//bind the texture
		if(m_textureID != -1)
			glBindTexture(GL_TEXTURE_2D, m_textureID);
		//draw the mesh
		m_mesh->draw();
	}
}

Mesh *BaseMeshRenderer::getMesh()
{
	return m_mesh;
}

void BaseMeshRenderer::setMesh(Mesh *mesh)
{
	m_mesh = mesh;
}

void BaseMeshRenderer::calculateBoundingBox()
{
	float *vertices = m_mesh->getVertices();
	for(unsigned int i = 0; i < m_mesh->getNumVertices(); i ++)
	{
		glm::vec4 currentVertex = glm::vec4(1.0f);
		currentVertex.x = vertices[i*3];
		currentVertex.y = vertices[i*3+1];
		currentVertex.z = vertices[i*3+2];

		currentVertex = m_model * currentVertex;

		if(i == 0)
		{
			m_min.x = m_max.x = currentVertex.x;
			m_min.y = m_max.y = currentVertex.y;
			m_min.z = m_max.z = currentVertex.z;
		}
		else
		{
			if(currentVertex.x > m_max.x)
				m_max.x = currentVertex.x;
			if(currentVertex.y > m_max.y)
				m_max.y = currentVertex.y;
			if(currentVertex.z > m_max.z)
				m_max.z = currentVertex.z;
			if(currentVertex.x < m_min.x)
				m_min.x = currentVertex.x;
			if(currentVertex.y < m_min.y)
				m_min.y = currentVertex.y;
			if(currentVertex.z < m_min.z)
				m_min.z = currentVertex.z;
		}
	}
}

void BaseMeshRenderer::calculateModelMatrix()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model,m_position);
	m_model = glm::rotate(m_model,1.0f,m_rotation);
	m_model = glm::scale(m_model,m_scale);
	if(detectCollision)
		calculateBoundingBox();
}