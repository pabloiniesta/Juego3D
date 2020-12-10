#pragma once

#include "ShaderProgram.h"
#include <GL/glut.h>
#include "TexturedQuad.h"
#include "Texture.h"

class Menu
{
public:
	Menu();
	~Menu() {}

	void init();
	void update(int deltaTime);
	void render();
	void activateWin(int score, int money);
	void activateGameOver(int score);
	void setInstructions(bool value) { bInstructions = value; }

private:
	void initShaders();
	ShaderProgram texProgram;
	glm::mat4 projection;
	int index;
	int numOptions;
	bool bUpPressed, bDownPressed, bEnterPressed;
	bool bCredits; //Go to credits screen
	bool bInstructions; //Go to instructions scree
	bool bGameOver; //Go to game over screen
	bool bWin;

	TexturedQuad* mainTextureQuad; //Menu Background
	Texture mainTexture;

	TexturedQuad* instructionsQuad; //Instructions Background
	Texture instruccionsTex;

	TexturedQuad* creditsQuad; //Credits Background
	Texture creditsTex;

	TexturedQuad* youWinQuad; //Game over Background
	Texture winTex;
};
