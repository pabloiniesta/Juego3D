#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <GL/glut.h>
#include "TexturedQuad.h"
#include "Texture.h"
#include "Level.h"

class Menu
{
public:
	Menu();
	~Menu() {}

	void init();
	void update(int deltaTime);
	void render();
	void activateWin();
	void setInstructions(bool value) { bInstructions = value; }

private:
	void initShaders();
	ShaderProgram texProgram;
	glm::mat4 projection;
	glm::mat4 modelview;
	glm::mat4 modelMatrix, viewMatrix;
	glm::mat3 normalMatrix;
	int index;
	int numOptions;
	bool bUpPressed, bDownPressed, bEnterPressed;
	bool bCredits; //Go to credits screen
	bool bInstructions; //Go to instructions scree
	bool bGameOver; //Go to game over screen
	bool bWin;
	Level* levelMenu;
	Level* levelInst;
	Level* levelCreds;
	Level* levelWin;

	TexturedQuad* mainTextureQuad; //Menu Background
	Texture mainTexture;

	TexturedQuad* instructionsQuad; //Instructions Background
	Texture instruccionsTex;

	TexturedQuad* creditsQuad; //Credits Background
	Texture creditsTex;

	TexturedQuad* youWinQuad; //Game over Background
	Texture winTex;
};
#endif // _MENU_INCLUDE