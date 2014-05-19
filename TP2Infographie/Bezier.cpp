#define _USE_MATH_DEFINES
#include <math.h>
#include "Bezier.h"
#include <iostream>
#include <fstream>

using namespace std;

Bezier::Bezier()
{

}



void Bezier::calcFragBezier(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3)
{
	for(int i=0; i<m_nbPointsVoulu; i++)
	{
		float t = ((float) i)/m_nbPointsVoulu;
		glm::vec3 temp = (*P0)*pow((1-t),3) + (*P1)*3.0f*t*pow((1-t),2) + (*P2)*3.0f*pow(t,2)*(1-t) + (*P3)*pow(t,3);
		m_tabPointsCourbe.insert(m_tabPointsCourbe.end(),temp.x);
		m_tabPointsCourbe.insert(m_tabPointsCourbe.end(),temp.y);
		m_tabPointsCourbe.insert(m_tabPointsCourbe.end(),temp.z);

		calcCircles(P0, P1, P2, P3, &temp, t);
	}
}

Mesh Bezier::initBezierMesh()
{
	m_bezierCurve.init(m_tabPointsCircles.size()/3, m_tabPointsCircles.data(),nullptr,m_tabNormBezier.data(),nullptr,m_tabSommetTrianglesTube.size(),m_tabSommetTrianglesTube.data(),false);
	return m_bezierCurve;
}

void Bezier::calcCurve(string filename)
{
	loadFile(filename);

	glm::vec3 P0, P1, P2, P3;

	for(int i = 0; (i*9+11)<(m_listPoints.size()); i++)
	{
		P0 = glm::vec3(m_listPoints.at(i*9), m_listPoints.at(i*9+1), m_listPoints.at(i*9+2));
		P1 = glm::vec3(m_listPoints.at(i*9+3), m_listPoints.at(i*9+4), m_listPoints.at(i*9+5));
		P2 = glm::vec3(m_listPoints.at(i*9+6), m_listPoints.at(i*9+7), m_listPoints.at(i*9+8));
		P3 = glm::vec3(m_listPoints.at(i*9+9), m_listPoints.at(i*9+10), m_listPoints.at(i*9+11));

		calcFragBezier(&P0, &P1, &P2, &P3);
	}

	createListTriangles();
}

void Bezier::calcCircles(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3, glm::vec3 *center, float t)
{
	glm::vec3 E = (*P1) - (*P0);
	glm::vec3 F = (*P2) - (*P1);
	glm::vec3 G = (*P3) - (*P2);
	glm::vec3 tan = pow((1-t),2)*E + 2.0f*t*(1-t)*F + pow(t,2)*G;
	glm::vec3 norm = (1-t) * (F-E) + t * (G-F);
	glm::vec3 ban = glm::normalize(glm::cross(tan, norm));
	norm = glm::normalize(glm::cross(ban,tan));

	float pi = M_PI;

	for(int i=0; i<m_nbPointsCircle; i++)
	{
		float u = (((float) i)/m_nbPointsCircle) * 2 * pi;
		glm::vec3 ptCircle = (*center) + m_RCircle * cos(u) * ban + m_RCircle * sin(u) * norm;

		m_tabPointsCircles.insert(m_tabPointsCircles.end(),ptCircle.x);
		m_tabPointsCircles.insert(m_tabPointsCircles.end(),ptCircle.y);
		m_tabPointsCircles.insert(m_tabPointsCircles.end(),ptCircle.z);

		glm::vec3 normale = ptCircle - *center;

		m_tabNormBezier.insert(m_tabNormBezier.end(),normale.x);
		m_tabNormBezier.insert(m_tabNormBezier.end(),normale.y);
		m_tabNormBezier.insert(m_tabNormBezier.end(),normale.z);
	}
}

void Bezier::createListTriangles()
{
	for(int i=1; i<m_nbPointsCircle-1; i++)
	{
		m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),0);
		m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),i);
		m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),i+1);
	}

	int nbCircles = (m_tabPointsCircles.size()/3)/m_nbPointsCircle;
	for(int i=0; i<nbCircles-1; i++)
	{
		for(int j=0; j<m_nbPointsCircle; j++)
		{
			if(j == m_nbPointsCircle-1)
			{
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),j + (i*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),j + ((i+1)*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),0 + ((i)*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),0 + ((i)*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),j + ((i+1)*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),0 + ((i+1)*m_nbPointsCircle));
			}
			else
			{
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),j + (i*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),j + ((i+1)*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),(j + 1) + (i*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),(j + 1) + (i*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),j + ((i+1)*m_nbPointsCircle));
				m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),(j + 1) + ((i+1)*m_nbPointsCircle));
			}
		}
	}

	int baseEndPoint = (nbCircles-1)*m_nbPointsCircle;

	for(int i=1; i<m_nbPointsCircle-1; i++)
	{
		m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),baseEndPoint);
		m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),baseEndPoint+i);
		m_tabSommetTrianglesTube.insert(m_tabSommetTrianglesTube.end(),baseEndPoint+i+1);
	}
}

void Bezier::loadFile(string filename)
{
	GLfloat x,y,z;
	ifstream myFile;
	myFile.open(filename);
	if(myFile.is_open()){

		myFile >> m_nbPointsVoulu;
		myFile >> m_nbPointsCircle;
		myFile >> m_RCircle;

		while(!myFile.eof()){
			myFile >> x;
			myFile >> y;
			myFile >> z;

			m_listPoints.insert(m_listPoints.end(),x);
			m_listPoints.insert(m_listPoints.end(),y);
			m_listPoints.insert(m_listPoints.end(),z);
		}
	}

	myFile.close();
}