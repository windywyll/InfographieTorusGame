#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <map>
#include <string>
#include <iostream>
#include <sstream>

#include <GL/glew.h>

#include "tools.h"

class ShaderManager
{
public:
	static void clearShaders();
	static GLuint getShader(char *vertName,char *fragName);
	static bool shaderExists(char *vertName,char *fragName);
	static bool shaderExists(GLuint shaderID);
	static void deleteShader(GLuint shaderID);
	static void useShader(GLuint shaderID);
private:

	static bool shaderExists(std::string concatenedName);

	static GLuint s_shaderInUse;
	static std::map<GLuint,std::string> s_existingsShaders;
};

#endif
