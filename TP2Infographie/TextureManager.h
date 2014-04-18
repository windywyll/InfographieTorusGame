#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <GL\glew.h>
#include <SDL.h>
#include <iostream>
#include <sstream>
#include <map>

class TextureManager
{
public:
	static void clearTextures();
	static GLuint getTexture(char *textureFileName, bool repeat = true);
	static bool textureExists(char *textureFileName, bool repeat = true);
	static bool textureExists(GLuint textureID);
	static void deleteTexture(GLuint textureID);
private:
	static GLuint loadTexture(char *fname, GLuint *texID, bool repeat);
	static std::map<char *,GLuint> s_repeatTextures;
	static std::map<char *,GLuint> s_clampTextures;
};

#endif
