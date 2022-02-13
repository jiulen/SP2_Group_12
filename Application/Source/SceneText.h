#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <fstream>
#include <vector>

class SceneText : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_RING,
		GEO_SPHERE,
		GEO_HEMISPHERE,
		GEO_CYLINDER,
		GEO_CONE,
		GEO_TORUS,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_BLEND,
		GEO_GROUND,
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_MODEL3,
		GEO_MODEL4,
		GEO_MODEL5,
		GEO_MODEL6,
		GEO_MODEL7,
		GEO_MODEL8,
		GEO_TABLE,
		GEO_BENCH,
		GEO_TEXT,
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
	void RenderSkybox();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	float FPS;
	std::ifstream fileStream;
	unsigned textMaxWidth;
	unsigned textSpacing[256];
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};
#endif