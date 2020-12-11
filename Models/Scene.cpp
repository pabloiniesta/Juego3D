#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL/glut.h>
#include <vector>

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


void Scene::init(int lvl)
{
	if (mapa != NULL)
		delete mapa;
	if (player != NULL)
		delete player;
	if (!objects.empty())
		objects.clear();
	//AQUI SE BORRA TODO
	initShaders();
	//cargar mapa
	if(lvl == 1){
		OutputDebugStringW(L"LVL1");
		mapa = Map::createMap("levels/level01.txt", glm::vec2(32, 16), texProgram);
	}
	else if (lvl == 2) {
		OutputDebugStringW(L"LVL1");
		mapa = Map::createMap("levels/level02.txt", glm::vec2(32, 16), texProgram);
	}
	else if (lvl == 3) {
		//mapa 3
		mapa = Map::createMap("levels/level03.txt", glm::vec2(32, 16), texProgram);
	}
	else if (lvl == 4) {
		//mapa 4
		mapa = Map::createMap("levels/level04.txt", glm::vec2(32, 16), texProgram);
	}
	else if (lvl == 5) {
		//mapa 5
		mapa = Map::createMap("levels/level05.txt", glm::vec2(32, 16), texProgram);
	}

	//cargar nivel
	level = Level::createLevel(glm::vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");

	//cargar player
	player = new Player();
	player->init(texProgram);
	player->setMap(mapa);

	//cargar game objects
	for (int i = 0; i < mapa->objectInfo.size(); i++) {
		pair<char, pair<int, int> > info = mapa->objectInfo[i];
		char tipoObject = info.first;
		int posObjectx = info.second.first;
		int posObjecty = info.second.second;
		GameObject *object = new GameObject();
		object->init(texProgram, tipoObject, posObjectx, posObjecty);
		objects.push_back(*object);
	}

	//camara
	projection = glm::perspective(45.f / 180.f * PI, float(CAMERA_WIDTH) / float(CAMERA_HEIGHT), 0.1f, 100.f);
	currentTime = 0.0f;

	stage = 1;
	camaraXpos = 10; //camara inicial en x = 10

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);

	//mirar colision objetos y player
	bool choque = false;
	for (int i = 0; i < objects.size();i++) {
		if (objects[i].hp > 0) {
			bool colision = CheckCollisionPlayerObject(*player, objects[i]);
			if (colision) {
				if (objects[i].tipoObject == 's') { //si es la estrella
					objects[i].colision();
					Game::instance().nextLevel(0);
				}
			}
		}
	}

	//control de camara dependiendo del stage y el player
	if (stage == 1) {
		if (player->posPlayer.x > 20 && player->velPlayer.x > 0) { //se va pa la derecha
			camaraXpos = 30;
			stage = 2;
		}
	}
	else if (stage == 2) {
		if (player->posPlayer.x > 40 && player->velPlayer.x > 0) { //se va pa la derecha
			camaraXpos = 50;
			stage = 3;
		}
		if (player->posPlayer.x < 20 && player->velPlayer.x < 0) { //se va pa la izquierda
			camaraXpos = 10;
			stage = 1;
		}
	}
	else if (stage == 3) {
		if (player->posPlayer.x < 40 && player->velPlayer.x < 0) { //se va pa la izquierda
			camaraXpos = 30;
			stage = 2;
		}
	}

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
	viewMatrix = glm::lookAt(glm::vec3(camaraXpos, 9.f, 30.f), glm::vec3(camaraXpos, 9.f, -1.f), glm::vec3(0.f, 1.f, 0.f));

	// Render level
	/*
	modelMatrix = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	texProgram.setUniformMatrix3f("normalmatrix", normalMatrix);
	level->render();*/

	//Render player
	player->render(currentTime, viewMatrix, texProgram);
	//render map
	mapa->render(currentTime, viewMatrix, texProgram);

	for (int i = 0; i < objects.size();i++) {
		if (objects[i].hp > 0) {
			objects[i].render(currentTime, viewMatrix, texProgram);
		}
	}
	
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

bool Scene::CheckCollisionPlayerObject(Player& one, GameObject& two)
{
	
	bool collisionX = one.posPlayer.x + one.sizePlayer.x >= two.posObject.x &&
		two.posObject.x + two.sizeObject.x >= one.posPlayer.x;
	// Collision y-axis?
	bool collisionY = one.posPlayer.y + one.sizePlayer.y >= two.posObject.y &&
		two.posObject.y + two.sizeObject.y >= one.posPlayer.y;
	// Collision only if on both axes
	return collisionX && collisionY;

}



