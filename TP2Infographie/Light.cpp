#include "Light.h"
#include "DemoMain.h"

void Light::initLight(tools::lightStruct light)
{
	m_light = light;
}

void Light::setUniform(GLuint shaderProgram)
{
	ShaderManager::useShader(shaderProgram);
	if(lastShaderUsed!= shaderProgram)
	{
		lastShaderUsed = shaderProgram;
		tools::setLight(shaderProgram, m_light);
	}
	glm::vec4 tmpvec(m_light.position[0],m_light.position[1],m_light.position[2],1.0f);
	tmpvec = DemoMain::getInstance().getViewMatrix() * tmpvec;
	tools::setLightPos(shaderProgram, glm::value_ptr(tmpvec));
}

void Light::setLightPosition(float *position)
{
	for(int i = 0; i< 3; i++)
	{
		m_light.position[i] = position[i];
	}
}
