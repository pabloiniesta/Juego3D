#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glut.h>

//testeando github
#define PI 3.14159f

Scene::Scene()
{
	level = NULL;
}

Scene::~Scene()
{
	if (level != NULL)
		delete level;
}


void Scene::init()
{
	initShaders();
	//cargar nivel
	level = Level::createLevel(glm::vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");

	//cargar player
	player = new Player();
	player->init(texProgram);

	//camara
	projection = glm::perspective(45.f / 180.f * PI, float(CAMERA_WIDTH) / float(CAMERA_HEIGHT), 0.1f, 100.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	player->update(deltaTime);

}

void Scene::render()
{
	glm::mat4 modelMatrix, viewMatrix;
	glm::mat3 normalMatrix;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//primer vec pos camara -> x: mirar der o izq, y: mirar arriba o abajo, z: alejar o acercar camara
	//segundo vec punto a donde mirar -> x e y = 0 (mirar al centro) y miramos a las z negativ porque ahi esta el player
	//tercer vec orientacion de la camara -> y pos todo del derecho y negativ todo boca abajo
	//para mover cam hay cambiar las x/y del primero y las x/y del segundo vec con los mismos valores
	viewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 25.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));

	// Render level
	modelMatrix = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	texProgram.setUniformMatrix3f("normalmatrix", normalMatrix);
	//level->render();

	
	//Render player
	player->render(currentTime, viewMatrix, texProgram);
}

void Scene::initShaders()
{
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



