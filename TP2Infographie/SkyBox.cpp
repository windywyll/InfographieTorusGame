#include "SkyBox.h"
#include "DemoMain.h"

void SkyBox::init(char* texBottom, char* texTop,char* texLeft, char* texFront, char* texRight, char* texBack)
{
	//create the mesh (a square)
	GLuint faces[6] = {
		0,1,2,
		0,2,3};
	GLfloat vertice [12] = {
		-2.0f,-2.0f,0.0f,
		2.0f,-2.0f,0.0f,
		2.0f,2.0f,0.0f,
		-2.0f,2.0f,0.0f,
	};
	GLfloat texCoord [8] = {
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		0.0f,0.0f,
	};
	m_square = Mesh();
	m_square.init(4, vertice, nullptr, nullptr, texCoord, 6, faces);

	//load each texture of the sky box and store their IDs
	m_TexIDs[0] = TextureManager::getTexture(texBottom,false);
	m_TexIDs[1] = TextureManager::getTexture(texTop,false);
	m_TexIDs[2] = TextureManager::getTexture(texLeft,false);
	m_TexIDs[3] = TextureManager::getTexture(texFront,false);
	m_TexIDs[4] = TextureManager::getTexture(texRight,false);
	m_TexIDs[5] = TextureManager::getTexture(texBack,false);
	//get the shader ID
	m_shaderID = ShaderManager::getShader("texture.vert","texture.frag");

	glm::mat4 identity(1.0f);

	//set each modelview (only translation and zero or one rotation)
	m_models[0] = identity;//botom
	m_models[0] = glm::translate(m_models[0],glm::vec3(0.0f,-2.0f,0.0f));
	m_models[0] = glm::rotate(m_models[0],-90.0f,glm::vec3(1.0f,0.0f,0.0f));
	
	m_models[1] = identity;//top
	m_models[1] = glm::translate(m_models[1],glm::vec3(0.0f, 2.0f,0.0f));
	m_models[1] = glm::rotate(m_models[1],180.0f,glm::vec3(0.0f,1.0f,0.0f));
	m_models[1] = glm::rotate(m_models[1],90.0f,glm::vec3(1.0f,0.0f,0.0f));

	m_models[2] = identity;//left
	m_models[2] = glm::translate(m_models[2],glm::vec3(2.0f, 0.0f,0.0f));
	m_models[2] = glm::rotate(m_models[2],-90.0f,glm::vec3(0.0f,1.0f,0.0f));

	m_models[3] = identity;//front
	m_models[3] = glm::translate(m_models[3],glm::vec3(0.0f,0.0f,2.0f));
	m_models[3] = glm::rotate(m_models[3],180.0f,glm::vec3(0.0f,1.0f,0.0f));

	m_models[4] = identity;//right
	m_models[4] = glm::translate(m_models[4],glm::vec3(-2.0f, 0.0f,0.0f));
	m_models[4] = glm::rotate(m_models[4],90.0f,glm::vec3(0.0f,1.0f,0.0f));

	m_models[5] = identity;//back
	m_models[5] = glm::translate(m_models[5],glm::vec3(0.0f,0.0f,-2.0f));
}

void SkyBox::draw()
{
	tools::setUniformMatrix4fv(m_shaderID, "projection", glm::value_ptr(DemoMain::getInstance().getProjectionMatrix()));
	//get the rotation matrix only

	glm::mat4 view = glm::mat4(glm::mat3(DemoMain::getInstance().getViewMatrix()));

	glDepthMask(GL_FALSE); // make sure depth test is off
	ShaderManager::useShader(m_shaderID);
	for(int i = 0; i< 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_TexIDs[i]);
		tools::setUniformMatrix4fv(m_shaderID, "modelview", glm::value_ptr(view*m_models[i]));
		m_square.draw();
	}
	glDepthMask(GL_TRUE); // make sure depth test is on
}