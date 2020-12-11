#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
//#include <irrKlang.h>

using namespace std;
//using namespace irrklang;


//ISoundEngine* SoundEngine = createIrrKlangDevice();

/*
void Game::playSound(const char* path, bool loop) {
	SoundEngine->play2D(path, loop);
}

void Game::stopSounds() {
	SoundEngine->stopAllSounds();
}
*/

void Game::init()
{
	bPlay = true;
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	setMenuState();
	MainMenu.init();
}

void Game::setMenuState() {
	levelAct = 0;
}


bool Game::update(int deltaTime) {
	if (levelAct == 0 || levelAct == 6) {  //Main menu
		MainMenu.update(deltaTime);
	}
	else {
		scene.update(deltaTime);
	}
	if (getKey(52)) { //4 -> saltar pantalla
		if (Game::instance().getKey(52)) {
			Game::instance().keyReleased(52);
		}
		if (Game::instance().getKey(52)) {
			Game::instance().keyReleased(52);
		}
		nextLevel(0);
	}
	return bPlay;
}


void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (levelAct == 0 || levelAct == 6) {  //Main menu
		MainMenu.render();
	}
	else {
		scene.render();
	}
}

void Game::nextLevel(int lvl) {
	if (lvl == 0) {
		++levelAct; //Go to next level
		if (levelAct == 1) {//Num of total levels+1
			//SoundEngine->stopAllSounds();
			//SoundEngine->play2D("sounds/background.mp3", true);
			OutputDebugStringW(L"LEVEL1");
			scene.init(1);
		}
		else if (levelAct == 2) {
			scene.init(2);
		}
		else if (levelAct == 3) {
			scene.init(3);
		}
		else if (levelAct == 4) {
			scene.init(4);
		}
		else if (levelAct == 5) {
			scene.init(5);
		}
		else if (levelAct == 6) {
			MainMenu.activateWin();
		}
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





