#ifndef DEMO_MAIN_H
#define DEMO_MAIN_H

#include "BaseMeshRenderer.h"
#include "SkyBox.h"
#include "Light.h"
#include "Bezier.h"

#include <ctime>
#include <vector>

#include <SDL.h>

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class DemoMain
{
public :
	static DemoMain &getInstance();
	static void deleteInstance();

	void init();
	void draw();
	void update();
	void handleCollisionDetection();//function used to have a clearer code
	void run();
	void handleEvents();
	glm::mat4 getViewMatrix(){return m_view;}
	glm::mat4 getProjectionMatrix(){return m_projection;}
	void incrementLight();//change the light intensity (and also the sound when the last sphere is collected)

	float getElapsedTime();

	static void exitFatalError(char *message);
	
private :
	static DemoMain *s_instance; //the instance of the singleton class
	SDL_GLContext m_glContext; // OpenGL context handle
    SDL_Window *m_window; // window handle
	bool m_isSetupDone;

	void setupRC();
	
	DemoMain();
	~DemoMain();

	float m_timeElapsed;

	bool m_running;

	//size of the window
	int m_windowWidth, m_windowHeight;

	//used for the mouse motion
	int m_xMovementMouse, m_yMovementMouse;
	float m_horizontalAngle, m_verticalAngle;

	glm::mat4 m_view;
	glm::mat4 m_projection;

	glm::vec3 m_playerPosition;
	glm::vec3 m_playerLastPosition;
	BaseMeshRenderer m_player;

	AbstractMeshRenderer *m_corde;
	Bezier m_bezierCorde;

	SkyBox m_sky;

	Mesh m_playerMesh, m_cordeMesh;

	GLfloat m_maxRadius, m_minRadius;
	GLuint m_nbPointsMax, m_nbPointsMin;

	//light
	Light m_light;
};

#endif
