#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "AssimpModel.h"
#include "Map.h"


class Player {

public:

	void init(ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram);
	void setPosition(const glm::vec2 &pos);

	void setMap(Map* mapa);

	glm::vec2 posPlayer;
	glm::vec2 sizePlayer;
	glm::vec2 velPlayer;
	
	AssimpModel* modelPlayer;
private:
	Map* map;
};

#endif

