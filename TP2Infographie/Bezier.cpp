#define _USE_MATH_DEFINES
#include <math.h>
#include "Bezier.h"
#include <iostream>
#include <fstream>

using namespace std;

Bezier::Bezier()
{
	m_lengthTabPtCircles ++;
}



void Bezier::calcFragBezier(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3, int iLoop)
{
	for(int i=0; i<m_nbPointsVoulu; i++)
	{
		float t = i/m_nbPointsVoulu;
		glm::vec3 temp = (*P0)*pow((1-t),3) + (*P1)*3.0f*t*pow((1-t),2) + (*P2)*3.0f*pow(t,2)*(1-t) + (*P3)*pow(t,3);
		m_tabPointsCourbe[i*3] = temp.x;
		m_tabPointsCourbe[i*3+1] = temp.y;
		m_tabPointsCourbe[i*3+2] = temp.z;

		calcCircles(P0, P1, P2, P3, &temp, t, iLoop, i);
	}
}

void Bezier::initBezierMesh()
{
	m_bezierCurve.init(m_lengthTabPtCircles, m_tabPointsCircles,nullptr,m_tabNormBezier,nullptr,m_lengthTabPtTriangle,m_tabSommetTrianglesTube,false);
}

void Bezier::draw()
{
	m_bezierCurve.draw();
}

void Bezier::calcCurve(string filename)
{
	loadFile(filename);

	glm::vec3 P0, P1, P2, P3;

	for(int i = 0; i<m_nbPoints; i+9)
	{
		P0 = glm::vec3(m_listPoints[i], m_listPoints[i+1], m_listPoints[i+2]);
		P1 = glm::vec3(m_listPoints[i+3], m_listPoints[i+4], m_listPoints[i+5]);
		P2 = glm::vec3(m_listPoints[i+6], m_listPoints[i+7], m_listPoints[i+8]);
		P3 = glm::vec3(m_listPoints[i+9], m_listPoints[i+10], m_listPoints[i+11]);

		calcFragBezier(&P0, &P1, &P2, &P3, i);
	}

	createListTriangles();
}

void Bezier::calcCircles(glm::vec3 *P0, glm::vec3 *P1, glm::vec3 *P2, glm::vec3 *P3, glm::vec3 *center, float t, int iLoop, int iBezier)
{
	m_RCircle = 5.0;
	glm::vec3 E = (*P1) - (*P0);
	glm::vec3 F = (*P2) - (*P1);
	glm::vec3 G = (*P3) - (*P2);
	glm::vec3 tan = pow((1-t),2)*E + 2.0f*t*(1-t)*F + pow(t,2)*G;
	glm::vec3 norm = (1-t) * (F-E) + t * (G-F);
	glm::vec3 ban = glm::cross(tan, norm);
	norm = glm::cross(tan,ban);

	norm = (1.0f/glm::normalize(norm)) * norm;
	ban = (1.0f/glm::normalize(ban)) * ban;

	float pi = M_PI;

	for(int i=0; i<m_nbPointsCircle; i++)
	{
		glm::vec3 ptCircle = (*center) + m_RCircle * glm::cos((i/m_nbPointsCircle) * 2 * pi) * norm + m_RCircle * glm::sin((i/m_nbPointsCircle) * 2 * pi) * ban;

		int indexPart1 = i*3;
		int indexPart2 = m_nbPointsCircle * iBezier * 3;
		int indexPart3 = m_nbPointsCircle * m_nbPointsVoulu * iLoop * 3;
		int index = indexPart1 + indexPart2 + indexPart3;

		m_tabPointsCircles[index] = ptCircle.x;
		m_tabPointsCircles[index+1] = ptCircle.y;
		m_tabPointsCircles[index+2] = ptCircle.z;

		glm::vec3 normale = ptCircle - *center;

		m_tabNormBezier[index] = normale.x;
		m_tabNormBezier[index+1] = normale.y;
		m_tabNormBezier[index+2] = normale.z;
	}
}

void Bezier::createListTriangles()
{
	m_lengthTabPtTriangle = 0;
	int firstCircleTriangles = 0;
	for(int i=1; i<m_nbPointsCircle; i++)
	{
		m_tabSommetTrianglesTube[(i-1)*3] = 0;
		m_tabSommetTrianglesTube[(i-1)*3 + 1] = i;
		m_tabSommetTrianglesTube[(i-1)*3 + 2] = i + 1;

		m_lengthTabPtTriangle ++;
		firstCircleTriangles++;
	}

	int nbCircles = (m_lengthTabPtCircles/3)/m_nbPointsCircle;
	for(int i=0; i<nbCircles; i++)
	{
		for(int j=0; j<m_nbPointsCircle; j++)
		{
			m_tabSommetTrianglesTube[firstCircleTriangles + j*6 + (i*m_nbPointsCircle*6)] = j + (i*m_nbPointsCircle);
			m_tabSommetTrianglesTube[firstCircleTriangles + j*6 + (i*m_nbPointsCircle*6) + 1] = j + (i*m_nbPointsCircle) + m_nbPointsCircle;
			m_tabSommetTrianglesTube[firstCircleTriangles + j*6 + (i*m_nbPointsCircle*6) + 2] = j + (i*m_nbPointsCircle) + 1;
			m_tabSommetTrianglesTube[firstCircleTriangles + j*6 + (i*m_nbPointsCircle*6) + 3] = j + (i*m_nbPointsCircle) + 1;
			m_tabSommetTrianglesTube[firstCircleTriangles + j*6 + (i*m_nbPointsCircle*6) + 4] = j + (i*m_nbPointsCircle) + m_nbPointsCircle;
			m_tabSommetTrianglesTube[firstCircleTriangles + j*6 + (i*m_nbPointsCircle*6) + 5] = j + (i*m_nbPointsCircle) + m_nbPointsCircle + 1;

			m_lengthTabPtTriangle ++;
		}
	}
}

void Bezier::loadFile(string filename)
{
	ifstream myFile;
	myFile.open(filename);
	if(myFile.is_open()){

		myFile >> m_nbPointsVoulu;
		myFile >> m_nbPointsCircle;

		while(!myFile.eof()){
			myFile >> m_listPoints[m_nbPoints*3];
			myFile >> m_listPoints[m_nbPoints*3 + 1];
			myFile >> m_listPoints[m_nbPoints*3 + 2];

			m_nbPoints ++;
		}
	}

	myFile.close();
}