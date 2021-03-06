#ifndef SCENE_NUMBERS_H
#define SCENE_NUMBERS_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <fstream>

class SceneNumbersGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		//Text
		GEO_TEXT,

		//Background
		GEO_BACKGROUND,

		//Game
		GEO_NUMBERS,
		GEO_TICK,

		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	MS modelStack, viewStack, projectionStack;
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Light light[2];
	bool bLightEnabled;

	//
	void RenderNumbersGame();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y, float rt);
	float interactRange;
	float FPS;
	std::ifstream fileStream;
	unsigned textMaxWidth;
	unsigned textSpacing[256];

	//game
	double timer = 0;
	int set = 0;
	int currentpos = -1;
	int pos[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int ticks[10] = { 0,0,0,0,0,0,0,0,0,0 };
	int clicked = 0;
	int prevno = 0;
	int currentno = 0;
	double mouseX = 0;
	double mouseY = 0;
public:
	SceneNumbersGame();
	~SceneNumbersGame();
	virtual int NextScene();
	virtual void UseScene();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};
#endif