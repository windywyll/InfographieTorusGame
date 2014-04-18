#include "TextureManager.h"

using namespace std;

map<char *,GLuint> TextureManager::s_repeatTextures = map<char *,GLuint>();
map<char *,GLuint> TextureManager::s_clampTextures = map<char *,GLuint>();

GLuint TextureManager::loadTexture(char *fname, GLuint *texID, bool repeat)
{
	// generate texture ID
	glGenTextures(1, texID);

	// load file - using core SDL library
	SDL_Surface *tmpSurface;

	stringstream streamTex;
	streamTex << "Textures\\" << fname;

	tmpSurface = SDL_LoadBMP(streamTex.str().c_str());
	if (!tmpSurface)
	{
		cout << "Error loading bitmap" << endl;
	}
	glActiveTexture(GL_TEXTURE0); 

	// bind texture and set parameters
	glBindTexture(GL_TEXTURE_2D, *texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(repeat)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	
	char *type;
	GLuint colours = tmpSurface->format->BytesPerPixel;
	GLuint format, internalFormat;
	if (colours == 4) // alpha
	{
		internalFormat = GL_RGBA;
		type = "Three colors and alpha";
		if (tmpSurface->format->Rmask == 0x000000ff)
			format = GL_RGBA;
	    else
		    format = GL_BGRA;
	} else if (colours == 3)  // no alpha
	{
		internalFormat = GL_RGB;
		type = "Three colors";
		if (tmpSurface->format->Rmask == 0x000000ff)
			format = GL_RGB;
	    else
		    format = GL_BGR;
	}
	else
	{
		format = GL_DEPTH_COMPONENT;
		internalFormat = GL_DEPTH_COMPONENT;
		type = "Depth component";
	}

	glTexImage2D(GL_TEXTURE_2D,0, internalFormat, tmpSurface->w, tmpSurface->h, 0, format, GL_UNSIGNED_BYTE, tmpSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	// texture loaded, free the temporary buffer

	if(tmpSurface)
	{

		cout << "Texture " << fname << " loaded, type : " << type << endl;
	}

	SDL_FreeSurface(tmpSurface);

	return *texID;	// return value of texure ID, redundant really
}

void TextureManager::clearTextures()
{
	map<char *,GLuint>::iterator it;
	for(it = s_repeatTextures.begin(); it != s_repeatTextures.end();it++)
		glDeleteTextures(1,&it->second);
	s_repeatTextures.clear();

	for(it = s_clampTextures.begin(); it != s_clampTextures.end();it++)
		glDeleteTextures(1,&it->second);
	s_clampTextures.clear();
}

GLuint TextureManager::getTexture(char *textureFileName, bool repeat)
{
	if(repeat)
	{
		map<char *,GLuint>::iterator it = s_repeatTextures.find(textureFileName);
		if(it != s_repeatTextures.end())
			return it->second;
	}
	else
	{
		map<char *,GLuint>::iterator it = s_clampTextures.find(textureFileName);
		if(it != s_clampTextures.end())
			return it->second;
	}
	GLuint newTexture;
	loadTexture(textureFileName,&newTexture,repeat);
	if(repeat)
		s_repeatTextures.insert(map<char *,GLuint>::value_type(textureFileName,newTexture));
	else
		s_clampTextures.insert(map<char *,GLuint>::value_type(textureFileName,newTexture));
	return newTexture;
}

bool TextureManager::textureExists(char *textureFileName, bool repeat)
{
	if(repeat)
		return s_repeatTextures.find(textureFileName) != s_repeatTextures.end();
	else
		return s_clampTextures.find(textureFileName) != s_clampTextures.end();
}

bool TextureManager::textureExists(GLuint textureID)
{
	map<char *,GLuint>::iterator it;
	for(it = s_repeatTextures.begin(); it != s_repeatTextures.end();it++)
		if(it->second == textureID)
			return true;
	for(it = s_clampTextures.begin(); it != s_clampTextures.end();it++)
		if(it->second == textureID)
			return true;
	return false;
}

void TextureManager::deleteTexture(GLuint textureID)
{
	map<char *,GLuint>::iterator it;
	for(it = s_repeatTextures.begin(); it != s_repeatTextures.end();it++)
	{
		if(it->second == textureID)
		{
			glDeleteTextures(1,&it->second);
			s_repeatTextures.erase(it->first);
			break;
		}
	}
	for(it = s_clampTextures.begin(); it != s_clampTextures.end();it++)
	{
		if(it->second == textureID)
		{
			glDeleteTextures(1,&it->second);
			s_clampTextures.erase(it->first);
			break;
		}
	}
}

