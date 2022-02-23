#include "SceneSettings.h" 
#include "Camera3.h"
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


SceneSettings::SceneSettings()
{
}

SceneSettings::~SceneSettings()
{
}
void SceneSettings::UseScene()
{
	glBindVertexArray(m_vertexArrayID);
	glUseProgram(m_programID);
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
}
void SceneSettings::Init()
{
	// Init VBO here
	redPicked = true;
	bluePicked = false;
	greenPicked = false;
	lowPicked = false;
	mediumPicked = true;
	highPicked = false;
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

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);


	camera.Init(Vector3(0, 4.5, 5.5), Vector3(0, 4.5, 4.5), Vector3(0, 1, 0)); //facing -x (1.0 diff in z)

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("greenbackground", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//background.tga");
	meshList[GEO_BACKBUTTON] = MeshBuilder::GenerateQuad("backbutton", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACKBUTTON]->textureID = LoadTGA("Image//backbutton.tga");
	meshList[GEO_OPTIONSLIDER] = MeshBuilder::GenerateQuad("slot", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_OPTIONSLIDER]->textureID = LoadTGA("Image//sliderslot.tga");
	meshList[GEO_OPTIONKNOB] = MeshBuilder::GenerateQuad("knob", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_OPTIONKNOB]->textureID = LoadTGA("Image//sliderknob.tga");
	meshList[GEO_OPTIONDESELECT] = MeshBuilder::GenerateQuad("deselect", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_OPTIONDESELECT]->textureID = LoadTGA("Image//deselect.tga");
	meshList[GEO_OPTIONSELECT] = MeshBuilder::GenerateQuad("select", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_OPTIONSELECT]->textureID = LoadTGA("Image//select.tga");

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

void SceneSettings::Update(double dt)
{
	//mouse inputs
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		//use tasklist as button
		float BUTTON_LEFT = 30;
		float BUTTON_RIGHT = 50;
		float BUTTON_BOTTOM = 10;
		float BUTTON_TOP = 20;
		float BUTTON_RED_LEFT = 32;
		float BUTTON_RED_RIGHT = 38;
		float BUTTON_RED_BOTTOM = 23;
		float BUTTON_RED_TOP = 28;
		float BUTTON_BLUE_LEFT = 43;
		float BUTTON_BLUE_RIGHT = 48;
		float BUTTON_BLUE_BOTTOM = 23;
		float BUTTON_BLUE_TOP = 28;
		float BUTTON_GREEN_LEFT = 52;
		float BUTTON_GREEN_RIGHT = 57;
		float BUTTON_GREEN_BOTTOM = 23;
		float BUTTON_GREEN_TOP = 28;
		float SLIDER_LEFT_MAX = 31;
		float SLIDER_RIGHT_MAX = 59;
		float SLIDER_TOP_MAX = 41;
		float SLIDER_BOTTOM_MAX = 39;
		float BUTTON_LOW_LEFT = 32;
		float BUTTON_LOW_RIGHT = 38;
		float BUTTON_LOW_BOTTOM = 38.5f;
		float BUTTON_LOW_TOP = 42;
		float BUTTON_MEDIUM_LEFT = 43;
		float BUTTON_MEDIUM_RIGHT = 48;
		float BUTTON_MEDIUM_BOTTOM = 38.5f;
		float BUTTON_MEDIUM_TOP = 42;
		float BUTTON_HIGH_LEFT = 52;
		float BUTTON_HIGH_RIGHT = 57;
		float BUTTON_HIGH_BOTTOM = 38.5f;
		float BUTTON_HIGH_TOP = 42;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / w * 80.f; //convert (0,800) to (0,80)
		float posY = (h - y) / h * 60.f; //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
		if (posX > BUTTON_LEFT && posX < BUTTON_RIGHT && posY > BUTTON_BOTTOM && posY < BUTTON_TOP)
		{
			check = 1;
		}
		else if (posX > BUTTON_RED_LEFT && posX < BUTTON_RED_RIGHT && posY > BUTTON_RED_BOTTOM && posY < BUTTON_RED_TOP)
		{
			std::cout << "Chosen red crosshair!" << std::endl;
			redPicked = true;
			bluePicked = false;
			greenPicked = false;
		}
		else if (posX > BUTTON_BLUE_LEFT && posX < BUTTON_BLUE_RIGHT && posY > BUTTON_BLUE_BOTTOM && posY < BUTTON_BLUE_TOP)
		{
			std::cout << "Chosen blue crosshair!" << std::endl;
			redPicked = false;
			bluePicked = true;
			greenPicked = false;
		}
		else if (posX > BUTTON_GREEN_LEFT && posX < BUTTON_GREEN_RIGHT && posY > BUTTON_GREEN_BOTTOM && posY < BUTTON_GREEN_TOP)
		{
			std::cout << "Chosen green crosshair!" << std::endl;
			redPicked = false;
			bluePicked = false;
			greenPicked = true;
		}
		if (posX > BUTTON_LOW_LEFT && posX < BUTTON_LOW_RIGHT && posY > BUTTON_LOW_BOTTOM && posY < BUTTON_LOW_TOP)
		{
			std::cout << "Chosen low sensitivty option!" << std::endl;
			lowPicked = true;
			mediumPicked = false;
			highPicked = false;
		}
		else if (posX > BUTTON_MEDIUM_LEFT && posX < BUTTON_MEDIUM_RIGHT && posY > BUTTON_MEDIUM_BOTTOM && posY < BUTTON_MEDIUM_TOP)
		{
			std::cout << "Chosen medium senstivity!" << std::endl;
			lowPicked = false;
			mediumPicked = true;
			highPicked = false;
		}
		else if (posX > BUTTON_HIGH_LEFT && posX < BUTTON_HIGH_RIGHT && posY > BUTTON_HIGH_BOTTOM && posY < BUTTON_HIGH_TOP)
		{
			std::cout << "Chosen high senstivity!" << std::endl;
			lowPicked = false;
			mediumPicked = false;
			highPicked = true;
		}
		else
		{
			std::cout << "Not clicking anywhere?" << std::endl;
		}
		/*if (posX > SLIDER_LEFT_MAX && posX < SLIDER_RIGHT_MAX && posY > SLIDER_BOTTOM_MAX && posY < SLIDER_TOP_MAX)
		{
			std::cout << "Slider!" << std::endl;
			Application::GetCursorPos(&mouseX, &mouseY);
			Vector3 newPos = Vector3(mouseX, mouseY, 1);
		}
		else
		{
			std::cout << "Let go!" << std::endl;
		}*/


	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
		if (check == 1)
		{
			check = 0;
			std::cout << "Go back!" << std::endl;
			nextscene = 1;
		}
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
}

void SceneSettings::RenderMesh(Mesh* mesh, bool enableLight)
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
void SceneSettings::RenderText(Mesh* mesh, std::string text, Color color)
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
void SceneSettings::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
void SceneSettings::RenderImageOnScreen(Mesh* mesh, Color color, float sizeX, float sizeY, float x, float y)
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
void SceneSettings::Render()
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

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	RenderImageOnScreen(meshList[GEO_BACKGROUND], Color(1, 1, 1), 80, 60, 40, 30);
	RenderImageOnScreen(meshList[GEO_BACKBUTTON], Color(1, 1, 1), 20, 10, 40, 15);
	//RenderImageOnScreen(meshList[GEO_OPTIONSLIDER], Color(1, 1, 1), 80, 60, 45, 40);
    //RenderImageOnScreen(meshList[GEO_OPTIONKNOB], Color(1, 1, 1), 60, 45, 45, 40);
	if (greenPicked == true)
	{
		Application::SetCrosshair(3);
		RenderImageOnScreen(meshList[GEO_OPTIONSELECT], Color(1, 1, 1), 4, 3, 55, 25);
	}
	else
	{
        RenderImageOnScreen(meshList[GEO_OPTIONDESELECT], Color(1, 1, 1), 4, 3, 55, 25);
	}
	if (bluePicked == true)
	{
		Application::SetCrosshair(2);
		RenderImageOnScreen(meshList[GEO_OPTIONSELECT], Color(1, 1, 1), 4, 3, 45, 25);
	}
	else
	{
		RenderImageOnScreen(meshList[GEO_OPTIONDESELECT], Color(1, 1, 1), 4, 3, 45, 25);
	}
	if (redPicked == true)
	{
		Application::SetCrosshair(1);
		RenderImageOnScreen(meshList[GEO_OPTIONSELECT], Color(1, 1, 1), 4, 3, 35, 25);
	}
	else
	{
		RenderImageOnScreen(meshList[GEO_OPTIONDESELECT], Color(1, 1, 1), 4, 3, 35, 25);
	}
	if (highPicked == true)
	{
		Application::SetSensitivity(1.5f);
		RenderImageOnScreen(meshList[GEO_OPTIONSELECT], Color(1, 1, 1), 4, 3, 55, 40);
	}
	else
	{
		RenderImageOnScreen(meshList[GEO_OPTIONDESELECT], Color(1, 1, 1), 4, 3, 55, 40);
	}
	if (mediumPicked == true)
	{
		Application::SetSensitivity(1);
		RenderImageOnScreen(meshList[GEO_OPTIONSELECT], Color(1, 1, 1), 4, 3, 45, 40);
	}
	else
	{
		RenderImageOnScreen(meshList[GEO_OPTIONDESELECT], Color(1, 1, 1), 4, 3, 45, 40);
	}
	if (lowPicked == true)
	{
		Application::SetSensitivity(0.5f);
		RenderImageOnScreen(meshList[GEO_OPTIONSELECT], Color(1, 1, 1), 4, 3, 35, 40);
	}
	else
	{
		RenderImageOnScreen(meshList[GEO_OPTIONDESELECT], Color(1, 1, 1), 4, 3, 35, 40);
	}
	RenderTextOnScreen(meshList[GEO_TEXT], "Sensitivity", Color(0, 0, 0), 4, 6, 38.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], "Crosshair Color", Color(0, 0, 0), 4, 2, 23.3f);
	RenderTextOnScreen(meshList[GEO_TEXT], "Red", Color(0, 0, 0), 4, 32, 29);
	RenderTextOnScreen(meshList[GEO_TEXT], "Blue", Color(0, 0, 0), 4, 41, 29);
	RenderTextOnScreen(meshList[GEO_TEXT], "Green", Color(0, 0, 0), 4, 50, 29);
	RenderTextOnScreen(meshList[GEO_TEXT], "Low", Color(0, 0, 0), 3, 32.5f, 45);
	RenderTextOnScreen(meshList[GEO_TEXT], "Medium", Color(0, 0, 0), 3, 40.5f, 45);
	RenderTextOnScreen(meshList[GEO_TEXT], "High", Color(0, 0, 0), 3, 52.5f, 45);

}

int SceneSettings::NextScene()
{
	if (nextscene == 1)
	{
		nextscene = 0;
		return 1; //from settings to main menu
	}
	return 0; //not switching
}

void SceneSettings::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}