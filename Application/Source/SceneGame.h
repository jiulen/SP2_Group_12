#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "Scene.h"
#include "Camera.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Structs.h"
#include "Entity.h"
#include "Enemy.h"
#include "BasicMelee.h"
#include "Vector3.h"
#include <fstream>

class SceneGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		//Shapes
		GEO_CUBE, //for testing

		//HUD
		GEO_HEALTH,
		GEO_AMMO,
		GEO_REDCROSSHAIR,
		GEO_GREENCROSSHAIR,
		GEO_BLUECROSSHAIR,

		//Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//Ground
		GEO_GROUND,

		//Other TGAs
		GEO_TEXT,

		//OBJs
		GEO_ENEMY1,

		GEO_BIGHOUSE_A,
		GEO_BIGHOUSE_B,
		GEO_BIGHOUSE_C,
		GEO_BIGHOUSE_D,
		GEO_BIGHOUSE_E,
		GEO_BIGHOUSE_F,
		GEO_BIGHOUSE_G,
		GEO_LOWHOUSE_A,
		GEO_LOWHOUSE_B,
		GEO_LOWHOUSE_C,
		GEO_LOWHOUSE_D,
		GEO_LOWHOUSE_E,
		GEO_LOWHOUSE_F,
		GEO_LOWHOUSE_G,
		GEO_LOWHOUSE_H,
		GEO_LOWHOUSE_I,
		GEO_LOWHOUSE_J,
		GEO_LOWHOUSE_K,
		GEO_LOWHOUSE_L,
		GEO_LOWHOUSE_M,
		GEO_LOWHOUSE_N,
		GEO_SKYSCRAPER_A,
		GEO_SKYSCRAPER_B,
		GEO_SKYSCRAPER_C,
		GEO_SKYSCRAPER_D,
		GEO_SKYSCRAPER_E,
		GEO_SKYSCRAPER_F,
		GEO_SMALLHOUSE_A,
		GEO_SMALLHOUSE_B,
		GEO_SMALLHOUSE_C,
		GEO_SMALLHOUSE_D,
		GEO_SMALLHOUSE_E,
		GEO_SMALLHOUSE_F,
		GEO_BOMB,
		GEO_GUN,

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
	bool redPicked;
	bool bluePicked;
	bool greenPicked;
	float DistBetweenPoints(float x1, float z1, float x2, float z2);
	void RenderSkybox();
	void RenderBomb();
	void RenderHUD();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y);
	

	float FPS;
	bool enterScene;

	std::ifstream fileStream;
	unsigned textMaxWidth;
	unsigned textSpacing[256];

	int nextscene = 3;

	int crosshairenabled = 1;
	int crosshair = 1;
	int check = 0;
	int bombspawn = 0;
	int currentbomb = 0;
	int bombrand1 = 0;
	int bombrand2 = 0;
	int bombrand3 = 0;
	int bomb, bomb2, bomb3;
	int minigamesused[3] = { 0,0,0 };

	std::vector<Hitbox> hitboxes; //floats are pos x, y, z and size x, y, z
	float yaw, pitch;
	Vector3 rightvector;
	int bulletCount;

	//Init player
	Player player;
	std::vector<Entity*> entities;

public:
	SceneGame();
	~SceneGame();
	virtual void Init();
	virtual void Update(double dt);
	void UpdateEnemyMovement(double dt);
	void EnemyAttack(double dt);
	virtual void Render();
	virtual void Exit();
	virtual int NextScene();
	virtual void UseScene();
};
#endif