#ifndef ABSTRACT_MESH_RENDERER_H
#define ABSTRACT_MESH_RENDERER_H

#include <string>
#include <map>
#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "tools.h"

class AbstractMeshRenderer
{
	public:
		AbstractMeshRenderer();
		virtual ~AbstractMeshRenderer();

		void initShaders(char *vertName,char *fragName);
		void setShader(GLuint shaderProgram);
		GLuint getShader();

		//setters for uniforms
		void setTexture(char *fileName);
		void setUniformMat4(std::string name, glm::mat4 value);
		void setUniformVec4(std::string name, glm::vec4 value);
		void setUniform1f(std::string name, float value);
		void setMaterial(tools::materialStruct material){m_material = material;}
		tools::materialStruct getMaterial(){return m_material;}

		//testing if unifroms exists
		bool uniformExists(std::string name);
		//getInformation on uniforms
		glm::mat4 getUniformMat4(std::string name);
		glm::vec4 getUniformVec4(std::string name);
		float getUniform1f(std::string name);
		unsigned int getTextureID();
		//deleting uniforms
		void deleteUniform(std::string name);
		void clearUniforms();

		virtual void draw() = 0;
		
		virtual void calculateModelMatrix() = 0;

		glm::vec3 getPosition(){return m_position;}
		void setPosition(glm::vec3 position){ m_position = position;if(m_generatedModelMatrix) calculateModelMatrix();}
		glm::vec3 getScale(){return m_scale;}
		void setScale(glm::vec3 scale){ m_scale = scale;if(m_generatedModelMatrix) calculateModelMatrix();}
		glm::vec3 getRotation(){return m_rotation;}
		void setRotation(glm::vec3 rotation){m_rotation = rotation;if(m_generatedModelMatrix) calculateModelMatrix();}

		glm::mat4 getModelMatrix(){return m_model;}
		void setModelMatrix(glm::mat4 model){m_model = model; m_generatedModelMatrix = false;}
		bool isGeneratedModelMatrix(){return m_generatedModelMatrix;}

		glm::mat4 getViewMatrix(){return m_view;}
		void setViewMatrix(glm::mat4 view){m_view = view;}

	protected :
		std::map<std::string,glm::mat4> m_uniformMat4;
		std::map<std::string,glm::vec4> m_uniformVec4;
		std::map<std::string,float> m_uniform1f;
		unsigned int m_textureID;
		tools::materialStruct m_material;

		glm::mat4 m_model;
		glm::mat4 m_view;

		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::vec3 m_rotation;

		GLuint m_shaderProgram;

		bool m_generatedModelMatrix;
};

#endif