#ifndef SCENE_WIRINGGAME_H
#define SCENE_WIRINGGAME_H

#include "Scene.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <fstream>

class SceneWiringGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		//Text
		GEO_TEXT,

		//Background
		GEO_BACKGROUND,

		//Defuse
		GEO_WIRING,
		GEO_YWIRE,
		GEO_BWIRE,
		GEO_RWIRE,
		GEO_PWIRE,

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
	float DistBetweenPoints(float x1, float z1, float x2, float z2);
	//Collision
	bool CollisionLineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	bool CollisionLineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh);
	bool CollisionPointCircle(float px, float py, float cx, float cy, float r);
	bool CollisionLinePoint(float x1, float y1, float x2, float y2, float px, float py);
	bool CollisionLineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r);
	//
	void RenderSkybox();
	void RenderWiringGame();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y, float rt);
	float interactRange;
	float FPS;
	std::ifstream fileStream;
	unsigned textMaxWidth;
	unsigned textSpacing[256];
	
	//wiring
	int completed = 4;
	int yellow = 0;
	int blue = 0;
	int red = 0;
	int pink = 0;
	int correct = 0;
	int yattached = 0;
	int battached = 0;
	int rattached = 0;
	int pattached = 0;
	double mouseX = 0;
	double mouseY = 0;
public:
	SceneWiringGame();
	~SceneWiringGame();
	virtual int NextScene();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};
#endif