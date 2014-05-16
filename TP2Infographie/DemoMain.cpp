#include "DemoMain.h"

#define PI 3.141592f


using namespace std;

DemoMain *DemoMain::s_instance;

////////////////
//STATIC MEMBERS
////////////////

DemoMain &DemoMain::getInstance()
{
	if(s_instance == nullptr)// if there is no current instance
		s_instance = new DemoMain();
	return *s_instance;
}

void DemoMain::deleteInstance()
{
	if(s_instance != nullptr)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}

/////////////////
//INSTANCE MEMBER
/////////////////

void DemoMain::setupRC()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
        tools::exitFatalError("Unable to initialize SDL"); 
	  
    // Request an OpenGL 3.0 context.
    // If you request a context not supported by your drivers,
    // no OpenGL context will be created
	
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on

    // Turn on x4 multisampling anti-aliasing (MSAA)
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering

	m_windowWidth = 800;
	m_windowHeight = 600;

    // Create 800x600 window
    m_window = SDL_CreateWindow("SDL/GLM/OpenGL Torus game",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if (!m_window) // Check window was created OK
        tools::exitFatalError("Unable to create window");

    // Create opengl context and attach to window
    m_glContext = SDL_GL_CreateContext(m_window);
    // set swap buffers to sync with monitor's vertical refresh rate
    SDL_GL_SetSwapInterval(1);

	GLenum err = glewInit();
	// Required on Windows... init GLEW to access OpenGL beyond 1.1
	// remove on other platforms
	if (GLEW_OK != err)
	{	// glewInit failed, something is seriously wrong.
		cout << "glewInit failed, aborting." << endl;
		exit (1);
	}
	m_isSetupDone = true;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

DemoMain::DemoMain()
{
	m_isSetupDone = false;
	m_corde = nullptr;
}


DemoMain::~DemoMain()
{
	if(m_isSetupDone)
	{
		SDL_GL_DeleteContext(m_glContext);
		SDL_DestroyWindow(m_window);
		m_isSetupDone = false;
	}
	if(m_corde != nullptr)
		delete m_corde;
	SDL_Quit();
}

void DemoMain::init()
{
	if(!m_isSetupDone)//if the setup is not done
		setupRC();

	//////////////
	//LOAD MESHES
	/////////////

	m_maxRadius = 1.0f;
	m_minRadius = 0.2f;
	m_nbPointsMax = 100;
	m_nbPointsMin = 20;

	m_cordeMesh.loadFromObjFile("Models//sphere.obj");
	m_playerMesh.createTorus(m_maxRadius,m_minRadius,m_nbPointsMax,m_nbPointsMin);

	////////////////////////////////
	//CREATE RENDERERS AND MATERIALS
	////////////////////////////////

	tools::materialStruct material ={
		{0.2f, 0.2f, 0.4f, 1.0f}, // ambient
		{0.5f, 0.5f, 0.8f, 1.0f}, // diffuse
		{0.8f, 0.8f, 1.0f, 1.0f}, // specular
		2.0f};  // shininess
	
	m_corde = new BaseMeshRenderer();
	((BaseMeshRenderer*)m_corde)->setMesh(&m_cordeMesh);
	m_corde->initShaders("colour.vert","colour.frag");
	m_corde->setMaterial(material);
	m_corde->setUniformVec4("colour",glm::vec4(0.5f,0.0f,0.7f,1.0f));

	/////////
	//PLAYER
	/////////

	
	//create the player
	
	m_player.setMesh(&m_playerMesh);
	m_player.initShaders("colour.vert","colour.frag");
	m_player.setMaterial(material);
	m_player.setUniformVec4("colour",glm::vec4(0.0f,0.5f,0.7f,1.0f));
	m_playerPosition = glm::vec3(0.0f,0.0f,0.0f);
	
	/////////
	//CAMERA
	/////////

	m_projection = glm::perspective(60.0f,800.0f/600.0f,1.0f,100.0f);
	m_horizontalAngle = m_verticalAngle = 0;

	/////////
	//SKY BOX
	/////////

	m_sky = SkyBox();
	m_sky.init("SkyBox\\bottom.bmp", "SkyBox\\top55.bmp", "SkyBox\\left55.bmp", "SkyBox\\front55.bmp", "SkyBox\\right55.bmp", "SkyBox\\back55.bmp");

	///////
	//LIGHT
	///////

	//start with no light
	tools::lightStruct light ={
		{0.4f,0.4f,0.4f,1.0f},//ambiant
		{0.6f,0.4f,0.4f,1.0f},//diffuse
		{0.3f,0.3f,0.3f,1.0f},//specular
		{0.0f,5.0f,1.0f,1.0f},//position
		0.1f,//constant atenuation
		0.0f,//linear Attenuation
		0.0f//quadratic Attenuation
	};

	m_light.initLight(light);

	///////
	//OTHER
	///////
	SDL_ShowCursor(SDL_DISABLE);
}

void DemoMain::draw()
{
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.5, 0.5, 0.5, 1.0); // set background colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window

	m_sky.draw();

	m_light.setUniform(m_player.getShader());
	m_player.draw();

	m_light.setUniform(m_corde->getShader());
	m_corde->draw();

	SDL_GL_SwapWindow(m_window); // swap buffers
}

void DemoMain::update()
{
	////////////////////
	//KEY DOWN HANDLING
	////////////////////

	//get the key pressed
	Uint8 *keys = SDL_GetKeyboardState(nullptr);
	if ( keys[SDL_SCANCODE_ESCAPE] )
		m_running = false;

	int forwardMove, sideMove;
	forwardMove = sideMove = 0;

	if ( keys[SDL_SCANCODE_A] )
		sideMove += -1;
	if ( keys[SDL_SCANCODE_D] )
		sideMove += 1;
	if ( keys[SDL_SCANCODE_W] )
		forwardMove += 1;
	if ( keys[SDL_SCANCODE_S] )
		forwardMove += -1;

	////////////////////////
	//CALCULATION FOR CAMERA
	////////////////////////

	if(m_xMovementMouse != 0 || m_yMovementMouse != 0)//if anything change, we do some calculation to get the new angles
	{
		float mouseSpeed = 0.2f;
		if( m_verticalAngle < PI/2 && m_verticalAngle > -PI/2)
			m_horizontalAngle += mouseSpeed * m_timeElapsed * (float)(-m_xMovementMouse);
		else
			m_horizontalAngle += -1 * mouseSpeed * m_timeElapsed * (float)(-m_xMovementMouse);
		m_verticalAngle   += mouseSpeed * m_timeElapsed * (float)(-m_yMovementMouse);
		//prevent to have an angle greater than PI or lesser than -PI
		if(m_verticalAngle > PI)
			m_verticalAngle -= 2*PI;
		if(m_verticalAngle < -PI)
			m_verticalAngle += 2*PI;

		m_player.setRotation(glm::vec3(m_verticalAngle,m_verticalAngle,0.0f));
	}

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
	cos(m_verticalAngle) * sin(m_horizontalAngle),
	sin(m_verticalAngle),
	cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(m_horizontalAngle - M_PI/2.0f),
		0,
		cos(m_horizontalAngle - M_PI/2.0f)
	);

	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross( right, direction );

	if(sideMove != 0 || forwardMove!= 0)//if a key is pressed
	{
		float moveSpeed = 10.0;
		if(sideMove!= 0)
		{
			m_playerPosition =  m_playerPosition + glm::normalize(right) * (float)sideMove * m_timeElapsed * moveSpeed;
		}
		if(forwardMove!= 0)
		{
			m_playerPosition =  m_playerPosition + glm::normalize(direction) * (float)forwardMove * m_timeElapsed * moveSpeed;
		}
		m_player.setPosition(m_playerPosition);
	}

	//rotate the player and the light

	//camera position : the player position, less 2.5 times the direction vector (2.5 is an arbitrary value) and
	//we put the camera a little higher with yShift to see better around the character
	glm::vec3 cameraPosition = m_playerPosition - glm::normalize(direction)*2.5f;

	if(m_xMovementMouse != 0 || m_yMovementMouse != 0 || sideMove != 0 || forwardMove!= 0)//if nothing have changed, we don't change the view matrix
	{
		m_view = glm::lookAt(cameraPosition,// Camera is here
			m_playerPosition, // and looks here : at the same position, plus "direction"
			up);

		m_corde->setViewMatrix(m_view);
		m_player.setViewMatrix(m_view);
		//m_player.setViewMatrix(glm::translate(glm::mat4(glm::mat3(m_view)), glm::vec3(direction)*2.5f)); //not for for the seeked result
		//m_player.setModelMatrix(glm::translate(m_view,glm::normalize(direction)*2.5f)); //Teleportation while you're not seing the torus
		//m_player.setModelMatrix(glm::transpose(m_view)); // Infinite tunnel effect
		
		//reset the movement with the mouse
		m_xMovementMouse = 0;
		m_yMovementMouse = 0;
	}

	m_playerLastPosition = m_playerPosition;
	
	/////////////////////
	//COLLISION DETECTION
	/////////////////////
	
}

void DemoMain::handleCollisionDetection()
{
}

void DemoMain::handleEvents()
{
	SDL_Event sdlEvent;	// variable to detect SDL events

	bool updateTorus = false;

	while(SDL_PollEvent(&sdlEvent))
	{
		updateTorus = false;
		if (sdlEvent.type == SDL_QUIT)
			m_running = false;
		else if (sdlEvent.type == SDL_MOUSEMOTION)
		{
			if(sdlEvent.motion.x != m_windowWidth/2 || sdlEvent.motion.y != m_windowHeight/2)
			{
				m_xMovementMouse = sdlEvent.motion.xrel;
				m_yMovementMouse = sdlEvent.motion.yrel;
				SDL_WarpMouseInWindow(m_window,m_windowWidth/2,m_windowHeight/2);
			}
		}
		if (sdlEvent.type == SDL_KEYDOWN)
		{
			// Can extend this to handle a wider range of keys
			switch( sdlEvent.key.keysym.sym )
			{
				case SDLK_UP:
					m_maxRadius += 0.1f;
					updateTorus = true;
					break;
				case SDLK_DOWN:
					m_maxRadius -= 0.1f;
					updateTorus = true;
					break;
				case SDLK_LEFT:
					m_minRadius -= 0.02f;
					updateTorus = true;
					break;
				case SDLK_RIGHT:
					m_minRadius += 0.02f;
					updateTorus = true;
					break;
				case SDLK_i:
					m_nbPointsMax ++;
					updateTorus = true;
					break;
				case SDLK_k:
					m_nbPointsMax --;
					updateTorus = true;
					break;
				case SDLK_j:
					m_nbPointsMin --;
					updateTorus = true;
					break;
				case SDLK_l:
					m_nbPointsMin ++;
					updateTorus = true;
					break;
					
				default:
					break;
			}
		}
	}

	if (updateTorus)
		m_playerMesh.createTorus(m_maxRadius,m_minRadius,m_nbPointsMax,m_nbPointsMin);
}

void DemoMain::run()
{
	clock_t currentTime, LastTime;
	currentTime = LastTime = clock();
	m_running = true;
	while (m_running)
	{
		currentTime = clock();
		m_timeElapsed = ((float)(currentTime - LastTime)/CLOCKS_PER_SEC);
		LastTime = currentTime;
		handleEvents();
		update();
		draw();
	}
}

float DemoMain::getElapsedTime()
{
	return m_timeElapsed;
}
