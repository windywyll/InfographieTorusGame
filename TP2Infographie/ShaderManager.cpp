#include "ShaderManager.h"


using namespace std;

//setting static variables
GLuint ShaderManager::s_shaderInUse = 0;
map<GLuint,string> ShaderManager::s_existingsShaders = map<GLuint,string>();

void ShaderManager::clearShaders()
{
	glUseProgram(0);
	//deleting all shaders
	map<GLuint,string>::iterator it;
	for(it = s_existingsShaders.begin(); it != s_existingsShaders.end(); it++)
	{
		glDeleteProgram(it->first);
	}
	//clearing the map
	s_existingsShaders.clear();
	s_shaderInUse = 0;
}

GLuint ShaderManager::getShader(char *vertName,char *fragName)
{
	stringstream streamToFind;
	streamToFind << vertName << fragName;
	string shaderConcatenedName;
	streamToFind >> shaderConcatenedName;
	if(shaderExists(shaderConcatenedName))
	{
		map<GLuint,string>::iterator it;
		for(it = s_existingsShaders.begin(); it != s_existingsShaders.end(); it++)
			if(it->second == shaderConcatenedName)
				return it->first;
	}
	else
	{
		stringstream streamVert, streamFrag;
		streamVert << "Shaders\\" <<vertName;
		streamFrag << "Shaders\\" <<fragName;
		GLuint newShader = tools::initShaders(streamVert.str().c_str(), streamFrag.str().c_str());
		s_existingsShaders.insert(map<GLuint,string>::value_type(newShader,shaderConcatenedName));
		return newShader;
	}
}

bool ShaderManager::shaderExists(char *vertName,char *fragName)
{
	stringstream streamToFind;
	streamToFind << vertName << fragName;
	map<GLuint,string>::iterator it;
	for(it = s_existingsShaders.begin(); it != s_existingsShaders.end(); it++)
		if(it->second == streamToFind.str())
			return true;
	return false;
}

bool ShaderManager::shaderExists(string concatenedName)
{
	map<GLuint,string>::iterator it;
	for(it = s_existingsShaders.begin(); it != s_existingsShaders.end(); it++)
		if(it->second == concatenedName)
			return true;
	return false;
}

bool ShaderManager::shaderExists(GLuint shaderID)
{
	return s_existingsShaders.find(shaderID) != s_existingsShaders.end();
}

void ShaderManager::deleteShader(GLuint shaderID)
{
	if(shaderExists(shaderID))
	{
		s_existingsShaders.erase(shaderID);
		glDeleteProgram(shaderID);
	}
}

void ShaderManager::useShader(GLuint shaderID)
{
	if(s_shaderInUse != shaderID)
	{
		if(shaderExists(shaderID))
		{
			glUseProgram(shaderID);
			s_shaderInUse = shaderID;
		}
		else
		{
			glUseProgram(0);
			s_shaderInUse = 0;
		}
	}
}
