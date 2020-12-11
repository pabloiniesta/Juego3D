#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"
#include "AssimpModel.h"
#include "Player.h"
#include "Map.h"
#include "GameObject.h"



#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Scene
{

public:
	Scene();
	~Scene();

	void init(int lvl);
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Level *level;
	AssimpModel *model;
	Map *mapa;
	Player *player;
	vector<GameObject> objects;

	void encenderLuz();

	bool CheckCollisionPlayerObject(Player& one, GameObject& two);
	pair<bool, pair<Direction, glm::ivec2>> CheckCollisionPlayerMuro(Player& one, GameObject& two); //mirar colision bola con objeto
	Direction VectorDirection(glm::vec2 target);

	int keys;
	int stage;
	float camaraXpos;
	bool god;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

