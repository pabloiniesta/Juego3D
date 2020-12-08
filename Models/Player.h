#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "AssimpModel.h"


class Player {

public:

	void init(ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram);
	void setPosition(const glm::fvec3 &pos);

	glm::vec3 posPlayer;
	glm::vec3 sizePlayer;

	AssimpModel* modelPlayer;

};

#endif

