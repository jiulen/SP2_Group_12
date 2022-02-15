#include "SceneMainMenu.h" 
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


SceneMainMenu::SceneMainMenu()
{
}

SceneMainMenu::~SceneMainMenu()
{
}

void SceneMainMenu::Init()
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
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//mesh.tga");
	meshList[GEO_ENEMY1] = MeshBuilder::GenerateOBJMTL("enemy1", "OBJ//basicCharacter.obj", "OBJ//basicCharacter.obj.mtl");
	meshList[GEO_ENEMY1]->textureID = LoadTGA("Image//skin_robot.tga");
	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("greenbackground", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");
	meshList[GEO_TITLE] = MeshBuilder::GenerateQuad("titleamogus", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TITLE]->textureID = LoadTGA("Image//title.tga");
	meshList[GEO_PLAY] = MeshBuilder::GenerateQuad("pvzstart", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_PLAY]->textureID = LoadTGA("Image//play.tga");
	meshList[GEO_OPTIONS] = MeshBuilder::GenerateQuad("settings", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_OPTIONS]->textureID = LoadTGA("Image//settings.tga");

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
	voting = false, lightsOn = true, impostor = false;
	voted = 0;
	redVotes = 0, blackVotes = 0, cyanVotes = 0, orangeVotes = 0, noVotes = 0, mostVotes = 0;
	redAlive = true, blackAlive = true, cyanAlive = true, orangeAlive = true;
	task1 = false, task2 = false, task3 = false, iTask1 = false, iTask2 = false;

	FPS = 0;
	talk = "";
	talkTarget = "";
	bLightEnabled = true;
	lightCutoff = 90, lightInner = 60;
	turnLightOff = false;
	redRotateX = 0, redRotateY = 0;
	//constant
	interactRange = 10.f;

	//All characters
	headBodyScaleX = 3, headBodyScaleZ = 3;
	bodyScaleY = 0.5;
	bodyBottomScaleY = 1;
	headScaleY = 1.5;
	headTopScaleY = 3;
	visorScaleX = 1, visorScaleY = 1, visorScaleZ = 2;
	thighLegScaleX = 1.25f, thighLegScaleZ = 1.25f;
	thighScaleY = 0.75f;
	thighTopBottomScaleY = 1.25f;
	legScaleY = 1.25f;
	legTopScaleY = 1;

	//For 3 npcs (wont change)
	blackTranslateX = 0.f; blackTranslateZ = -25.5f;
	cyanTranslateX = -20.5f, cyanTranslateZ = 20.f;
	orangeTranslateX = 20.5f, orangeTranslateZ = 20.f;
}

void SceneMainMenu::Update(double dt)
{
	//mouse inputs
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		//use tasklist as button
		float BUTTON_LEFT = 10;
		float BUTTON_RIGHT = 40;
		float BUTTON_BOTTOM = 15;
		float BUTTON_TOP = 26;

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
			std::cout << "Options!" << std::endl;
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
	if (!redAlive) gameOver = 2;
	else if (!blackAlive && !cyanAlive && !orangeAlive) gameOver = 1;
	if (gameOver == 0) {
		if (!voting) {
			if (Application::IsKeyPressed('1'))
				glEnable(GL_CULL_FACE);
			if (Application::IsKeyPressed('2'))
				glDisable(GL_CULL_FACE);
			if (Application::IsKeyPressed('3'))
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
			if (Application::IsKeyPressed('4'))
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
			camera.Update(dt);
			static const float ROTATE_SPEED = 90.f;
			if (Application::IsKeyPressed(VK_RIGHT))
			{
				float yaw = -ROTATE_SPEED * static_cast<float>(dt);
				redRotateY += yaw;
			}
			if (Application::IsKeyPressed(VK_LEFT))
			{
				float yaw = ROTATE_SPEED * static_cast<float>(dt);
				redRotateY += yaw;
			}
			if (Application::IsKeyPressed('F')) { //interact
				//talk with black
				if (blackAlive && DistBetweenPoints(camera.position.x, camera.position.z, blackTranslateX, blackTranslateZ) <= 3 + interactRange) { //3 is radius of 2 characters
					if (lightsOn) talk = "Black: r u the impoter?";
					else talk = "Black: u cant see me";
					talkTarget = "black";
					if (!task1) task1 = true;
				}
				//talk with cyan
				else if (cyanAlive && DistBetweenPoints(camera.position.x, camera.position.z, cyanTranslateX, cyanTranslateZ) <= 3 + interactRange) { //3 is radius of 2 characters
					if (lightsOn) talk = "Cyan: u r sus";
					else talk = "Cyan: SUS!!1!1!!";
					talkTarget = "cyan";
					if (!task1) task1 = true;
				}
				//talk with orange
				else if (orangeAlive && DistBetweenPoints(camera.position.x, camera.position.z, orangeTranslateX, orangeTranslateZ) <= 3 + interactRange) { //3 is radius of 2 characters
					if (lightsOn) talk = "Orange: i m not imposter";
					else talk = "Orange: you r the imposter!!!";
					talkTarget = "orange";
					if (!task1) task1 = true;
				}
				//press emergency button
				if (DistBetweenPoints(camera.position.x, camera.position.z, 0, 0) <= 0.1 + 1.5 + interactRange) {
					if (lightsOn) {
						voting = true;
						voted = 0;
						redVotes = 0, blackVotes = 0, cyanVotes = 0, orangeVotes = 0, noVotes = 0;
						if (!task2) task2 = true;
					}
					else {
						//Error message
						talk = "Turn on the lights first.";
					}

				}
				//near switch , if light off, turn on
				if (!lightsOn && DistBetweenPoints(camera.position.x, camera.position.z, -47, -48.8) <= 1.5 + interactRange) {
					lightsOn = true;
					lightCutoff = 90.f;
					lightInner = 60.f;
					light[0].cosCutoff = cos(Math::DegreeToRadian(lightCutoff));
					light[0].cosInner = cos(Math::DegreeToRadian(lightInner));
					glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
					glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
				}
			}
		}
		//near knife, if impostor, drop knife, else, take knife
		static bool FPressed = false; //wont keep triggering while held down
		if (!FPressed && Application::IsKeyPressed('F')) {
			if (DistBetweenPoints(camera.position.x, camera.position.z, 35.85, 46.8) <= 1.5 + interactRange) {
				impostor = !impostor;
				if (!task3) task3 = true;
			}
			FPressed = true;
		}
		else if (FPressed && !Application::IsKeyPressed('F')) {
			FPressed = false;
		}
		if (Application::IsKeyPressed('E')) {
			//turn light off
			if (lightsOn && impostor && DistBetweenPoints(camera.position.x, camera.position.z, -47, -48.8) <= 1.5 + interactRange) {
				turnLightOff = true;
				lightsOn = false;
				if (!iTask1) iTask1 = true;
			}
		}
		if (turnLightOff && !lightsOn) {
			if (lightCutoff > 0.f) {
				//reduce light size
				float const SPEED = 90.f;
				lightCutoff -= SPEED * dt;
				if (lightCutoff < 0.f) {
					lightCutoff = 0.f;
				}
				lightInner -= SPEED * dt;
				if (lightInner < 0.f) {
					lightInner = 0.f;
				}
				light[0].cosCutoff = cos(Math::DegreeToRadian(lightCutoff));
				light[0].cosInner = cos(Math::DegreeToRadian(lightInner));
				glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
				glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
			}
			else {
				turnLightOff = false;
			}
		}
		static bool QPressed = false;
		if (!QPressed && Application::IsKeyPressed('Q')) {
			QPressed = true;
			//kill (k) - only can activate when targeting body (legs not counted) (hitbox is cube)
			if (impostor) {
				float characterScaleY = 3.8f;
				float characterPosY = 4.1f;
				float killRange = 15.f;
				Vector3 killTarget = camera.position + (camera.target - camera.position) * killRange;
				//kill black
				if (blackAlive) {
					if (CollisionLineCircle(camera.position.x, camera.position.z, killTarget.x, killTarget.z, blackTranslateX, blackTranslateZ, 1.5)) { //circle collsion
						if (CollisionLineRect(camera.position.y, camera.position.z, killTarget.y, killTarget.z, characterPosY, blackTranslateZ, characterScaleY, headBodyScaleZ)) { //rect collision
							//teleports to black pos and kills him
							float xChange = blackTranslateX - camera.position.x;
							float zChange = blackTranslateZ - camera.position.z;
							camera.target.x += xChange;
							camera.target.z += zChange;
							camera.position.x += xChange;
							camera.position.z += zChange;
							blackAlive = false;
							if (!iTask2) iTask2 = true;
						}
					}
				}
				//kill cyan
				if (cyanAlive) {
					if (CollisionLineCircle(camera.position.x, camera.position.z, killTarget.x, killTarget.z, cyanTranslateX, cyanTranslateZ, 1.5)) { //circle collsion
						if (CollisionLineRect(camera.position.y, camera.position.z, killTarget.y, killTarget.z, characterPosY, cyanTranslateZ, characterScaleY, headBodyScaleZ)) { //rect collision
							//teleports to cyan pos and kills him
							float xChange = cyanTranslateX - camera.position.x;
							float zChange = cyanTranslateZ - camera.position.z;
							camera.target.x += xChange;
							camera.target.z += zChange;
							camera.position.x += xChange;
							camera.position.z += zChange;
							cyanAlive = false;
							if (!iTask2) iTask2 = true;
						}
					}
				}
				//kill orange
				if (orangeAlive) {
					if (CollisionLineCircle(camera.position.x, camera.position.z, killTarget.x, killTarget.z, orangeTranslateX, orangeTranslateZ, 1.5)) { //circle collsion
						if (CollisionLineRect(camera.position.y, camera.position.z, killTarget.y, killTarget.z, characterPosY, orangeTranslateZ, characterScaleY, headBodyScaleZ)) { //rect collision
							//teleports to orange pos and kills him
							float xChange = orangeTranslateX - camera.position.x;
							float zChange = orangeTranslateZ - camera.position.z;
							camera.target.x += xChange;
							camera.target.z += zChange;
							camera.position.x += xChange;
							camera.position.z += zChange;
							orangeAlive = false;
							if (!iTask2) iTask2 = true;
						}
					}
				}
			}
		}
		else if (QPressed && !Application::IsKeyPressed('Q')) {
			QPressed = false;
		}
	}
	if (voting) {
		if (voted == 0) {
			for (int i = 0; i < 3; ++i) {
				int vote;
				bool valid = false;
				while (!valid) { //check no vote dead player
					vote = Math::RandIntMinMax(1, 5);
					if (vote == 1 || vote == 5) {
						valid = true;
					}
					else {
						switch (vote) {
						case 2:
							if (blackAlive) valid = true;
							else valid = false;
							break;
						case 3:
							if (cyanAlive) valid = true;
							else valid = false;
							break;
						case 4:
							if (orangeAlive) valid = true;
							else valid = false;
							break;
						}
					}
				}
				switch (vote) {
				case 1:
					redVotes++;
					break;
				case 2:
					blackVotes++;
					break;
				case 3:
					cyanVotes++;
					break;
				case 4:
					orangeVotes++;
					break;
				case 5:
					noVotes++;
					break;
				}
			}
			voted++;
		}
		if (voted == 1 && Application::IsKeyPressed('1')) {
			voted++;
			redVotes++;
		}
		if (voted == 1 && Application::IsKeyPressed('2')) {
			if (blackAlive) {
				voted++;
				blackVotes++;
			}
		}
		if (voted == 1 && Application::IsKeyPressed('3')) {
			if (cyanAlive) {
				voted++;
				cyanVotes++;
			}
		}
		if (voted == 1 && Application::IsKeyPressed('4')) {
			if (orangeAlive) {
				voted++;
				orangeVotes++;
			}
		}
		if (voted == 1 && Application::IsKeyPressed('5')) {
			voted++;
			noVotes++;
		}
		if (voted == 2) {
			//determine who gets voted out
			if (noVotes >= redVotes && noVotes >= blackVotes && noVotes >= cyanVotes && noVotes >= orangeVotes) mostVotes = 0;
			else if (redVotes > blackVotes && redVotes > cyanVotes && redVotes > orangeVotes) mostVotes = 1;
			else if (blackVotes > redVotes && blackVotes > cyanVotes && blackVotes > orangeVotes) mostVotes = 2;
			else if (cyanVotes > redVotes && cyanVotes > blackVotes && cyanVotes > orangeVotes) mostVotes = 3;
			else if (orangeVotes > redVotes && orangeVotes > blackVotes && orangeVotes > cyanVotes) mostVotes = 4;
			else mostVotes = 0;

			//kills most voted player
			switch (mostVotes) {
			case 0:
				break; //nothing happens
			case 1:
				redAlive = false;
				break;
			case 2:
				blackAlive = false;
				break;
			case 3:
				cyanAlive = false;
				break;
			case 4:
				orangeAlive = false;
				break;
			}
			voted++;
		}
		if (Application::IsKeyPressed(VK_RETURN) && voted == 3) {
			voting = false;
		}
	}
	// remove chat if too far away
	if (talkTarget == "black") {
		if (DistBetweenPoints(camera.position.x, camera.position.z, blackTranslateX, blackTranslateZ) > 3 + interactRange) {
			talk = "";
			talkTarget = "";
		}
	}
	if (talkTarget == "cyan") {
		if (DistBetweenPoints(camera.position.x, camera.position.z, cyanTranslateX, cyanTranslateZ) > 3 + interactRange) {
			talk = "";
			talkTarget = "";
		}
	}
	if (talkTarget == "orange") {
		if (DistBetweenPoints(camera.position.x, camera.position.z, orangeTranslateX, orangeTranslateZ) > 3 + interactRange) {
			talk = "";
			talkTarget = "";
		}
	}
	if (Application::IsKeyPressed('R')) {
		gameOver = 0;
		voting = false, lightsOn = true, impostor = false;
		voted = 0;
		redVotes = 0, blackVotes = 0, cyanVotes = 0, orangeVotes = 0, noVotes = 0, mostVotes = 0;
		redAlive = true, blackAlive = true, cyanAlive = true, orangeAlive = true;
		task1 = false, task2 = false, task3 = false, iTask1 = false, iTask2 = false;
		FPS = 0;
		talk = "";
		talkTarget = "";
		bLightEnabled = true;
		lightCutoff = 90, lightInner = 60;
		turnLightOff = false;
		redRotateX = 0;
		redRotateY = 0;
	}
}
float SceneMainMenu::DistBetweenPoints(float x1, float z1, float x2, float z2)
{
	float distX = x1 - x2;
	float distZ = z1 - z2;
	float dist = 0.0f;
	dist = sqrt((distX * distX) + (distZ * distZ));
	return dist;
}
bool SceneMainMenu::CollisionLineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	// calculate the direction of the lines
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) return true;
	return false;
}
bool SceneMainMenu::CollisionLineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh)
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

bool SceneMainMenu::CollisionPointCircle(float px, float py, float cx, float cy, float r)
{
	// get distance between the point and circle's center using the Pythagorean Theorem
	float distX = px - cx;
	float distY = py - cy;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the circle's radius the point is inside!
	if (distance <= r) return true;
	return false;
}

bool SceneMainMenu::CollisionLinePoint(float x1, float y1, float x2, float y2, float px, float py)
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

bool SceneMainMenu::CollisionLineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r)
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

void SceneMainMenu::RenderMesh(Mesh* mesh, bool enableLight)
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
void SceneMainMenu::RenderText(Mesh* mesh, std::string text, Color color)
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
void SceneMainMenu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void SceneMainMenu::RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y)
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
void SceneMainMenu::Render()
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

	/*modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(0.35, 0.35, 0.35);
	RenderMesh(meshList[GEO_ENEMY1], true);
	modelStack.PopMatrix();*/

	/*modelStack.PushMatrix();
	modelStack.Scale(75, 75, 75);
	modelStack.Translate(0, 0, -0.8f);
	RenderMesh(meshList[GEO_BACKGROUND], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(10, 10, 10);
	modelStack.Translate(0, 0.8f, -1);
	RenderMesh(meshList[GEO_TITLE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(3.7f, 3.7f, 3.7f);
	modelStack.Translate(0, 0.9f, -0.7f);
	RenderMesh(meshList[GEO_PLAY], false);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	//modelStack.Scale(0, 0, 1);
	//modelStack.Translate(0, 0, 0);
	RenderImageOnScreen(meshList[GEO_BACKGROUND], Color(1, 1, 1), 80, 60, 40, 30);
	RenderImageOnScreen(meshList[GEO_TITLE], Color(1, 1, 1), 40, 30, 40, 45);
	RenderImageOnScreen(meshList[GEO_PLAY], Color(1, 1, 1), 40, 30, 25, 20);
	RenderImageOnScreen(meshList[GEO_OPTIONS], Color(1, 1, 1), 80, 60, 60, 20);
	modelStack.PopMatrix();

	RenderSkybox();

}

void SceneMainMenu::RenderSkybox()
{
	/*modelStack.PushMatrix();
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
	modelStack.PopMatrix();*/
}

void SceneMainMenu::Exit()
{
	// Cleanup VBO here
	delete meshList[GEO_TEXT];
	/*delete meshList[GEO_LEFT];
	delete meshList[GEO_RIGHT];
	delete meshList[GEO_TOP];
	delete meshList[GEO_BOTTOM];
	delete meshList[GEO_FRONT];
	delete meshList[GEO_BACK];*/
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}