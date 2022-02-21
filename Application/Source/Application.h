
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();
	static int GetCrosshair();
	static void SetCrosshair(int a);
	static int GetReset();
	static void SetReset(int a);
	static unsigned m_width;
	static unsigned m_height;
	static unsigned crosshair;
	static unsigned resetscene;

private:

	//Declare a window object
	StopWatch m_timer;
	float lastX, lastY;
};

#endif