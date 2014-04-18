#include "AbstractMeshRenderer.h"

using namespace std;

AbstractMeshRenderer::AbstractMeshRenderer()
{
	m_shaderProgram = 0;

	m_position = glm::vec3(0.0f);
	m_scale = glm::vec3(1.0f);
	m_rotation = glm::vec3(0.0f);

	detectCollision = true;
	m_generatedModelMatrix = true;
}

AbstractMeshRenderer::~AbstractMeshRenderer()
{
	clearUniforms();
}

//init the shader of the meshRenderer
void AbstractMeshRenderer::initShaders(char *vertName,char *fragName)
{
	m_shaderProgram = ShaderManager::getShader(vertName,fragName);
}

void AbstractMeshRenderer::setShader(GLuint shaderProgram)
{
	if(ShaderManager::shaderExists(shaderProgram))
		m_shaderProgram = shaderProgram;
}

GLuint AbstractMeshRenderer::getShader()
{
	return m_shaderProgram;
}

void AbstractMeshRenderer::setUniformMat4(string name, glm::mat4 value)
{
	if(m_uniformMat4.find(name) != m_uniformMat4.end())
	{
		m_uniformMat4.erase(name);
	}
	m_uniformMat4.insert(map<string,glm::mat4>::value_type(name,value));
}

void AbstractMeshRenderer::setUniformVec4(std::string name, glm::vec4 value)
{
	
	if(m_uniformVec4.find(name) != m_uniformVec4.end())
	{
		m_uniformVec4.erase(name);
	}
	m_uniformVec4.insert(map<string,glm::vec4>::value_type(name,value));
}

void AbstractMeshRenderer::setUniform1f(std::string name, float value)
{
	if(m_uniform1f.find(name) != m_uniform1f.end())
	{
		m_uniform1f.erase(name);
	}
	m_uniform1f.insert(map<string,float>::value_type(name,value));
}

void AbstractMeshRenderer::setTexture(char *fileName)
{
	m_textureID= TextureManager::getTexture(fileName);
}

bool AbstractMeshRenderer::uniformExists(string name)
{
	return m_uniformMat4.find(name) != m_uniformMat4.end() || m_uniformVec4.find(name) != m_uniformVec4.end();
}

glm::mat4 AbstractMeshRenderer::getUniformMat4(string name)
{
	if(m_uniformMat4.find(name) != m_uniformMat4.end())
		return m_uniformMat4[name];
	return glm::mat4();
}

glm::vec4 AbstractMeshRenderer::getUniformVec4(std::string name)
{
	if(m_uniformVec4.find(name) != m_uniformVec4.end())
		return m_uniformVec4[name];
	return glm::vec4();
}

float AbstractMeshRenderer::getUniform1f(std::string name)
{
	if(m_uniform1f.find(name) != m_uniform1f.end())
		return m_uniform1f[name];
	return 0.0f;
}

unsigned int AbstractMeshRenderer::getTextureID()
{
	return m_textureID;
}


void AbstractMeshRenderer::deleteUniform(string name)
{
	if(m_uniformMat4.find(name) != m_uniformMat4.end())
	{
		m_uniformMat4.erase(name);
	}
	if(m_uniformVec4.find(name) != m_uniformVec4.end())
	{
		m_uniformVec4.erase(name);
	}
	if(m_uniform1f.find(name) != m_uniform1f.end())
	{
		m_uniform1f.erase(name);
	}
}

void AbstractMeshRenderer::clearUniforms()
{
	m_uniformMat4.clear();
}

bool AbstractMeshRenderer::isCollision(AbstractMeshRenderer &otherMesh)
{
	if(detectCollision && otherMesh.detectCollision)
	{
		//we only test for collision of the side
		if(m_min.x < otherMesh.m_max.x && m_min.x > otherMesh.m_min.x 
			&& m_min.z < otherMesh.m_max.z && m_min.z > otherMesh.m_min.z
		|| m_max.x < otherMesh.m_max.x && m_max.x > otherMesh.m_min.x
			&& m_max.z < otherMesh.m_max.z && m_max.z > otherMesh.m_min.z
		||otherMesh.m_min.x < m_max.x && otherMesh.m_min.x > m_min.x 
			&& otherMesh.m_min.z < m_max.z && otherMesh.m_min.z > m_min.z
		||otherMesh.m_max.x < m_max.x && otherMesh.m_max.x > m_min.x 
			&& otherMesh.m_max.z < m_max.z && otherMesh.m_max.z > m_min.z
		||m_min.x < otherMesh.m_max.x && m_min.x > otherMesh.m_min.x 
			&& m_max.z < otherMesh.m_max.z && m_max.z > otherMesh.m_min.z
		|| m_max.x < otherMesh.m_max.x && m_max.x > otherMesh.m_min.x
			&& m_min.z < otherMesh.m_min.z && m_min.z > otherMesh.m_min.z
		||otherMesh.m_min.x < m_max.x && otherMesh.m_min.x > m_min.x 
			&& otherMesh.m_max.z < m_max.z && otherMesh.m_max.z > m_min.z
		||otherMesh.m_max.x < m_max.x && otherMesh.m_max.x > m_min.x 
			&& otherMesh.m_min.z < m_max.z && otherMesh.m_min.z > m_min.z)
		return true;
	return false;
	}
	return false;
}