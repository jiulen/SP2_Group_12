#include "SceneGame.h" 
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

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 19.99, 0);
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

	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(46, 8, 46);
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

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//tron_ft.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//tron_bk.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//tron_up.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//tron_lf.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//tron_rt.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//tron_dn.tga");
	meshList[GEO_GROUND] = MeshBuilder::GenerateRepeatingQuad("bottom", Color(1, 1, 1), 20.f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//whitebrickfloor.tga");
	meshList[GEO_ENEMY1] = MeshBuilder::GenerateOBJMTL("enemy1", "OBJ//basicCharacter.obj", "OBJ//basicCharacter.obj.mtl");
	meshList[GEO_ENEMY1]->textureID = LoadTGA("Image//skin_robot.tga");
	meshList[GEO_BOMB] = MeshBuilder::GenerateOBJMTL("bomb", "OBJ//clipA.obj", "OBJ//clipA.mtl");
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
	//changing
	hitboxes.clear();

	FPS = 0;
	bLightEnabled = true;

	//Building hitboxes
	hitboxes.push_back(Hitbox(-60, 29, -60, 24, 58, 24));
	hitboxes.push_back(Hitbox(0, 9, -70, 32, 18, 20));
	hitboxes.push_back(Hitbox(-60, 40, 0, 24, 80, 24));
	hitboxes.push_back(Hitbox(0, 17, -20, 40, 34, 24));
	hitboxes.push_back(Hitbox(60, 12, -65, 40, 28, 16));
	hitboxes.push_back(Hitbox(60, 20, -15, 32, 40, 32));
	hitboxes.push_back(Hitbox(30.9, 4, -26.1, 0.6, 8, 0.6));
	hitboxes.push_back(Hitbox(30.9, 4, -3.9, 0.6, 8, 0.6));
	hitboxes.push_back(Hitbox(40, 9, -15, 20, 2, 24));
	hitboxes.push_back(Hitbox(0, 13, 78.5, 36, 26, 19));
	hitboxes.push_back(Hitbox(-60, 13, 60, 16, 26, 16));
	hitboxes.push_back(Hitbox(-56, 13, 51, 8, 26, 2));
	hitboxes.push_back(Hitbox(-40, 8, 60, 16, 16, 16));
	hitboxes.push_back(Hitbox(10, 12, 80, 16, 24, 16));
	hitboxes.push_back(Hitbox(10, 4, 70.5, 16, 8, 3));
	hitboxes.push_back(Hitbox(60, 25, 60, 24, 50, 24));
	//test hitbox
	hitboxes.push_back(Hitbox(5, 1, 5, 5, 2, 5));

	//Enemies for Assignment 2
	enemy1X = 60;
	enemy1Z = 2;
	chase = false;
	characterFacing = 0;
	enemyVector = Vector3(0, 0, 1);
	targetVector = Vector3(0, 0, 1);
}

void SceneGame::Update(double dt)
{
	//mouse inputs
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		//use tasklist as button
		float BUTTON_LEFT = 0;
		float BUTTON_RIGHT = 30;
		float BUTTON_BOTTOM = 38;
		float BUTTON_TOP = 55;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x/w * 80.f; //convert (0,800) to (0,80)
		float posY = (h-y)/h * 60.f; //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
		if (posX > BUTTON_LEFT && posX < BUTTON_RIGHT && posY > BUTTON_BOTTOM && posY < BUTTON_TOP)
		{
			std::cout << "Hit!" << std::endl;
		}
		else
		{
			std::cout << "Miss!" << std::endl;
		}

	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
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

	if (DistBetweenPoints(camera.position.x, camera.position.z, enemy1X, enemy1Z) <= 20)
	{
		chase = true;
	}

	if (chase == true)
	{
		if (DistBetweenPoints(camera.position.x, camera.position.z, enemy1X, enemy1Z) > 20)//if enemy is out of range
		{
			if (enemy1X < camera.position.x)
			{
				enemy1X += (float)(10 * dt);
			}
			if (enemy1X > camera.position.x)
			{
				enemy1X -= (float)(10 * dt);
			}
			if (enemy1Z < camera.position.z)
			{
				enemy1Z += (float)(10 * dt);
			}
			if (enemy1Z > camera.position.z)
			{
				enemy1Z -= (float)(10 * dt);
			}
		}
		targetVector = Vector3(camera.position.x, 0, camera.position.z) - Vector3(enemy1X, 0, enemy1Z);
		targetVector = targetVector.Normalized();
		characterFacing = acosf(enemyVector.Dot(targetVector));
		characterFacing = Math::RadianToDegree(characterFacing);
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
bool SceneGame::CollisionLineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// calculate the direction of the lines
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) return true;
	return false;
}
bool SceneGame::CollisionLineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh)
{
	// check if the line has hit any of the rectangle's sides
    // uses the Line/Line function below
	bool left = CollisionLineLine(x1, y1, x2, y2, rx - rw * 0.5f, ry - rh * 0.5f, rx - rw * 0.5f, ry + rh * 0.5f);
	bool right = CollisionLineLine(x1, y1, x2, y2, rx + rw * 0.5f, ry - rh * 0.5f, rx + rw * 0.5f, ry - rh * 0.5f);
	bool top = CollisionLineLine(x1, y1, x2, y2, rx - rw * 0.5f, ry - rh * 0.5f, rx + rw * 0.5f, ry - rh * 0.5f);
	bool bottom = CollisionLineLine(x1, y1, x2, y2, rx - rw * 0.5f, ry + rh * 0.5f, rx + rw * 0.5f, ry + rh * 0.5f);

	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) return true;
	return false;
}

bool SceneGame::CollisionPointCircle(float px, float py, float cx, float cy, float r)
{
	// get distance between the point and circle's center using the Pythagorean Theorem
	float distX = px - cx;
	float distY = py - cy;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the circle's radius the point is inside!
	if (distance <= r) return true;
	return false;
}

bool SceneGame::CollisionLinePoint(float x1, float y1, float x2, float y2, float px, float py)
{
	// get distance from the point to the two ends of the line
	float d1 = DistBetweenPoints(px, py, x1, y1);
	float d2 = DistBetweenPoints(px, py, x2, y2);

	// get the length of the line
	float lineLen = DistBetweenPoints(x1, y1, x2, y2);

	// since floats are so minutely accurate, add a little buffer zone that will give collision
	float buffer = 0.1;    // higher # = less accurate

	// if the two distances are equal to the line's length, the point is on the line!
	// note we use the buffer here to give a range,  rather than one #
	if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) return true;
	return false;
}

bool SceneGame::CollisionLineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r)
{
	// if either end INSIDE the circle return true
	bool inside1 = CollisionPointCircle(x1, y1, cx, cy, r);
	bool inside2 = CollisionPointCircle(x2, y2, cx, cy, r);
	if (inside1 || inside2) return true;

	// get length of the line
	float len = DistBetweenPoints(x1, y1, x2, y2);

	// get dot product of the line and circle
	float dot = (((cx - x1) * (x2 - x1)) + ((cy - y1) * (y2 - y1))) / pow(len, 2.f);

	// find the closest point on the line
	float closestX = x1 + (dot * (x2 - x1));
	float closestY = y1 + (dot * (y2 - y1));

	// if this point is on the line segment keep going, but if not, return false
	bool onSegment = CollisionLinePoint(x1, y1, x2, y2, closestX, closestY);
	if (!onSegment) return false;

	// get distance to closest point
	float distance = DistBetweenPoints(closestX, closestY, cx, cy);

	if (distance <= r) return true;
	return false;
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
void SceneGame::RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y)
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
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
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
	}

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();	

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(enemy1X, 0, enemy1Z);
	modelStack.Rotate(characterFacing, 0, 1, 0);
	modelStack.Scale(0.35, 0.35, 0.35);
	RenderMesh(meshList[GEO_ENEMY1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, -60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SKYSCRAPER_A], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(-60, 29, -60, 24, 58, 24));

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -70);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_E], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(0, 9, -70, 32, 18, 20));

	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SKYSCRAPER_F], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(-60, 40, 0, 24, 80, 24));

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -20);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BIGHOUSE_A], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(0, 17, -20, 40, 34, 24));

	modelStack.PushMatrix();
	modelStack.Translate(60, 0, -65);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BIGHOUSE_F], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(60, 12, -65, 40, 28, 16));
	
	modelStack.PushMatrix();
	modelStack.Translate(60, 0, -15);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_BIGHOUSE_G], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(60, 20, -15, 32, 40, 32));
	//hitboxes.push_back(Hitbox(30.9, 4, -26.1, 0.6, 8, 0.6));
	//hitboxes.push_back(Hitbox(30.9, 4, -3.9, 0.6, 8, 0.6));
	//hitboxes.push_back(Hitbox(40, 9, -15, 20, 2, 24));

	modelStack.PushMatrix();
	modelStack.Translate(-10, 0, 80);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_D], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(0, 13, 78.5, 36, 26, 19));

	modelStack.PushMatrix();
	modelStack.Translate(-60, 0, 60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_C], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(-60, 13, 60, 16, 26, 16));
	//hitboxes.push_back(Hitbox(-56, 13, 51, 8, 26, 2));

	modelStack.PushMatrix();
	modelStack.Translate(-40, 0, 60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_F], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(-40, 8, 60, 16, 16, 16));

	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 80);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SMALLHOUSE_D], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(10, 12, 80, 16, 24, 16));
	//hitboxes.push_back(Hitbox(10, 4, 70.5, 16, 8, 3));

	modelStack.PushMatrix();
	modelStack.Translate(60, 0, 60);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_SKYSCRAPER_E], true);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(60, 25, 60, 24, 50, 24));

	//Render Bomb
	RenderBomb(0);

	//test floor collision
	modelStack.PushMatrix();
	modelStack.Translate(5, 1, 5);
	modelStack.Scale(5, 2, 5);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();
	//hitboxes.push_back(Hitbox(5, 1, 5, 5, 2, 5));

	RenderSkybox();

	std::ostringstream ss;
	ss.str("");
	ss.precision(4);
	//ss << "FPS: " << FPS;
	ss << "direction: " << characterFacing;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 0, 0); //FPS
}

void SceneGame::RenderBomb(int a)
{
	if (bombrand1 == 0)
	{
		bomb = rand() % 9 + 1;
		bombrand1++;
	}
	if (bombrand2 == 0)
	{
		bomb2 = rand() % 9 + 1;
		if (bomb2==bomb)
			bomb2 = rand() % 9 + 1;
		else 
			bombrand2++;
	}
	if (bombrand3 == 0)
	{
		bomb3 = rand() % 9 + 1;
		if ((bomb3 == bomb)||(bomb3==bomb2))
			bomb3 = rand() % 9 + 1;
		else
			bombrand3++;
	}

	if ((bomb == 1)||(bomb2==1)||(bomb3==1))
	{
		modelStack.PushMatrix();
		modelStack.Translate(76.1, 3, -11);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 2) || (bomb2 == 2) || (bomb3 == 2))
	{
		modelStack.PushMatrix();
		modelStack.Translate(62, 3, -56.9);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 3) || (bomb2 == 3) || (bomb3 == 3))
	{
		modelStack.PushMatrix();
		modelStack.Translate(-16.1, 3, -67);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 4) || (bomb2 == 4) || (bomb3 == 4))
	{
		modelStack.PushMatrix();
		modelStack.Translate(-72.1, 3, -64);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 5) || (bomb2 == 5) || (bomb3 == 5))
	{
		modelStack.PushMatrix();
		modelStack.Translate(-53, 3, 12.1);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 6) || (bomb2 == 6) || (bomb3 == 6))
	{
		modelStack.PushMatrix();
		modelStack.Translate(-31.9, 3, 60);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 7) || (bomb2 == 7) || (bomb3 == 7))
	{
		modelStack.PushMatrix();
		modelStack.Translate(7, 3, 88.1);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 8) || (bomb2 == 8) || (bomb3 == 8))
	{
		modelStack.PushMatrix();
		modelStack.Translate(56, 3, 47.9);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
	if ((bomb == 9) || (bomb2 == 9) || (bomb3 == 9))
	{
		modelStack.PushMatrix();
		modelStack.Translate(20.1, 3, -15);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BOMB], true);
		modelStack.PopMatrix();
	}
}

void SceneGame::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	int OFFSET = 499;
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
	modelStack.Rotate(180, 0, 0, 1);
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

int SceneGame::NextScene()
{
	return nextscene;
}

void SceneGame::Exit()
{
	// Cleanup VBO here
	delete meshList[GEO_TEXT];
	delete meshList[GEO_LEFT];
	delete meshList[GEO_RIGHT];
	delete meshList[GEO_TOP];
	delete meshList[GEO_BOTTOM];
	delete meshList[GEO_FRONT];
	delete meshList[GEO_BACK];
	delete meshList[GEO_BOMB];
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}