#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "SceneWiringGame.h"
#include "SceneNumbersGame.h"
#include "SceneCopyGame.h"
#include "SceneMainMenu.h"
#include "SceneSettings.h"
#include "SceneGame.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
unsigned Application::m_width;
unsigned Application::m_height;

void resize_callback(GLFWwindow* window, int w, int h)
{
	Application::m_width = w;
	Application::m_height = h;
	glViewport(0, 0, w, h);
}

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}

void Application::GetCursorPos(double* xpos, double* ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}

int Application::GetWindowWidth()
{
	return m_width;
}

int Application::GetWindowHeight()
{
	return m_height;
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	glfwSetWindowSizeCallback(m_window, resize_callback);

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	
	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_width = 800;
	m_height = 600;
	m_window = glfwCreateWindow(m_width, m_height, "Test Window", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run()
{
	//Main Loop
	Scene* scene1 = new SceneMainMenu();
	Scene* scene2 = new SceneSettings();
	Scene* scene3 = new SceneGame();
	Scene* scene4 = new SceneWiringGame();
	Scene* scene5 = new SceneNumbersGame();
	Scene* scene6 = new SceneCopyGame();
	Scene* scene = scene1;
	scene1->Init();
	scene2->Init();
	scene3->Init();
	scene4->Init();
	scene5->Init();
	scene6->Init();

	//Set cursor mode (do for scene switch also if cursor mode change)
	
	//

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		if ((scene == scene4) || (scene == scene5)||(scene==scene1)||(scene==scene2)||(scene==scene6))
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //do normal instead of disabled to restore normal cursor mode
		if (glfwRawMouseMotionSupported()) {
			glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
		int sceneno = scene->NextScene();
		if (IsKeyPressed(VK_F1))
			sceneno = 1;
		else if (IsKeyPressed(VK_F2))
			sceneno = 2;
		else if (IsKeyPressed(VK_F3))
			sceneno = 3;
		else if (IsKeyPressed(VK_F4))
			sceneno = 4;
		else if (IsKeyPressed(VK_F5))
			sceneno = 5;
		else if (IsKeyPressed(VK_F6))
			sceneno = 6;
		if (sceneno == 1)
			scene = scene1;
		else if (sceneno == 2)
			scene = scene2;
		else if (sceneno == 3)
			scene = scene3;
		else if (sceneno == 4)
			scene = scene4;
		else if (sceneno == 5)
			scene = scene5;
		else if (sceneno == 6)
			scene = scene6;
		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	scene1->Exit();
	delete scene1;
	scene2->Exit();
	delete scene2;
	scene3->Exit();
	delete scene3;
	scene4->Exit();
	delete scene4;
	scene5->Exit();
	delete scene5;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
