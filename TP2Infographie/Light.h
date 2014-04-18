#ifndef LIGHT_H
#define LIGHT_H

#include "tools.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

class Light
{
public:
	Light(){}
	~Light(){}
	void initLight(tools::lightStruct light);
	void setUniform(GLuint shaderProgram);
	void setLightPosition(float *position);

	//parameters of the light stored in the struct
	float ambient[4];
private:
	GLuint lastShaderUsed;

	tools::lightStruct m_light;
};
#endif