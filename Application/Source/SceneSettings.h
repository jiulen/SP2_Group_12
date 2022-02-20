#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H

#include "Scene.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <fstream>

class SceneSettings : public Scene
{
	enum GEOMETRY_TYPE
	{
		//Shapes
		
		//TGAs
		GEO_TEXT,
		GEO_BACKGROUND,
		GEO_TITLE,
		GEO_BACKBUTTON,
		GEO_OPTIONSLIDER,
		GEO_OPTIONKNOB,
		GEO_OPTIONDESELECT,
		GEO_OPTIONSELECT,

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
	Light light[1];
	bool bLightEnabled;
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y);
	float FPS;
	std::ifstream fileStream;
	unsigned textMaxWidth;
	unsigned textSpacing[256];
	bool redPicked;
	bool bluePicked;
	bool greenPicked;
	int nextscene = 2;
	double mouseX = 0;
	double mouseY = 0;
public:
	SceneSettings();
	~SceneSettings();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual int NextScene();
	virtual void UseScene();
};
#endif