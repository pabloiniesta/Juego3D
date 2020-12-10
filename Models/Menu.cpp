#include "Menu.h"
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace std;


Menu::Menu()
{
}


void Menu::init() {
	bEnterPressed = false;
	bCredits = false;
	bInstructions = false;
	bWin = false;

	//Shader
	initShaders();
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	//Texture quads
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geomGUI[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };


	mainTextureQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	mainTexture.loadFromFile("images/main_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);

	instructionsQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	instruccionsTex.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);

	creditsQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	creditsTex.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);

	youWinQuad = TexturedQuad::createTexturedQuad(geomGUI, texCoords, texProgram);
	winTex.loadFromFile("images/youwin.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

void Menu::update(int deltaTime) {

	if (Game::instance().getKey(13) && !bEnterPressed) { //enter key
		//Game::instance().playSound("sounds/menu_sel.wav", false);
		bEnterPressed = true;
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
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	glm::mat4 modelview;

	//BACKGROUND
	texProgram.setUniformMatrix4f("modelview", modelview);

	mainTextureQuad->render(mainTexture);

	if (bInstructions) {//At instructions screen
		//Render instructions
		instructionsQuad->render(instruccionsTex);
	}
	else if (bCredits) {
		//render records
		creditsQuad->render(creditsTex);
	}
	else if (bWin) {
		youWinQuad->render(winTex);
	}
	else { //Main menu screen
		mainTextureQuad->render(mainTexture);
	}
}

void Menu::activateWin(int score, int money) {
		//Game::instance().stopSounds();
		//Game::instance().playSound("sounds/win.wav", false);
		bWin = true;
		bEnterPressed = false;
		//calcularTexScore(score);
		//calcularTexMoney(money);
}















void Menu::initShaders() {
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}