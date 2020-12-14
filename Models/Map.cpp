#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
using namespace std;

Map* Map::createMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	Map* map = new Map(levelFile, minCoords, program);
	return map;
}

Map::Map(const string &levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{	
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	modelCubo = new AssimpModel();
	modelCubo->loadFromFile("models/cubopared.obj", program);
}

Map::~Map()
{
	if (map != NULL)
		delete map;
}

void Map::render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram) const
{
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	float scaleFactor = 1.f / modelCubo->getHeight();
	glm::vec3 centerModelBase = modelCubo->getCenter() - glm::vec3(0.f, -modelCubo->getHeight() / 2.f, 0.f);
	for (int i = 0; i < posCubos.size();++i) {
		
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(posCubos[i].x, posCubos[i].y, 0)); //esta establece la pos del objeto

		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
		modelMatrix = glm::translate(modelMatrix, -centerModelBase);

		shaderProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
		normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
		shaderProgram.setUniformMatrix3f("normalmatrix", normalMatrix);

		modelCubo->render(shaderProgram);
	}
	
}

void Map::free()
{
	glDeleteBuffers(1, &vbo);
}


bool Map::loadLevel(const string& levelFile)
{
	
	OutputDebugStringW(L"EMPIEZA LECTURA");
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	fin.open(levelFile.c_str());
	if (!fin.is_open()) {
		OutputDebugStringW(L"ESTOY EN IS OPEN");
		return false;
	}
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0) {
		OutputDebugStringW(L"ESTOY EN COMPARE");
		return false;
	}

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;
	map = new int[mapSize.x * mapSize.y];
	for (int j = mapSize.y-1; j >= 0; j--)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			/*OutputDebugStringW(L" ");
			char s[256];
			sprintf(s, "%d",i);
			OutputDebugStringA((LPCSTR)s);
			sprintf(s, "%d", j);
			OutputDebugStringA((LPCSTR)s);
			OutputDebugStringW(L" ");*/
			fin.get(tile);
			if (tile == ' ') //vacio
				map[j * mapSize.x + i] = 0;
			else if (tile == 's') { //s de star. Pa ganar el nivel
				objectInfo.push_back(make_pair('s', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'p') { //p de pincho, te mata
				objectInfo.push_back(make_pair('p', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'm') { //m de muro. te bloquea
				objectInfo.push_back(make_pair('m', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'c') { //c de carcel. puerta que se abre al pillar llaves
				objectInfo.push_back(make_pair('c', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'k') { //k de key. llaves para abrir puertas
				objectInfo.push_back(make_pair('k', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'l') { //l de luz. se enciende al pillar una llave
				objectInfo.push_back(make_pair('l', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'a') { //Pincho tipo A. se activa/desactiva
				objectInfo.push_back(make_pair('a', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'b') { //pincho tipo B. se activa/desactiva
				objectInfo.push_back(make_pair('b', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'i') { //interruptor. activa o desactiva pinchos
				objectInfo.push_back(make_pair('i', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else if (tile == 'r') { //rail. 
				objectInfo.push_back(make_pair('r', make_pair(i, j)));
				map[j * mapSize.x + i] = 0;
			}
			else {
				map[j * mapSize.x + i] = tile - int('0');
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}


void Map::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, nTiles = 0;
	
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				glm::vec2 posTile;
				posTile = glm::vec2(i, j);
				

				posCubos.push_back(posTile);
				
			}
		}
	}
}

bool Map::collisionMoveLeft(const glm::ivec2& pos) const
{
	int x, y, y2;
	x = pos.x;
	y = pos.y;
	y2 = pos.y + 1;
	if (map[pos.y * mapSize.x + pos.x] != 0 ){ //bloque izquierda es pared
		return true;
	}
	else {
		if (map[((pos.y + 1) * mapSize.x + pos.x + 1)] == 0) { //bloque de encima izq es pared
			if (map[((pos.y + 1) * mapSize.x + pos.x)] != 0) {
				return true;
			}
		}
	}
	return false;
}

bool Map::collisionMoveRight(const glm::ivec2& pos) const
{	
	int x, y, y2;
	x = pos.x+1;
	y = pos.y;
	y2 = pos.y + 1;
	if (map[pos.y * mapSize.x + pos.x+1] != 0 ){ //bloque derecha es pared
		return true;
	}
	
	else {
		if (map[((pos.y + 1) * mapSize.x + pos.x)] == 0) {
			if (map[((pos.y + 1) * mapSize.x + pos.x + 1)] != 0) { //bloque de encima derecha es pared
				return true;
			}
		}
	}
	return false;
}

bool Map::collisionMoveUp(const glm::ivec2& pos) const
{
	int x, x2, y;
	x = pos.x;
	x2 = pos.x + 1;
	y = pos.y+1;
	if (map[(pos.y + 1) * mapSize.x + pos.x] != 0){ //si hay bloque encima 
		return true;
	}
	else {
		if (map[((pos.y) * mapSize.x + pos.x + 1)] == 0) { //si el bloque de la derecha esta vacio
			if (map[((pos.y + 1) * mapSize.x + pos.x + 1)] != 0) {
				return true;
			}
		}
	}
	return false;
}

bool Map::collisionMoveDown(const glm::ivec2& pos) const
{
	int x, y;
	x = pos.x;
	y = pos.y;
	if (map[pos.y * mapSize.x + pos.x] != 0) { //si hay bloque debajo nuestro
		return true;
	}
	else {
		if ((map[pos.y * mapSize.x + pos.x] == 0) && (map[pos.y * mapSize.x + pos.x] == 0)) { //si el bloque de la derecha esta vacio
			if (map[pos.y * mapSize.x + pos.x + 1] != 0) { //miramos si el bloque de abajo derecha nuestro existe
				return true;
			}
		}
	}
	return false;
}