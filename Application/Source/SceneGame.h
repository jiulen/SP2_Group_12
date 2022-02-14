#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "Scene.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <fstream>

class SceneGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		//Shapes
		
		//TGAs
		GEO_TEXT,
		//OBJs
		GEO_LIGHT0, //ceiling light (can be turned off)
		
		//Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

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
	Camera3 camera;
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Light light[2];
	bool bLightEnabled;
	float DistBetweenPoints(float x1, float z1, float x2, float z2);
	//Collision
	bool CollisionLineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	bool CollisionLineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh);
	bool CollisionPointCircle(float px, float py, float cx, float cy, float r);
	bool CollisionLinePoint(float x1, float y1, float x2, float y2, float px, float py);
	bool CollisionLineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r);
	//
	void RenderSkybox();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y);
	float interactRange;
	float FPS;
	std::string talk;
	std::string talkTarget;
	std::ifstream fileStream;
	unsigned textMaxWidth;
	unsigned textSpacing[256];
	bool redAlive, blackAlive, cyanAlive, orangeAlive;
	bool voting, lightsOn, impostor;
	bool turnLightOff;
	int voted; //0: not voted, 1: bots voted, 2: player voted
	int redVotes, blackVotes, cyanVotes, orangeVotes, noVotes;
	int mostVotes; //0 means tie/no vote, others means that player got most votes
	int gameOver; //0: not over, 1: win, 2: lose;
	bool task1, task2, task3; //1 is talk, 2 is button, 3 is knife
	bool iTask1, iTask2; //1 is turn off lights, 2 is kill
	float lightCutoff, lightInner;
	float
		headBodyScaleX, headBodyScaleZ,
		bodyScaleY,
		bodyBottomScaleY,
		headScaleY,
		headTopScaleY,
		visorScaleX, visorScaleY, visorScaleZ,
		thighLegScaleX, thighLegScaleZ,
		thighScaleY,
		thighTopBottomScaleY,
		legScaleY,
		legTopScaleY,
		redTranslateX, redTranslateY, redTranslateZ, redRotateX, redRotateY,
		blackTranslateX, blackTranslateY, blackTranslateZ,
		cyanTranslateX, cyanTranslateY, cyanTranslateZ,
		orangeTranslateX, orangeTranslateY, orangeTranslateZ;
public:
	SceneGame();
	~SceneGame();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};
#endif