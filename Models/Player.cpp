#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
#include <GL/glut.h>
#include "Game.h"

const float X_INICIAL = 0.f;
const float Y_INICIAL = 0.f;
const float Z_INICIAL = -5.f;

const glm::vec2 INITIAL_VELOCITY(0.1f, 0.1f);

void Player::init(ShaderProgram& shaderProgram)
{
	modelPlayer = new AssimpModel();
	modelPlayer->loadFromFile("models/cubocara.obj", shaderProgram);


	//vel player inicial x = 1, y = 1
	velPlayer = INITIAL_VELOCITY;
	//posicion inicial (hardcodeao por ahora)
	posPlayer = glm::vec3(X_INICIAL, Y_INICIAL, Z_INICIAL);
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
		velPlayer.y *= -1.f;
	}
	if (Game::instance().getKey(52)) { //4
		if (Game::instance().getKey(52)) {
			Game::instance().keyReleased(52);
		}
		if (Game::instance().getKey(52)) {
			Game::instance().keyReleased(52);
		}
		velPlayer.y *= -1.f;
	}
	if (Game::instance().getKey(53)) { //5
		if (Game::instance().getKey(53)) {
			Game::instance().keyReleased(53);
		}
		if (Game::instance().getKey(53)) {
			Game::instance().keyReleased(52);
		}
		velPlayer.x *= -1.f;
	}
	
	posPlayer.x += velPlayer.x;
	posPlayer.y += velPlayer.y;
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
	modelMatrix = glm::translate(modelMatrix, glm::vec3(posPlayer.x, posPlayer.y, posPlayer.z)); //esta establece la pos del objeto
	
	//rotar modelo: cuanto quieres rotar y en que eje quieres hacerlo
	//modelMatrix = glm::rotate(modelMatrix, 2.f, glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -centerModelBase);

	shaderProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	shaderProgram.setUniformMatrix3f("normalmatrix", normalMatrix);

	modelPlayer->render(shaderProgram);
}

void Player::setPosition(const glm::vec3& pos)
{
	posPlayer = pos;
}
