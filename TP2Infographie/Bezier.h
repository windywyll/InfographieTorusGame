#ifndef BEZIER
#define BEZIER

#include "Mesh.h"
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <string>
#include <vector>

class Bezier{
	private:
		Mesh m_bezierCurve;
		std::vector<GLfloat> m_listPoints;
		int m_nbPointsVoulu;
		std::vector<GLfloat> m_tabPointsCourbe;
		std::vector<GLfloat> m_tabPointsTube;
		int m_nbPointsCircle;
		std::vector<GLfloat> m_tabPointsCircles;
		std::vector<GLfloat> m_tabNormBezier;
		float m_RCircle;
		std::vector<GLuint> m_tabSommetTrianglesTube;

	public:

		Bezier();
		void loadFile(std::string filename);
		void calcFragBezier(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3);
		void calcCurve(std::string filename);
		void calcCircles(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3, glm::vec3 *center, float t);
		void createListTriangles();
		Mesh initBezierMesh();
};

#endif