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
#include "Watcher.h"
#include "Boss.h"
#include "ScaredGuy.h"
#include "Rain.h"
#include "Vector3.h"
#include <fstream>

class SceneGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		//Shapes
		GEO_CUBE, //for particle
		GEO_SPHERE, //for bullet
		GEO_SPHERE2, //for boss bullet

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
		GEO_BOMBARROW,
		GEO_TEXT,

		//OBJs
		GEO_ENEMY1,
		GEO_ENEMY2,
		GEO_BOSS,
		GEO_NPC,
		GEO_BIRD,

		GEO_SPIKE,

		GEO_BIGHOUSE_A,
		GEO_BIGHOUSE_F,
		GEO_BIGHOUSE_G,
		GEO_SKYSCRAPER_A,
		GEO_SKYSCRAPER_E,
		GEO_SKYSCRAPER_F,
		GEO_SMALLHOUSE_C,
		GEO_SMALLHOUSE_D,
		GEO_SMALLHOUSE_E,
		GEO_SMALLHOUSE_F,
		GEO_WALL,
		GEO_WALL_CORNER,
		GEO_LIGHTPOST,
		GEO_BENCH,
		GEO_GROUND_GRASS,
		GEO_TREE,
		GEO_BOMB,
		GEO_GUN,
		GEO_RED,
		GEO_BLACK,
		GEO_BLUE,
		GEO_TEXTBOX,
		GEO_RAIN,
		GEO_COIN,

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
	void Reset();
	float DistBetweenPoints(float x1, float z1, float x2, float z2);
	void RenderSkybox();
	void RenderBomb();
	void RenderBoss();
	void RenderHUD();
	void RenderTutorial();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y, Vector3 rotationAxis = Vector3(0, 0, 1), float angle = 0.f);
	void RenderAmbientEffects();

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

	double timer = 0;
	double spiketimer = 0;
	int spikestart = 0;
	float spikeypos = -1.5;
	float spikexpos = 0;
	float spikezpos = 0;
	int spikedmg = 0;
	int spikelockon = 0;
	int win = 0;
	int bossshoot = 0;
	double prevshot = 0;

	//tutorial
	int tutorial = 0;
	double keyused = 0;
	double timer2 = 0;
	int starttimer2 = 0;
	int stage = -3;
	Vector3 currentview;

	//Ambient Effetcs
	float rainy1 = 50;
	float rainy2 = 50;
	std::vector<Rain*> rain;
	int spawnrain = 0;
	double raintime = 0;
	
	//sidequests
	bool firstcoinPicked;
	bool secondcoinPicked;
	bool thirdcoinPicked;
	bool fourthcoinPicked;
	bool fifthcoinPicked;
	bool sixthcoinPicked;
	bool seventhcoinPicked;
	bool eighthcoinPicked;
	int coinscollected;

	//spawn enemies
	int spawn = 0;
	
	//Cheats
	int cheat = 0;

	Vector3 bombPos;

	std::vector<Hitbox> hitboxes; //floats are pos x, y, z and size x, y, z

	float yaw, pitch;
	Vector3 rightvector;

	int bulletCount;
	bool bulletHit;
	std::vector<Bullet> bulletVector;

	//Init player
	Player player;
	std::vector<Entity*> entities;

	//bullet effect
	std::vector<Particle> particles;

	//Reloading and the animation
	bool reloading;
	float reloadTime;
	float reloadRotateTime;
	float reloadAngle;
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