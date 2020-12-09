#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
using namespace std;

Map* Map::createMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	Map* map = new Map(levelFile, minCoords, program);

	return map;
}

Map::Map(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

Map::~Map()
{
	if (map != NULL)
		delete map;
}

void Map::render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram)
{
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	AssimpModel* modelCubo = new AssimpModel();
	modelCubo->loadFromFile("models/cubocara.obj", shaderProgram);
	for (int i = 0; i < posCubos.size();++i) {
		
		float scaleFactor = 1.f / modelCubo->getHeight();
		glm::vec3 centerModelBase = modelCubo->getCenter() - glm::vec3(0.f, -modelCubo->getHeight() / 2.f, 0.f);

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
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;
	
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = tile - int('0');
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
	glm::vec2 posTile;
	
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				posCubos.push_back(posTile);
				
			}
		}
	}
}
