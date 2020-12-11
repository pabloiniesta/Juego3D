#include "Menu.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#define PI 3.14159f
#include "Scene.h"
#include <GL/glut.h>

using namespace std;


Menu::Menu()
{
}


void Menu::init() {
	bEnterPressed = false;
	bCredits = false;
	bInstructions = false;
	bGameOver = false;
	bWin = false;
	

	//Shader
	initShaders();
	//projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	projection = glm::perspective(45.f / 180.f * PI, float(CAMERA_WIDTH) / float(CAMERA_HEIGHT), 0.1f, 100.f);
	levelMenu = Level::createLevel(glm::vec3(4, 4, 7), texProgram, "images/floor.png", "images/main_menu.png");
	levelInst = Level::createLevel(glm::vec3(4, 4, 7), texProgram, "images/floor.png", "images/instructions.png");
	levelCreds = Level::createLevel(glm::vec3(4, 4, 7), texProgram, "images/floor.png", "images/credits.png");
	levelWin = Level::createLevel(glm::vec3(4, 4, 7), texProgram, "images/floor.png", "images/youwin.png");
}

void Menu::update(int deltaTime) {


	if (Game::instance().getKey(13) && !bEnterPressed) { //enter key
		//Game::instance().playSound("sounds/menu_sel.wav", false);
		bEnterPressed = true;
		OutputDebugStringW(L"LEVEL1");
		if (bInstructions) //If at Instructions screen
			bInstructions = false; //Exit instructions screen
		else if (bCredits) //If at Credits screen
			bCredits = false; //Exit instructions screen
		else if (bGameOver) { //If at Lose screen
			bGameOver = false; //Exit instructions screen
			//Game::instance().stopSounds();
			//Game::instance().playSound("sounds/menu.mp3", true);
		}
		else if (bWin) { //If at Win screen
			bWin = false; //Exit instructions screen
			//Game::instance().stopSounds();
			//Game::instance().playSound("sounds/menu.mp3", true);
		}

	}
	else { //Main menu screen
		if (Game::instance().getKey(49)) { //Input 1 - Start
			//Game::instance().playSound("sounds/menu_sel.wav", false);
			Game::instance().nextLevel(0);
		}
		else if (Game::instance().getKey(50)) { //Input 2 - Instructions
			//Game::instance().playSound("sounds/menu_sel.wav", false);
			bInstructions = true;
			bEnterPressed = false;
		}
		else if (Game::instance().getKey(51)) { //Input 3 - Credits
			//Game::instance().playSound("sounds/menu_sel.wav", false);
			bCredits = true;
			bEnterPressed = false;
		}
	}
}

void Menu::render() {
	
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f); //Color uniform transform
	viewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	texProgram.setUniformMatrix3f("normalmatrix", normalMatrix);
	

	
	if (bInstructions) {//At instructions screen
		levelInst->render();
	}
	else if (bCredits) {
		levelCreds->render();
	}
	else if (bWin) {
		levelWin->render();
	}
	else { //Main menu screen
		levelMenu->render();
	}
	
}

void Menu::activateWin() {
	//Game::instance().stopSounds();
	//Game::instance().playSound("sounds/win.wav", false);
	bWin = true;
	bEnterPressed = false;
}

void Menu::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}