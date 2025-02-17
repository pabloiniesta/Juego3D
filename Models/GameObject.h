#ifndef _GAMEOBJECT_INCLUDE
#define _GAMEOBJECT_INCLUDE

#include "AssimpModel.h"


class GameObject
{
public:

	void init(ShaderProgram& shaderProgram, char tipo, int x, int y, int lvl);
	void render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram);
	void colision();
	void encender(ShaderProgram& shaderProgram);
	void activarpincho(ShaderProgram& shaderProgram);
	void desactivarpincho(ShaderProgram& shaderProgram);
	void pulsarboton(ShaderProgram& shaderProgram);
	glm::vec2 posObject;
	glm::vec2 sizeObject;
	int hp;
	char tipoObject;
	bool encendido;
	bool activo;
	float scaling;

	AssimpModel* modelObject;

private:




};

#endif // _GAMEOBJECT_INCLUDE