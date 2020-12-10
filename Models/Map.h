#ifndef _MAP_INCLUDE
#define _MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AssimpModel.h"


class Map
{

public:
	// Tile maps can only be created inside an OpenGL context
	static Map* createMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	Map(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~Map();

	void render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram) const;
	void free();

	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2& pos) const;
	bool collisionMoveRight(const glm::ivec2& pos) const;
	bool collisionMoveUp(const glm::ivec2& pos) const;
	bool collisionMoveDown(const glm::ivec2& pos) const;


	

private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);


private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, tilesheetSize, mapSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;
	vector<glm::vec2> posCubos; //vector con las posiciones de los cubos


};


#endif // _TILE_MAP_INCLUDE