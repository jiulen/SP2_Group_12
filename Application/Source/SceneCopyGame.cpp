#include "SceneCopyGame.h" 
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


SceneCopyGame::SceneCopyGame()
{
}

SceneCopyGame::~SceneCopyGame()
{
}
void SceneCopyGame::UseScene()
{
	glBindVertexArray(m_vertexArrayID);
	glUseProgram(m_programID);
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
}
void SceneCopyGame::Init()
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

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = nullptr;
	}
	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//tron_up.tga");
	meshList[GEO_PAD] = MeshBuilder::GenerateQuad("pad", Color(1, 1, 1), 1.f);
	meshList[GEO_PAD]->textureID = LoadTGA("Image//copy.tga");
	meshList[GEO_PRESSED] = MeshBuilder::GenerateQuad("pressed", Color(1, 1, 1), 1.f);
	meshList[GEO_PRESSED]->textureID = LoadTGA("Image//greysquare.tga");
	meshList[GEO_BLUESQUARE] = MeshBuilder::GenerateQuad("bluesquare", Color(1, 1, 1), 1.f);
	meshList[GEO_BLUESQUARE]->textureID = LoadTGA("Image//bluesquare.tga");
	meshList[GEO_GREENLIGHT] = MeshBuilder::GenerateQuad("greenlight", Color(1, 1, 1), 1.f);
	meshList[GEO_GREENLIGHT]->textureID = LoadTGA("Image//greenlight.tga");

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

}

void SceneCopyGame::Update(double dt)
{
	Application::GetCursorPos(&mouseX, &mouseY);
	unsigned w = Application::GetWindowWidth();
	unsigned h = Application::GetWindowHeight();
	mouseX = mouseX / w * 80.f; //convert (0,800) to (0,80)
	mouseY = (h - mouseY) / h * 60.f; //convert (600,0) to (0,60)
	//mouse inputs
	static bool bLButtonState = false;
	if (turn == 1)
	{
		if (!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
			if ((mouseY >= 31.4) && (mouseY <= 38.6))
			{
				if ((mouseX >= 47) && (mouseX <= 52.6))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 1;
							i = currentstage;
						}
					}
					pressedkey[0] = 1;
					clicked = 1;
				}
				else if ((mouseX >= 54.1) && (mouseX <= 59.7))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 2;
							i = currentstage;
						}
					}
					pressedkey[1] = 1;
					clicked = 1;
				}
				else if ((mouseX >= 61.3) && (mouseX <= 66.9))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 3;
							i = currentstage;
						}
					}
					pressedkey[2] = 1;
					clicked = 1;
				}
			}
			else if ((mouseY >= 22.4) && (mouseY <= 29.6))
			{
				if ((mouseX >= 47) && (mouseX <= 52.6))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 4;
							i = currentstage;
						}
					}
					pressedkey[3] = 1;
					clicked = 1;
				}
				else if ((mouseX >= 54.1) && (mouseX <= 59.7))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 5;
							i = currentstage;
						}
					}
					pressedkey[4] = 1;
					clicked = 1;
				}
				else if ((mouseX >= 61.3) && (mouseX <= 66.9))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 6;
							i = currentstage;
						}
					}
					pressedkey[5] = 1;
					clicked = 1;
				}
			}
			else if ((mouseY >= 13.1) && (mouseY <= 20.4))
			{
				if ((mouseX >= 47) && (mouseX <= 52.6))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 7;
							i = currentstage;
						}
					}
					pressedkey[6] = 1;
					clicked = 1;
				}
				else if ((mouseX >= 54.1) && (mouseX <= 59.7))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 8;
							i = currentstage;
						}
					}
					pressedkey[7] = 1;
					clicked = 1;
				}
				else if ((mouseX >= 61.3) && (mouseX <= 66.9))
				{
					for (int i = 0; i < currentstage; i++)
					{
						if (pressed[i] == 0)
						{
							pressed[i] = 9;
							i = currentstage;
						}
					}
					pressedkey[8] = 1;
					clicked = 1;
				}
			}
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			if (clicked == 1)
			{
				for (int i = 0; i < 9; i++)
				{
					if (pressedkey[i] != 0)
						pressedkey[i] = 0;
				}
				if (pressed[correct] == tocopy[correct])
				{
					correct++;
					if (correct == currentstage)
					{
						for (int i = 0; i < currentstage; i++)
							pressed[i] = 0;
						correct = 0;
						turn = 0;
						currentstage++;
						displayed = 0;
					}
				}
				else
				{
					attempts++;
					set = currentstage = displayed = correct = turn = start = time = 0;
					for (int i = 0; i < 5; i++)
					{
						tocopy[i] = 0;
						pressed[i] = 0;
					}
					for (int i = 0; i < 9; i++)
						pressedkey[i] = 0;
				}
				clicked = 0;
			}
		}
	}
	if (start == 1)
		time += dt;
	else
		time = 0;
	if (attempts > 3)
	{
		timer += dt;
	}
	
	FPS = 1 / (float)dt;
}


void SceneCopyGame::RenderMesh(Mesh* mesh, bool enableLight)
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
void SceneCopyGame::RenderText(Mesh* mesh, std::string text, Color color)
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
void SceneCopyGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void SceneCopyGame::RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y, float rt)
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
	modelStack.Rotate(-rt, 0, 0, 1);
	modelStack.Scale(sizeX, sizeY, 1);
	RenderMesh(mesh, false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
void SceneCopyGame::Render()
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

	modelStack.LoadIdentity();

	RenderImageOnScreen(meshList[GEO_BACKGROUND], Color(1, 1, 1), 100, 100, 40, 30, 0);

	if (attempts<=3)
		RenderCopyGame();
	else
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "YOU FAILED!", Color(1, 1, 1), 10, 13.5, 25);
		if (timer >= 3)
			currentstage = 7;
	}
}

void SceneCopyGame::RenderCopyGame()
{
	RenderImageOnScreen(meshList[GEO_PAD], Color(1, 1, 1), 70, 50, 39.5, 30, 0);
	if (currentstage>=2)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 16.5, 45.6, 0);
	if (currentstage >= 3)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 21.4, 45.6, 0);
	if (currentstage >= 4)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 26.3, 45.6, 0);
	if (currentstage >= 5)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 31, 45.6, 0);
	if (pressed[0] !=0)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 47.4, 45.6, 0);
	if (pressed[1] != 0)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 52.3, 45.6, 0);
	if (pressed[2] != 0)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 57.3, 45.6, 0);
	if (pressed[3] != 0)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 62.1, 45.6, 0);
	if (pressed[4] != 0)
		RenderImageOnScreen(meshList[GEO_GREENLIGHT], Color(1, 1, 1), 3.8, 5.5, 66.8, 45.6, 0);
	if (set == 0)
	{
		for (int i = 0; i < 5; i++)
			tocopy[i] = rand() % 9 + 1;
		currentstage = 1;
		set++;
	}
	if (turn == 0)
	{
		for (int i = 0; i < 9; i++)
			pressedkey[i] = 1;
		if (displayed!=currentstage)
		{
			start = 1;
			if ((time > 0.3) && (time <= 1))
			{
				if (tocopy[displayed] == 1)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 14.6, 35, 0);
				else if (tocopy[displayed] == 2)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 21.8, 35, 0);
				else if (tocopy[displayed] == 3)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 29, 35, 0);
				else if (tocopy[displayed] == 4)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 14.6, 25.8, 0);
				else if (tocopy[displayed] == 5)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 21.8, 25.8, 0);
				else if (tocopy[displayed] == 6)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 29, 25.8, 0);
				else if (tocopy[displayed] == 7)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 14.6, 16.6, 0);
				else if (tocopy[displayed] == 8)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 21.8, 16.6, 0);
				else if (tocopy[displayed] == 9)
					RenderImageOnScreen(meshList[GEO_BLUESQUARE], Color(1, 1, 1), 5.5, 7.5, 29, 16.6, 0);
			}
			else if (time > 1)
			{
				start = 0;
				displayed++;
			}
		}
		else if (displayed == currentstage)
		{
			clear = 0;
			turn = 1;
		}
	}
	if ((turn == 1) && (clear == 0))
	{
		for (int i = 0; i < 9; i++)
			pressedkey[i] = 0;
		clear = 1;
	}

	if (pressedkey[0] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 49.8, 35, 0);
	if (pressedkey[1] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 57, 35, 0);
	if (pressedkey[2] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 64.2, 35, 0);
	if (pressedkey[3] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 49.8, 25.8, 0);
	if (pressedkey[4] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 57, 25.8, 0);
	if (pressedkey[5] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 64.2, 25.8, 0);
	if (pressedkey[6] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 49.8, 16.6, 0);
	if (pressedkey[7] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 57, 16.6, 0);
	if (pressedkey[8] == 1)
		RenderImageOnScreen(meshList[GEO_PRESSED], Color(1, 1, 1), 5.5, 7.5, 64.2, 16.6, 0);

	std::ostringstream ss;
	ss.precision(4);
	ss << "Attempt: " << attempts<<"/3";
	if (attempts==3)
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 0, 0);
	else 
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 0);
}

int SceneCopyGame::NextScene()
{
	if (currentstage >= 6)
	{
		int a = currentstage;
		set = currentstage = displayed = correct = turn = start = time = timer = 0;
		attempts = 1;
		for (int i = 0; i < 5; i++)
		{
			tocopy[i] = 0;
			pressed[i] = 0;
		}
		for (int i = 0; i < 9; i++)
			pressedkey[i] = 0;
		if (a==6)
			return 3;
		else if (a == 7)
			return 7;
	}
	return 0; //not switching
}

void SceneCopyGame::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		delete meshList[i];
	}

	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}