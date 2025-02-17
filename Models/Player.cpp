#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
#include <GL/glut.h>
#include "Game.h"
#include <irrKlang.h>
using namespace irrklang;

const glm::vec2 INITIAL_POS(2.f, 2.f);
const glm::vec2 INITIAL_VELOCITY(0.1f, 0.1f);




ISoundEngine* SoundEngine1 = createIrrKlangDevice();


void Player::playSound(const char* path, bool loop) {
	SoundEngine1->play2D(path, loop);
}

void Player::stopSounds() {
	SoundEngine1->stopAllSounds();
}

void Player::init(ShaderProgram& shaderProgram)
{
	modelPlayer = new AssimpModel();
	modelPlayer->loadFromFile("models/cubocara.obj", shaderProgram);


	//vel player inicial x = 1, y = 1
	velPlayer = INITIAL_VELOCITY;
	//posicion inicial (hardcodeao por ahora)
	posPlayer = INITIAL_POS;
	sizePlayer = glm::vec2(1, 1);
	rail = false;
}

void Player::update(int deltaTime)
{
	
	if (Game::instance().getKey(32)) { //space
		if (Game::instance().getKey(32)) {
			Game::instance().keyReleased(32);
		}
		if (Game::instance().getKey(32)) {
			Game::instance().keyReleased(32);
		}
		if(rail)velPlayer.x *= -1.f; //si esta rail se mueve horizontal
		else velPlayer.y *= -1.f;
	}

	posPlayer.x += velPlayer.x;
	if (map->collisionMoveRight(posPlayer)) {
		posPlayer.x -= velPlayer.x;
		velPlayer.x = -velPlayer.x;
		playSound("sounds/pared.mp3", false);
	}
	if (map->collisionMoveLeft(posPlayer)) {
		posPlayer.x -= velPlayer.x;
		velPlayer.x = -velPlayer.x;
		playSound("sounds/pared.mp3", false);
	}

	if (!rail) { //si no esta en el rail se mueve vertical
		posPlayer.y += velPlayer.y;
		if (map->collisionMoveUp(posPlayer)) {
			posPlayer.y -= velPlayer.y;
			velPlayer.y = -velPlayer.y;
			playSound("sounds/pared.mp3", false);
		}
		if (map->collisionMoveDown(posPlayer)) {
			posPlayer.y -= velPlayer.y;
			velPlayer.y = -velPlayer.y;
			playSound("sounds/pared.mp3", false);
		}
	}
	
}

void Player::render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram) //current time es para mover/rotar ciclicamente
{
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	// Render loaded model
	float scaleFactor = 1.f / modelPlayer->getHeight();
	glm::vec3 centerModelBase = modelPlayer->getCenter() - glm::vec3(0.f, -modelPlayer->getHeight() / 2.f, 0.f);

	modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.5f * fabs(sinf(3.f * currentTime / 1000.f)), 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(posPlayer.x, posPlayer.y, 0)); //esta establece la pos del objeto
	
	//rotar modelo: cuanto quieres rotar y en que eje quieres hacerlo
	//modelMatrix = glm::rotate(modelMatrix, 2.f, glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -centerModelBase);

	shaderProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	shaderProgram.setUniformMatrix3f("normalmatrix", normalMatrix);

	modelPlayer->render(shaderProgram);
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
}

void Player::reset()
{
	posPlayer = INITIAL_POS;
	velPlayer = INITIAL_VELOCITY;
}

void Player::setMap(Map* mapa)
{
	map = mapa;
}
