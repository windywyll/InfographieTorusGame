#ifndef BEZIER
#define BEZIER

#include "Mesh.h"
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <string>

class Bezier{
	private:
		Mesh m_bezierCurve;
		GLuint m_nbPoints;
		GLfloat *m_listPoints;
		int m_nbPointsVoulu;
		GLfloat* m_tabPointsCourbe;
		GLfloat* m_tabPointsTube;
		int m_nbPointsCircle;
		GLuint m_lengthTabPtCircles;
		GLfloat* m_tabPointsCircles;
		GLfloat* m_tabNormBezier;
		float m_RCircle;
		GLuint* m_tabSommetTrianglesTube;
		GLuint m_lengthTabPtTriangle;

	public:

		Bezier();
		void loadFile(std::string filename);
		void calcFragBezier(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3, int iLoop);
		void calcCurve(std::string filename);
		void calcCircles(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3, glm::vec3 *center, float t, int iLoop, int iBezier);
		void createListTriangles();
		void initBezierMesh();
		void draw();
};

#endif