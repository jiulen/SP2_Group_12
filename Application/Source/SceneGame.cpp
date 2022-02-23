#include "SceneGame.h" 
#include "SceneSettings.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "MyMath.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include <fstream>
#include <sstream>
#include <iostream>

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::UseScene()
{
	glBindVertexArray(m_vertexArrayID);
	glUseProgram(m_programID);
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);

	enterScene = true;

	//values for start of game
	yaw = 0;
	pitch = 0;
	
	//Init player
	player = Player();

	if (Application::GetReset() == 1)
	{
		Reset();
		Application::SetReset(0);
	}
}

void SceneGame::Reset()
{
	player = Player();
	camera.Init(Vector3(0, 4.5, 5.5), Vector3(0, 4.5, 4.5), Vector3(0, 1, 0));
	for (int i = entities.size() - 1; i >= 0; i--) { //removes all entities
		delete entities[i];
		entities.pop_back();
	}
	bulletVector.clear();
	particles.clear();
	crosshairenabled = 1;
	check = 0;
	bombspawn = 0;
	currentbomb = 0;
	bombrand1 = 0;
	bombrand2 = 0;
	bombrand3 = 0;
	spawn = 0;
	timer = 0;
	spiketimer = 0;
	spikestart = 0;
	spikeypos = -1.5;
	spikexpos = 0;
	spikezpos = 0;
	spikedmg = 0;
	spikelockon = 0;
	win = 0;
	bossshoot = 0;
	prevshot = 0;
	bombPos = Vector3(0, 0, 0);
	for (int i = 0; i < 3; i++)
		minigamesused[i] = 0;
}

void SceneGame::Init()
{
	// Init VBO here

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	glUseProgram(m_programID);
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 100, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1.f;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(90));
	light[0].cosInner = cos(Math::DegreeToRadian(60));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(0, 100, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1.f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);


	camera.Init(Vector3(0, 4.5, 5.5), Vector3(0, 4.5, 4.5), Vector3(0, 1, 0)); //facing -x (1.0 diff in z)

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0, 0, 0));
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Daylight Box_Front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Daylight Box_Back.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Daylight Box_Top.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Daylight Box_Right.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Daylight Box_Left.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Daylight Box_Bottom.tga");
	meshList[GEO_GROUND] = MeshBuilder::GenerateRepeatingQuad("bottom", Color(1, 1, 1), 20.f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//whitebrickfloor.tga");
	meshList[GEO_ENEMY1] = MeshBuilder::GenerateOBJMTL("enemy1", "OBJ//basicCharacter.obj", "OBJ//basicCharacter.obj.mtl");
	meshList[GEO_ENEMY1]->textureID = LoadTGA("Image//skin_man.tga");
	meshList[GEO_ENEMY2] = MeshBuilder::GenerateOBJMTL("enemy2", "OBJ//basicCharacter.obj", "OBJ//basicCharacter.obj.mtl");
	meshList[GEO_ENEMY2]->textureID = LoadTGA("Image//skin_adventurer.tga");	
	meshList[GEO_BOSS] = MeshBuilder::GenerateOBJMTL("boss", "OBJ//basicCharacter.obj", "OBJ//basicCharacter.obj.mtl");
	meshList[GEO_BOSS]->textureID = LoadTGA("Image//skin_robot.tga");
	meshList[GEO_SPIKE] = MeshBuilder::GenerateOBJ("spike", "OBJ//spike.obj");
	meshList[GEO_BOMB] = MeshBuilder::GenerateOBJMTL("bomb", "OBJ//bomb.obj", "OBJ//bomb.mtl");
	meshList[GEO_GUN] = MeshBuilder::GenerateOBJMTL("gun", "OBJ//pistol.obj", "OBJ//pistol.mtl");
	meshList[GEO_GUN]->textureID = LoadTGA("Image//pistol.tga");
	meshList[GEO_BIGHOUSE_A] = MeshBuilder::GenerateOBJMTL("big house a", "OBJ//large_buildingA.obj", "OBJ//large_buildingA.mtl");
	meshList[GEO_BIGHOUSE_B] = MeshBuilder::GenerateOBJMTL("big house b", "OBJ//large_buildingB.obj", "OBJ//large_buildingB.mtl");
	meshList[GEO_BIGHOUSE_C] = MeshBuilder::GenerateOBJMTL("big house c", "OBJ//large_buildingC.obj", "OBJ//large_buildingC.mtl");
	meshList[GEO_BIGHOUSE_D] = MeshBuilder::GenerateOBJMTL("big house d", "OBJ//large_buildingD.obj", "OBJ//large_buildingD.mtl");
	meshList[GEO_BIGHOUSE_E] = MeshBuilder::GenerateOBJMTL("big house e", "OBJ//large_buildingE.obj", "OBJ//large_buildingE.mtl");
	meshList[GEO_BIGHOUSE_F] = MeshBuilder::GenerateOBJMTL("big house f", "OBJ//large_buildingF.obj", "OBJ//large_buildingF.mtl");
	meshList[GEO_BIGHOUSE_G] = MeshBuilder::GenerateOBJMTL("big house g", "OBJ//large_buildingG.obj", "OBJ//large_buildingG.mtl");
	meshList[GEO_LOWHOUSE_A] = MeshBuilder::GenerateOBJMTL("small house a", "OBJ//low_buildingA.obj", "OBJ//low_buildingA.mtl");
	meshList[GEO_LOWHOUSE_B] = MeshBuilder::GenerateOBJMTL("small house b", "OBJ//low_buildingB.obj", "OBJ//low_buildingB.mtl");
	meshList[GEO_LOWHOUSE_C] = MeshBuilder::GenerateOBJMTL("small house c", "OBJ//low_buildingC.obj", "OBJ//low_buildingC.mtl");
	meshList[GEO_LOWHOUSE_D] = MeshBuilder::GenerateOBJMTL("small house d", "OBJ//low_buildingD.obj", "OBJ//low_buildingD.mtl");
	meshList[GEO_LOWHOUSE_E] = MeshBuilder::GenerateOBJMTL("small house e", "OBJ//low_buildingE.obj", "OBJ//low_buildingE.mtl");
	meshList[GEO_LOWHOUSE_F] = MeshBuilder::GenerateOBJMTL("small house f", "OBJ//low_buildingF.obj", "OBJ//low_buildingF.mtl");
	meshList[GEO_LOWHOUSE_G] = MeshBuilder::GenerateOBJMTL("small house g", "OBJ//low_buildingG.obj", "OBJ//low_buildingG.mtl");
	meshList[GEO_LOWHOUSE_H] = MeshBuilder::GenerateOBJMTL("small house h", "OBJ//low_buildingH.obj", "OBJ//low_buildingH.mtl");
	meshList[GEO_LOWHOUSE_I] = MeshBuilder::GenerateOBJMTL("small house i", "OBJ//low_buildingI.obj", "OBJ//low_buildingI.mtl");
	meshList[GEO_LOWHOUSE_J] = MeshBuilder::GenerateOBJMTL("small house j", "OBJ//low_buildingJ.obj", "OBJ//low_buildingJ.mtl");
	meshList[GEO_LOWHOUSE_K] = MeshBuilder::GenerateOBJMTL("small house k", "OBJ//low_buildingK.obj", "OBJ//low_buildingK.mtl");
	meshList[GEO_LOWHOUSE_L] = MeshBuilder::GenerateOBJMTL("small house l", "OBJ//low_buildingL.obj", "OBJ//low_buildingL.mtl");
	meshList[GEO_LOWHOUSE_M] = MeshBuilder::GenerateOBJMTL("small house m", "OBJ//low_buildingM.obj", "OBJ//low_buildingM.mtl");
	meshList[GEO_LOWHOUSE_N] = MeshBuilder::GenerateOBJMTL("small house n", "OBJ//low_buildingN.obj", "OBJ//low_buildingN.mtl");
	meshList[GEO_SKYSCRAPER_A] = MeshBuilder::GenerateOBJMTL("skyscraper a", "OBJ//skyscraperA.obj", "OBJ//skyscraperA.mtl");
	meshList[GEO_SKYSCRAPER_B] = MeshBuilder::GenerateOBJMTL("skyscraper b", "OBJ//skyscraperB.obj", "OBJ//skyscraperB.mtl");
	meshList[GEO_SKYSCRAPER_C] = MeshBuilder::GenerateOBJMTL("skyscraper c", "OBJ//skyscraperC.obj", "OBJ//skyscraperC.mtl");
	meshList[GEO_SKYSCRAPER_D] = MeshBuilder::GenerateOBJMTL("skyscraper d", "OBJ//skyscraperD.obj", "OBJ//skyscraperD.mtl");
	meshList[GEO_SKYSCRAPER_E] = MeshBuilder::GenerateOBJMTL("skyscraper e", "OBJ//skyscraperE.obj", "OBJ//skyscraperE.mtl");
	meshList[GEO_SKYSCRAPER_F] = MeshBuilder::GenerateOBJMTL("skyscraper f", "OBJ//skyscraperF.obj", "OBJ//skyscraperF.mtl");
	meshList[GEO_SMALLHOUSE_A] = MeshBuilder::GenerateOBJMTL("small house a", "OBJ//small_buildingA.obj", "OBJ//small_buildingA.mtl");
	meshList[GEO_SMALLHOUSE_B] = MeshBuilder::GenerateOBJMTL("small house b", "OBJ//small_buildingB.obj", "OBJ//small_buildingB.mtl");
	meshList[GEO_SMALLHOUSE_C] = MeshBuilder::GenerateOBJMTL("small house c", "OBJ//small_buildingC.obj", "OBJ//small_buildingC.mtl");
	meshList[GEO_SMALLHOUSE_D] = MeshBuilder::GenerateOBJMTL("small house d", "OBJ//small_buildingD.obj", "OBJ//small_buildingD.mtl");
	meshList[GEO_SMALLHOUSE_E] = MeshBuilder::GenerateOBJMTL("small house E", "OBJ//small_buildingE.obj", "OBJ//small_buildingE.mtl");
	meshList[GEO_SMALLHOUSE_F] = MeshBuilder::GenerateOBJMTL("small house F", "OBJ//small_buildingF.obj", "OBJ//small_buildingF.mtl");
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJMTL("wall", "OBJ//stoneWall.obj", "OBJ//stoneWall.mtl");
	meshList[GEO_WALL_CORNER] = MeshBuilder::GenerateOBJMTL("wall", "OBJ//stoneWallCurve.obj", "OBJ//stoneWallCurve.mtl");
	meshList[GEO_LIGHTPOST] = MeshBuilder::GenerateOBJMTL("light post", "OBJ//lightpostSingle.obj", "OBJ//lightpostSingle.mtl");
	meshList[GEO_BENCH] = MeshBuilder::GenerateOBJMTL("bench", "OBJ//bench.obj", "OBJ//bench.mtl");

	//HUD + UI
	meshList[GEO_BLUE] = MeshBuilder::GenerateQuad("blue", Color(0, 0.8, 1), 1.f);
	meshList[GEO_RED] = MeshBuilder::GenerateQuad("red", Color(1, 0, 0), 1.f);
	meshList[GEO_BLACK] = MeshBuilder::GenerateQuad("black", Color(0, 0, 0), 1.f);
	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("health", Color(1, 1, 1), 1.f);
	meshList[GEO_HEALTH]->textureID = LoadTGA("Image//health.tga");
	meshList[GEO_AMMO] = MeshBuilder::GenerateQuad("ammo", Color(1, 1, 1), 1.f);
	meshList[GEO_AMMO]->textureID = LoadTGA("Image//ammo.tga");
	meshList[GEO_REDCROSSHAIR] = MeshBuilder::GenerateQuad("redcrosshair", Color(1, 1, 1), 1.f);
	meshList[GEO_REDCROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");
	meshList[GEO_GREENCROSSHAIR] = MeshBuilder::GenerateQuad("greencrosshair", Color(1, 1, 1), 1.f);
	meshList[GEO_GREENCROSSHAIR]->textureID = LoadTGA("Image//greencrosshair.tga");
	meshList[GEO_BLUECROSSHAIR] = MeshBuilder::GenerateQuad("bluecrosshair", Color(1, 1, 1), 1.f);
	meshList[GEO_BLUECROSSHAIR]->textureID = LoadTGA("Image//bluecrosshair.tga");
	meshList[GEO_BOMBARROW] = MeshBuilder::GenerateQuad("bomb arrow", Color(1, 1, 1), 1.f);
	meshList[GEO_BOMBARROW]->textureID = LoadTGA("Image//bomb-pointer.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//arial.tga");
	
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	glEnable(GL_DEPTH_TEST);

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);

	Math::InitRNG();
	textMaxWidth = 64; //initial value

	//for text spacing (check)
	const char* file_path;
	file_path = "Image//ArialFontData.csv"; //name of font data csv file
	fileStream.open(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
	}
	else {
		while (!fileStream.eof()) {
			std::string buf;
			std::getline(fileStream, buf, ' ');
			if (buf == "Char") {
				std::getline(fileStream, buf, ' ');
				unsigned i = stoi(buf);
				std::getline(fileStream, buf, ',');
				if (buf == "Base Width") {
					std::getline(fileStream, buf);
					unsigned width = stoi(buf);
					textSpacing[i] = width;
				}
				else {
					std::getline(fileStream, buf);
				}
			}
			else if (buf == "Cell") {
				std::getline(fileStream, buf, ',');
				if (buf == "Width") {
					std::getline(fileStream, buf);
					textMaxWidth = stoi(buf);
				}
				else {
					std::getline(fileStream, buf);
				}
			}
			else {
				std::getline(fileStream, buf);
			}
		}
		fileStream.close();
	}
	bombPos = Vector3(0, 0, 0);

	hitboxes.clear();

	rightvector = Vector3(1, 0, 0);
	FPS = 0;
	bLightEnabled = true;
	enterScene = true;
	bulletHit = true;


	//Init non moving hitboxes
	hitboxes.push_back(Hitbox(-60, 29, -60, 24, 58, 24));
	hitboxes.push_back(Hitbox(0, 9, -70, 32, 18, 20));
	hitboxes.push_back(Hitbox(-60, 40, 0, 24, 80, 24));
	hitboxes.push_back(Hitbox(0, 17, -20, 40, 34, 24));
	hitboxes.push_back(Hitbox(60, 12, -65, 40, 28, 16));
	hitboxes.push_back(Hitbox(60, 20, -15, 32, 40, 32));
	hitboxes.push_back(Hitbox(30.9, 4, -26.1, 0.6, 8, 0.6));
	hitboxes.push_back(Hitbox(30.9, 4, -3.9, 0.6, 8, 0.6));
	hitboxes.push_back(Hitbox(40, 9, -15, 20, 2, 24));
	hitboxes.push_back(Hitbox(-10, 12, 80, 16, 24, 16));
	hitboxes.push_back(Hitbox(-10, 4, 70.5, 16, 8, 3));
	hitboxes.push_back(Hitbox(-60, 13, 60, 16, 26, 16));
	hitboxes.push_back(Hitbox(-56, 13, 51, 8, 26, 2));
	hitboxes.push_back(Hitbox(-40, 8, 60, 16, 16, 16));
	hitboxes.push_back(Hitbox(10, 12, 80, 16, 24, 16));
	hitboxes.push_back(Hitbox(10, 4, 70.5, 16, 8, 3));
	hitboxes.push_back(Hitbox(60, 25, 60, 24, 50, 24));
	hitboxes.push_back(Hitbox(30, 10, 60, 10, 20, 5));
}

void SceneGame::Update(double dt)
{
	if (enterScene) {
		camera.setFirstMouse();
		enterScene = false;
	}


	FPS = 1 / (float)dt;
	
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	camera.Update(dt, hitboxes);

	if (Application::IsKeyPressed('R')) {
		bLightEnabled = true;
	}

	player.Update(dt);
	
	
	Vector3 viewvector = (camera.target - camera.position).Normalized();
	yaw = Math::RadianToDegree(atan2(-viewvector.x, -viewvector.z));
	pitch = Math::RadianToDegree(asin(viewvector.y));
	rightvector = camera.getRightVector();
	Vector3 viewvector2;
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getName() == "Boss")
		{
			viewvector2 = Vector3(camera.getPlayerHitbox().posX, camera.getPlayerHitbox().posY, camera.getPlayerHitbox().posZ) - Vector3(entities[i]->getHitbox().posX, entities[i]->getHitbox().posY, entities[i]->getHitbox().posZ);
			viewvector2.Normalize();
		}
	}

	//Reloading
	static bool rOnClick = false;
	static bool reloading = false;
	if (!rOnClick && Application::IsKeyPressed('R'))
	{
		rOnClick = true;
		if (!reloading) {
			reloading = true;
			PlaySound(L"Sound//reload.wav", NULL, SND_FILENAME | SND_ASYNC); //play reloading sound
			if (stage == 4)
				keyused = 1;
		}
	}
	else if (rOnClick && !Application::IsKeyPressed('R'))
	{
		rOnClick = false;
	}
	if (reloading == true)
	{
		player.reload(dt, reloading);
	}
	//Shooting
	if (Application::IsMousePressed(0))
	{
		if (!reloading) {
			if (player.attack(dt)) {
				PlaySound(L"Sound//single-shot.wav", NULL, SND_FILENAME | SND_ASYNC);
				bulletVector.push_back(Bullet(player.damage, 75, 1, 1, 1, viewvector, camera.position, 'P'));
				if (stage == 3)
					keyused++;
			}
		}
	}

	if (bossshoot==1)
	{
		PlaySound(L"Sound//single-shot.wav", NULL, SND_FILENAME | SND_ASYNC);
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->getName() == "Boss")
			{
				bulletVector.push_back(Bullet(entities[i]->getdamage(), 75, 1, 1, 1, viewvector2, Vector3(entities[i]->getHitbox().posX, entities[i]->getHitbox().posY, entities[i]->getHitbox().posZ), 'E'));
			}
		}
		bossshoot = 0;
	}

	//Enemy updates
	UpdateEnemyMovement(dt);
	EnemyAttack(dt);

	//Bullet updates
	for (int i = 0; i < bulletVector.size(); i++)
	{
		bulletVector[i].Update(dt);
	}
	std::vector<int>bulletInt;
	bulletInt.clear();
	for (int i = 0; i < bulletVector.size(); i++)
	{
		bool collided = false;

		if (bulletVector[i].bulletType == 'P') { //damage enemies
			for (int j = 0; j < entities.size() && !collided; j++)
			{
				if (bulletVector[i].bulletHit(entities[j]->getHitbox()))
				{
					entities[j]->takedamage(bulletVector[i].bulletDamage);
					bulletInt.push_back(i);
					collided = true;
				}
			}
		}

		if (bulletVector[i].bulletType == 'E') { //damage player
			if (bulletVector[i].bulletHit(camera.getPlayerHitbox()))
			{
				player.takedamage(bulletVector[i].bulletDamage);
				bulletInt.push_back(i);
				collided = true;
			}
		}
		
		for (int k = 0; k < hitboxes.size() && !collided; k++)
		{
			if (bulletVector[i].bulletHit(hitboxes[k]))
			{
				bulletInt.push_back(i);
				collided = true;
			}
		}

		if (collided) {
			Vector3 effectPos = Vector3(bulletVector[i].bulletHitbox.posX, bulletVector[i].bulletHitbox.posY, bulletVector[i].bulletHitbox.posZ) - bulletVector[i].directionVector * 0.5f; //moves bullet back 0.5f
			particles.push_back(Particle(effectPos));
		}
	}

	for (int i = bulletInt.size() - 1; i >= 0; i--)
	{
		bulletVector.erase(bulletVector.begin() + bulletInt[i]);
	}

	for (int i = bulletVector.size() - 1; i >= 0; i--)
	{
		if (bulletVector[i].bulletHitbox.posX > 200 || bulletVector[i].bulletHitbox.posX < -200 ||
			bulletVector[i].bulletHitbox.posY > 250 || bulletVector[i].bulletHitbox.posY < 0 + bulletVector[i].bulletHitbox.sizeY / 2 ||
			bulletVector[i].bulletHitbox.posZ > 200 || bulletVector[i].bulletHitbox.posZ < -200)
		{
			Vector3 effectPos = Vector3(bulletVector[i].bulletHitbox.posX, bulletVector[i].bulletHitbox.posY, bulletVector[i].bulletHitbox.posZ) - bulletVector[i].directionVector * 0.5f; //moves bullet back 0.5f
			particles.push_back(Particle(effectPos));
			bulletVector.erase(bulletVector.begin() + i);
		}
	}

	//remove particles
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].time += dt;
	}
	for (int i = particles.size() - 1; i >= 0; i--)
	{
		if (particles[i].time > 0.25) {
			particles.erase(particles.begin() + i);
		}
	}

	//remove dead enemies
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->getcurrenthealth() <= 0)
		{
			if (entities[i]->getName() == "Boss")
				win = 1;
			delete entities[i];
			entities[i] = nullptr;
		}
	}

	entities.erase(std::remove(entities.begin(), entities.end(), nullptr), entities.end());

	if (spikestart == 1)
	{
		spiketimer += dt;
		if ((spiketimer >= 5)&&(spiketimer<=7))
		{
			if (spikeypos<=0)
				spikeypos += 10 * dt;
		}
		if (spiketimer > 7)
		{
			if (spikeypos >-1.5)
				spikeypos -= 15 * dt;
		}
	}

	//Game over
	if (player.currentHealth <= 0)
	{
		nextscene = 7;
		PlaySound(L"Sound//gameover.wav", NULL, SND_FILENAME | SND_ASYNC);
	}

	//Win
	if ((win==1)&&(entities.size()==0))
	{
		timer += dt;
		if (timer >= 3)
		{
			nextscene = 7;
			Application::SetWin(1);
		}
	}

	//Tutorial
	if (stage == 0)
	{
		if ((Application::IsKeyPressed('W'))|| (Application::IsKeyPressed('A')) || (Application::IsKeyPressed('S')) || (Application::IsKeyPressed('D')))
		{
			keyused += dt;
		}
	}
	else if (stage == 1)
	{
		if ((Application::IsKeyPressed(VK_SHIFT))&&((Application::IsKeyPressed('W')) || (Application::IsKeyPressed('A')) || (Application::IsKeyPressed('S')) || (Application::IsKeyPressed('D'))))
		{
			keyused += dt;
		}
	}
	if (starttimer2 == 1)
	{
		timer2 += dt;
	}

}
void SceneGame::UpdateEnemyMovement(double dt)
{
	//For enemies
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getType() == 'E') {
			entities[i]->alert(entities);
			entities[i]->move(Vector3(camera.position.x, 0, camera.position.z), dt, hitboxes, entities, camera.getPlayerHitbox());
		}
	}
}
void SceneGame::EnemyAttack(double dt)
{
	//For enemies
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getType() == 'E') {
			entities[i]->attack(Vector3(camera.position.x, 0, camera.position.z), camera.getPlayerRadius(), player, dt);
		}
	}
}
float SceneGame::DistBetweenPoints(float x1, float z1, float x2, float z2)
{
	float distX = x1 - x2;
	float distZ = z1 - z2;
	float dist = 0.0f;
	dist = sqrt((distX * distX) + (distZ * distZ));
	return dist;
}

void SceneGame::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneGame::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(x, y, 1);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneGame::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float offset = 0.f;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(offset, 0, i * 0.01f); //slightly increase z val
		offset += (textSpacing[(int)(text[i])] / (float)textMaxWidth);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}
void SceneGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float offset = 0.f;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + offset, 0.5f, 0);
		offset += textSpacing[(int)(text[i])] / (float)textMaxWidth;
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
}
void SceneGame::RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y, Vector3 rotationAxis, float angle)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizeX, sizeY, 1);
	modelStack.Rotate(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z); //testing
	RenderMesh(mesh, false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
void SceneGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	/*if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}*/

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();	

	RenderSkybox();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(300, 300, 300);
	RenderMesh(meshList[GEO_GROUND], true);
	modelStack.PopMatrix();

	//for enemies
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->getType() == 'E') {
			modelStack.PushMatrix();
			modelStack.Translate(entities[i]->getPosition().x, 0, entities[i]->getPosition().z);
			//Health bar
			modelStack.PushMatrix();
			Vector3 targetVector = Vector3(camera.position.x, 0, camera.position.z) - Vector3(entities[i]->getPosition().x, 0, entities[i]->getPosition().z);
			targetVector = targetVector.Normalized();
			float healthbarFacing = acosf(Vector3(0,0,1).Dot(targetVector));
			if (targetVector.x > 0) {
				healthbarFacing = Math::RadianToDegree(healthbarFacing);
			}
			else {
				healthbarFacing = -Math::RadianToDegree(healthbarFacing);
			}
			modelStack.Rotate(healthbarFacing,0,1,0);
			modelStack.PushMatrix();
			modelStack.Translate(0, 6.3, 0);
			modelStack.Scale(3, 0.7, 1);
			RenderMesh(meshList[GEO_BLACK], false);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			float length = (float(entities[i]->getcurrenthealth()) / float(entities[i]->getmaxhealth())) * 3;
			modelStack.Translate(-(3-length)/2, 6.3, 0.01);
			modelStack.Scale(length, 0.7, 1);
			RenderMesh(meshList[GEO_RED], false);
			modelStack.PopMatrix();
			modelStack.PopMatrix();

			modelStack.Rotate(entities[i]->getFacing(), 0, 1, 0);
			modelStack.Scale(0.35, 0.35, 0.35);
			if (entities[i]->getName() == "BasicMelee") {
				RenderMesh(meshList[GEO_ENEMY1], false);
			}
			else if (entities[i]->getName() == "Watcher") {
				RenderMesh(meshList[GEO_ENEMY2], false);
			}
			else if (entities[i]->getName() == "Boss") {
				bombPos = entities[i]->getPosition();
				RenderMesh(meshList[GEO_BOSS], false);
			}
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, -60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SKYSCRAPER_A], true);
	modelStack.PopMatrix();
	//Hitbox(-60, 29, -60, 24, 58, 24);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -70);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_E], true);
	modelStack.PopMatrix();
	//Hitbox(0, 9, -70, 32, 18, 20);

	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SKYSCRAPER_F], true);
	modelStack.PopMatrix();
	//Hitbox(-60, 40, 0, 24, 80, 24);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -20);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BIGHOUSE_A], true);
	modelStack.PopMatrix();
	//Hitbox(0, 17, -20, 40, 34, 24);

	modelStack.PushMatrix();
	modelStack.Translate(60, 0, -65);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BIGHOUSE_F], true);
	modelStack.PopMatrix();
	//Hitbox(60, 12, -65, 40, 28, 16);
	
	modelStack.PushMatrix();
	modelStack.Translate(60, 0, -15);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BIGHOUSE_G], true);
	modelStack.PopMatrix();
	//Hitbox(60, 20, -15, 32, 40, 32);
	//Hitbox(30.9, 4, -26.1, 0.6, 8, 0.6);
	//Hitbox(30.9, 4, -3.9, 0.6, 8, 0.6);
	//Hitbox(40, 9, -15, 20, 2, 24);

	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, 80);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_D], true);
	modelStack.PopMatrix();
	//Hitbox(-10, 12, 80, 16, 24, 16);
	//Hitbox(-10, 4, 70.5, 16, 8, 3);


	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, 60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_C], true);
	modelStack.PopMatrix();
	//Hitbox(-60, 13, 60, 16, 26, 16);
	//Hitbox(-56, 13, 51, 8, 26, 2);

	modelStack.PushMatrix();
	modelStack.Translate(-40, 0, 60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_F], true);
	modelStack.PopMatrix();
	//Hitbox(-40, 8, 60, 16, 16, 16);

	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 80);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_D], true);
	modelStack.PopMatrix();
	//Hitbox(10, 12, 80, 16, 24, 16);
	//Hitbox(10, 4, 70.5, 16, 8, 3);

	modelStack.PushMatrix();
	modelStack.Translate(60, 0, 60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SKYSCRAPER_E], true);
	modelStack.PopMatrix();
	//Hitbox(60, 25, 60, 24, 50, 24);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 160);
	modelStack.Scale(300, 20, 20);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -140);
	modelStack.Scale(300, 20, 20);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(159, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(310, 20, 20);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-140, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(300, 20, 20);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-45, 0, 10);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_LIGHTPOST], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-25, 0, -20);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_LIGHTPOST], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-45, 0, -50);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_LIGHTPOST], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(40, 0, 60);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_LIGHTPOST], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(30, 0, -60);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_LIGHTPOST], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-25, 0, 60);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_LIGHTPOST], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(30, 0, 60);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(12, 12, 12);
	RenderMesh(meshList[GEO_BENCH], true);
	modelStack.PopMatrix();
	//Hitbox(30, 10, 60, 10, 20, 5);


	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.PushMatrix();
	//revolve around cam
	modelStack.Rotate(pitch, rightvector.x, rightvector.y, rightvector.z);
	modelStack.Rotate(yaw, 0, 1, 0);
	modelStack.Translate(1, -1.25, -4);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_GUN], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();


	if (tutorial == 0)//Render tutorial
		RenderTutorial();
	else if (tutorial==1)//Render Bomb and enemies
		RenderBomb();

	//Render Bullet
	for (int i = 0; i < bulletVector.size(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bulletVector[i].bulletHitbox.posX, bulletVector[i].bulletHitbox.posY, bulletVector[i].bulletHitbox.posZ);
		modelStack.Scale(bulletVector[i].bulletHitbox.sizeX, bulletVector[i].bulletHitbox.sizeY, bulletVector[i].bulletHitbox.sizeZ);
		RenderMesh(meshList[GEO_SPHERE], false);
		modelStack.PopMatrix();
	}

	//RenderHUD
	RenderHUD();

	if ((bombspawn==3)&&(win==0))
		RenderBoss();

	//render particle (bullet hit indicator)
	for (int i = 0; i < particles.size(); i++) {
		modelStack.PushMatrix();
		modelStack.Translate(particles[i].position.x, particles[i].position.y, particles[i].position.z);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();
	}

	//Render bomb indicator
	Vector3 viewTarget = (camera.target - camera.position).Normalized();
	viewTarget.y = 0;
	Vector3 viewBomb = (bombPos - camera.position).Normalized();
	viewBomb.y = 0;
	float arrowAngle;
	arrowAngle = atan2(viewBomb.z * viewTarget.x - viewBomb.x * viewTarget.z, viewBomb.x * viewTarget.x + viewBomb.z * viewTarget.z);
	arrowAngle = -Math::RadianToDegree(arrowAngle);
	RenderImageOnScreen(meshList[GEO_BOMBARROW], Color(1, 1, 1), 10, 10, 40, 5, Vector3(0, 0, 1), arrowAngle);
}

void SceneGame::RenderBomb()
{
	if (bombrand1 == 0)
	{
		bomb = rand() % 9 + 1;
		bombrand1++;
	}
	if (bombrand2 == 0)
	{
		bomb2 = rand() % 9 + 1;
		if (bomb2!=bomb)
			bombrand2++;
	}
	if (bombrand3 == 0)
	{
		bomb3 = rand() % 9 + 1;
		if ((bomb3 != bomb)&&(bomb3!=bomb2))
			bombrand3++;
	}
	if (check == 0)
	{
		int a = rand() % 3 + 4;
		minigamesused[0] = a;
		check++;
	}
	if (check == 1)
	{
		int a = rand() % 3 + 4;
		if (a != minigamesused[0])
		{
			minigamesused[1] = a;
			check++;
		}
	}
	if(check == 2)
	{
		int a = rand() % 3 + 4;
		if ((a != minigamesused[0])&& (a != minigamesused[1]))
		{
			minigamesused[2] = a;
			check++;
		}
	}
	if (bombspawn < 3)
	{
		if (bombspawn == 0)
			currentbomb = bomb;
		else if (bombspawn == 1)
			currentbomb = bomb2;
		else
			currentbomb = bomb3;
		if (currentbomb==1)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(90, Vector3(78, 0, -6), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.f));
					entities.push_back(new BasicMelee(90, Vector3(78, 0, -16), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.25f));
					entities.push_back(new BasicMelee(90, Vector3(78, 0, -11), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(0, Vector3(68, 0, 2), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(0, Vector3(73, 0, 2), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(90, Vector3(78, 0, -21), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(90, Vector3(78, 0, -26), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(76.1, 3, -11);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(76.1, 3, -11);
			if ((camera.position.x > 76.1) && (camera.position.x < 80) && (camera.position.z > -13) && (camera.position.z < -9))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 1)
						bomb = 0;
					else if (bomb2 == 1)
						bomb2 = 0;
					else if (bomb3 == 1)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else 
				crosshairenabled = 1;
		}
		if (currentbomb == 2)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(0, Vector3(67, 0, -54), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.f));
					entities.push_back(new BasicMelee(0, Vector3(72, 0, -54), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.25f));
					entities.push_back(new BasicMelee(0, Vector3(62, 0, -54), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(0, Vector3(57, 0, -54), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(0, Vector3(52, 0, -54), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(0, Vector3(47, 0, -54), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(0, Vector3(42, 0, -54), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(62, 3, -56.9);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(62, 3, -56.9);
			if ((camera.position.x > 60) && (camera.position.x < 64) && (camera.position.z > -56.9) && (camera.position.z < -53))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 2)
						bomb = 0;
					else if (bomb2 == 2)
						bomb2 = 0;
					else if (bomb3 == 2)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
		if (currentbomb == 3)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(270, Vector3(-18, 0, -69), Vector3(0, 0, 1), Vector3(-1, 0, 0), 0.f));
					entities.push_back(new BasicMelee(270, Vector3(-18, 0, -74), Vector3(0, 0, 1), Vector3(-1, 0, 0), 0.25f));
					entities.push_back(new BasicMelee(270, Vector3(-18, 0, -64), Vector3(0, 0, 1), Vector3(-1, 0, 0), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(0, Vector3(-13, 0, -59), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(0, Vector3(-8, 0, -59), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(180, Vector3(-13, 0, -82), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(180, Vector3(-8, 0, -82), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(-16.1, 3, -67);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(-16.1, 3, -67);
			if ((camera.position.x > -20) && (camera.position.x < -16.1) && (camera.position.z > -69) && (camera.position.z < -65))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 3)
						bomb = 0;
					else if (bomb2 == 3)
						bomb2 = 0;
					else if (bomb3 == 3)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
		if (currentbomb == 4)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(270, Vector3(-74, 0, -69), Vector3(0, 0, 1), Vector3(-1, 0, 0), 0.f));
					entities.push_back(new BasicMelee(270, Vector3(-74, 0, -59), Vector3(0, 0, 1), Vector3(-1, 0, 0), 0.25f));
					entities.push_back(new BasicMelee(270, Vector3(-74, 0, -64), Vector3(0, 0, 1), Vector3(-1, 0, 0), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(270, Vector3(-74, 0, -54), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(270, Vector3(-74, 0, -49), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(180, Vector3(-70, 0, -75), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(180, Vector3(-65, 0, -75), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(-72.1, 3, -64);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(-72.1, 3, -64);
			if ((camera.position.x > -76) && (camera.position.x < -72.1) && (camera.position.z > -66) && (camera.position.z < -62))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 4)
						bomb = 0;
					else if (bomb2 == 4)
						bomb2 = 0;
					else if (bomb3 == 4)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
		if (currentbomb == 5)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(0, Vector3(-53, 0, 14), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.f));
					entities.push_back(new BasicMelee(0, Vector3(-58, 0, 14), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.25f));
					entities.push_back(new BasicMelee(0, Vector3(-63, 0, 14), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(0, Vector3(-68, 0, 14), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(270, Vector3(-73, 0, 10), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(90, Vector3(-45, 0, 10), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(90, Vector3(-45, 0, 5), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(-53, 3, 12.1);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3 (-53, 3, 12.1);
			if ((camera.position.x > -55) && (camera.position.x < -51) && (camera.position.z > 12.1) && (camera.position.z < 16))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 5)
						bomb = 0;
					else if (bomb2 == 5)
						bomb2 = 0;
					else if (bomb3 == 5)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
		if (currentbomb == 6)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(90, Vector3(-30, 0, 60), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.f));
					entities.push_back(new BasicMelee(90, Vector3(-30, 0, 65), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.25f));
					entities.push_back(new BasicMelee(90, Vector3(-30, 0, 55), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(0, Vector3(-35, 0, 70), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(0, Vector3(-40, 0, 70), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(180, Vector3(-35, 0, 50), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(180, Vector3(-40, 0, 50), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(-31.9, 3, 60);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(-31.9, 3, 60);
			if ((camera.position.x > -31.9) && (camera.position.x < -28) && (camera.position.z > 58) && (camera.position.z < 62))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 6)
						bomb = 0;
					else if (bomb2 == 6)
						bomb2 = 0;
					else if (bomb3 == 6)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
		if (currentbomb == 7)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(0, Vector3(7, 0, 90), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.f));
					entities.push_back(new BasicMelee(0, Vector3(12, 0, 90), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.25f));
					entities.push_back(new BasicMelee(0, Vector3(2, 0, 90), Vector3(0, 0, 1), Vector3(0, 0, 1), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(0, Vector3(-3, 0, 90), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(0, Vector3(-8, 0, 90), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(90, Vector3(20, 0, 85), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(90, Vector3(20, 0, 80), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(7, 3, 88.1);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(7, 3, 88.1);
			if ((camera.position.x > 5) && (camera.position.x < 9) && (camera.position.z > 88.1) && (camera.position.z < 92))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 7)
						bomb = 0;
					else if (bomb2 == 7)
						bomb2 = 0;
					else if (bomb3 == 7)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
		if (currentbomb == 8)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(180, Vector3(56, 0, 46), Vector3(0, 0, 1), Vector3(0, 0, -1), 0.f));
					entities.push_back(new BasicMelee(180, Vector3(51, 0, 46), Vector3(0, 0, 1), Vector3(0, 0, -1), 0.25f));
					entities.push_back(new BasicMelee(180, Vector3(61, 0, 46), Vector3(0, 0, 1), Vector3(0, 0, -1), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(180, Vector3(66, 0, 46), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(180, Vector3(71, 0, 46), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(270, Vector3(46, 0, 51), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(270, Vector3(46, 0, 56), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(56, 3, 47.9);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(56, 3, 47.9);
			if ((camera.position.x > 54) && (camera.position.x < 58) && (camera.position.z > 44) && (camera.position.z < 47.9))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 8)
						bomb = 0;
					else if (bomb2 == 8)
						bomb2 = 0;
					else if (bomb3 == 8)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
		if (currentbomb == 9)
		{
			if (spawn == 0)
			{
				if (bombspawn <= 2)
				{
					entities.push_back(new BasicMelee(90, Vector3(22, 0, -15), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.f));
					entities.push_back(new BasicMelee(90, Vector3(22, 0, -10), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.25f));
					entities.push_back(new BasicMelee(90, Vector3(22, 0, -20), Vector3(0, 0, 1), Vector3(1, 0, 0), 0.5f));
					if (bombspawn >= 1)
					{
						entities.push_back(new BasicMelee(90, Vector3(22, 0, -25), Vector3(0, 0, 1), Vector3(0, 0, 0), 0.f));
						entities.push_back(new Watcher(90, Vector3(22, 0, -30), Vector3(0, 0, 1)));
						if (bombspawn == 2)
						{
							entities.push_back(new Watcher(180, Vector3(17, 0, -35), Vector3(0, 0, 1)));
							entities.push_back(new Watcher(180, Vector3(12, 0, -35), Vector3(0, 0, 1)));
						}
					}
				}
				spawn++;
			}
			modelStack.PushMatrix();
			modelStack.Translate(20.1, 3, -15);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_BOMB], true);
			modelStack.PopMatrix();
			bombPos = Vector3(20.1, 3, -15);
			if ((camera.position.x > 20.1) && (camera.position.x < 24) && (camera.position.z > -17) && (camera.position.z < -13))
			{
				crosshairenabled = 0;
				RenderTextOnScreen(meshList[GEO_TEXT], "Press F to start defusing", Color(1, 0, 0), 4, 22, 28);
				if (Application::IsKeyPressed('F'))
				{
					spawn--;
					nextscene = rand() % 3 + 4;
					if (bombspawn == 0)
						nextscene = minigamesused[0];
					else if (bombspawn == 1)
						nextscene = minigamesused[1];
					else if (bombspawn == 2)
						nextscene = minigamesused[2];
					if (bomb == 9)
						bomb = 0;
					else if (bomb2 == 9)
						bomb2 = 0;
					else if (bomb3 == 9)
						bomb3 = 0;
					bombspawn++;
				}
			}
			else
				crosshairenabled = 1;
		}
	}
	else 
		crosshairenabled = 1;
}

void SceneGame::RenderHUD()
{
	crosshair = Application::GetCrosshair();
	std::ostringstream ss,sss,ssss,ss1;
	ss.precision(4);
	ss << "FPS: " << FPS;
	sss << player.currentHealth;
	ss1 << player.currentAmmo << "/" << player.maxAmmo;
	ssss << "Scams: " << bombspawn << "/3";
	if (Application::GetStamina() < 5)
	{
		RenderMeshOnScreen(meshList[GEO_BLACK], 40, 25, 16, 1);
		float length = (float(Application::GetStamina()) / 5)*16;
		RenderMeshOnScreen(meshList[GEO_BLUE], 32+(length/2), 25, length, 1);
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0.6, 0.1), 3, 0, 57); //FPS
	RenderImageOnScreen(meshList[GEO_HEALTH], Color(1, 1, 1), 7, 7, 4, 14); //Health
	if (player.currentHealth > 30)
		RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(0, 0.6, 0.1), 7, 8, 10);
	else
		RenderTextOnScreen(meshList[GEO_TEXT], sss.str(), Color(1, 0, 0), 7, 8, 10);
	RenderImageOnScreen(meshList[GEO_AMMO], Color(1, 1, 1), 2, 8, 4, 5);//Ammo
	if (player.currentAmmo>player.maxAmmo*0.3)
		RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 0.6, 0.1), 7, 8, 0);
	else
		RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(1, 0, 0), 7, 8, 0);
	if (crosshairenabled == 1) //Crosshair
	{
		if (crosshair == 1)
			RenderImageOnScreen(meshList[GEO_REDCROSSHAIR], Color(1, 1, 1), 3, 3, 40, 30);
		else if (crosshair == 2)
			RenderImageOnScreen(meshList[GEO_BLUECROSSHAIR], Color(1, 1, 1), 3, 3, 40, 30);
		else if (crosshair == 3)
			RenderImageOnScreen(meshList[GEO_GREENCROSSHAIR], Color(1, 1, 1), 3, 3, 40, 30);
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ssss.str(), Color(0, 0.6, 0.1), 3, 67, 57); //Scams
	
}

void SceneGame::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	static int OFFSET = 499;
	modelStack.PushMatrix();
	modelStack.Translate(OFFSET, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(-OFFSET, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -OFFSET);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, OFFSET);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, OFFSET, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	modelStack.Translate(0, -OFFSET, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneGame::RenderBoss()
{
	if (spikestart == 0)
	{
		entities.push_back(new Boss(0, Vector3(0, 0, 0), Vector3(0, 0, 1)));
		spikestart = 2;
	}
	if (spikestart == 2)
	{
		for (int i = 0; i < entities.size(); i++)
		{
			if (entities[i]->getName() == "Boss")
			{
				if (entities[i]->getPosition() != Vector3(0, 0, 0))
					spikestart = 1;
			}
		}
	}
	if ((spiketimer >= 1.6 + prevshot) && (bossshoot == 0))
	{
		bossshoot = 1;
		prevshot = spiketimer;
	}
	if ((spiketimer >= 4.7) && (spikelockon == 0))
		spikelockon = 1;
	if (spikelockon == 1)
	{
		PlaySound(L"Sound//spikesound.wav", NULL, SND_FILENAME | SND_ASYNC);
		spikexpos = camera.position.x;
		spikezpos = camera.position.z;
		spikelockon = 2;
	}
	if ((spiketimer >= 5)&&(spiketimer<=8))
	{
		modelStack.PushMatrix();
		modelStack.Translate(spikexpos, spikeypos, spikezpos);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_SPIKE], true);
		modelStack.PopMatrix();
		if ((camera.position.x >= spikexpos - 2.4) && (camera.position.x <= spikexpos + 2.4) && (camera.position.z >= spikezpos - 2.4) && (camera.position.z <= spikezpos + 2.4) && (camera.position.y < spikeypos + 5))
		{
			if (spikedmg == 0)
			{
				player.currentHealth -= 20;
				spikedmg = 1;
			}
		}
		else
			spikedmg = 0;
	}
	if (spiketimer > 8)
	{
		spikelockon = 0;
		spiketimer = 0;
		spikeypos = -1.5;
		bossshoot = 0;
		prevshot = 0;
	}
}

void SceneGame::RenderTutorial()
{
	if (stage == -2)
	{
		currentview = (camera.target - camera.position).Normalized();
		stage = -1;
	}
	else if (stage == -1)
	{
		if (keyused == 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use mouse to look around", Color(1, 0, 0), 3, 24, 53);
			if (currentview != (camera.target - camera.position).Normalized())
				keyused = 1;
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use mouse to look around", Color(0, 0.6, 0.1), 3, 24, 53);
			starttimer2 = 1;
		}
		if (timer2 > 1.5)
		{
			stage = 0;
			keyused = 0;
			starttimer2 = 0;
			timer2 = 0;
		}
	}
	else if (stage == 0)
	{
		if (keyused < 2)
			RenderTextOnScreen(meshList[GEO_TEXT], "Use W,A,S,D to move (2 sec)", Color(1, 0, 0), 3, 22, 53);
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use W,A,S,D to move (2 sec)", Color(0, 0.6, 0.1), 3, 22, 53);
			starttimer2 = 1;
		}
		if (timer2 > 1.5)
		{
			stage = 1;
			keyused = 0;
			starttimer2 = 0;
			timer2 = 0;
		}
	}
	else if (stage == 1)
	{
		if (keyused < 2)
			RenderTextOnScreen(meshList[GEO_TEXT], "Use SHIFT and W,A,S,D to sprint (2 sec)", Color(1, 0, 0), 3, 18, 53);
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use SHIFT and W,A,S,D to sprint (2 sec)", Color(0, 0.6, 0.1), 3, 18, 53);
			starttimer2 = 1;
		}
		if (timer2 > 1.5)
		{
			stage = 2;
			keyused = 0;
			starttimer2 = 0;
			timer2 = 0;
		}
	}
	else if (stage == 2)
	{
		if (keyused == 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use SPACE to jump", Color(1, 0, 0), 3, 28, 53);
			if (Application::IsKeyPressed(VK_SPACE))
				keyused = 1;
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use SPACE to jump", Color(0, 0.6, 0.1), 3, 28, 53);
			starttimer2 = 1;
		}
		if (timer2 > 1.5)
		{
			stage = 3;
			keyused = 0;
			starttimer2 = 0;
			timer2 = 0;
		}
	}
	else if (stage == 3)
	{
		if (keyused == 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use Left Click to shoot", Color(1, 0, 0), 3, 27, 53);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use Left Click to shoot", Color(0, 0.6, 0.1), 3, 27, 53);
			starttimer2 = 1;
		}
		if (timer2 > 1.5)
		{
			stage = 4;
			keyused = 0;
			starttimer2 = 0;
			timer2 = 0;
		}
	}
	else if (stage == 4)
	{
		if (keyused == 0)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use R to reload", Color(1, 0, 0), 3, 30, 53);
		}
		else
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Use R to reload", Color(0, 0.6, 0.1), 3, 30, 53);
			starttimer2 = 1;
		}
		if (timer2 > 1.5)
		{
			stage = 5;
			keyused = 0;
			starttimer2 = 0;
			timer2 = 0;
		}
	}
	else if (stage == 5)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "You are now prepared.", Color(0, 0.6, 0.1), 3, 28, 53);
		RenderTextOnScreen(meshList[GEO_TEXT], "Defeat all the scammers and stop the scams.", Color(0, 0.6, 0.1), 3, 16, 50);
		RenderTextOnScreen(meshList[GEO_TEXT], "Good luck Officer.", Color(0, 0.6, 0.1), 3, 30, 47);
		starttimer2 = 1;
		if (timer2 > 5)
		{
			stage = 6;
			keyused = 0;
			starttimer2 = 0;
			timer2 = 0;
			tutorial = 1;
			player.currentAmmo = 7;
			camera.Init(Vector3(0, 4.5, 5.5), Vector3(0, 4.5, 4.5), Vector3(0, 1, 0));
		}
	}
}

int SceneGame::NextScene()
{
	if (nextscene == 4)
	{
		nextscene = 3;
		return 4; //wire game
	}
	else if (nextscene == 5)
	{
		nextscene = 3;
		return 5; //number game
	}
	else if (nextscene == 6)
	{
		nextscene = 3;
		return 6; //copy game
	}
	else if (nextscene == 7)
	{
		nextscene = 3;
		return 7; //game over
	}
	return 0; //not switching
}

void SceneGame::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}