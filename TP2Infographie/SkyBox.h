#ifndef SKY_BOX_H
#define SKY_BOX_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "TextureManager.h"
#include "ShaderManager.h"

class SkyBox
{
public:
	SkyBox(){}
	void init(char* texBottom, char* texTop,char* texLeft, char* texFront, char* texRight, char* texBack);
	void draw();
private:
	Mesh m_square;
	GLuint m_TexIDs[6];
	GLuint m_shaderID;
	glm::mat4 m_models[6];
};

#endif