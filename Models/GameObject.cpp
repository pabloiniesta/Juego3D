#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_FORCE_RADIANS
#include <GL/glut.h>

void GameObject::init(ShaderProgram& shaderProgram, char tipo, int posx, int posy)
{
	if (tipo == 's') { //Star, 1hp.
		modelObject = new AssimpModel();
		modelObject->loadFromFile("models/star2.obj", shaderProgram);
		hp = 1;
		sizeObject = glm::vec2(1, 1);
		tipoObject = 's';
		posObject = glm::vec2((float)posx, (float)posy);
		
	}

}

void GameObject::colision() {
	--hp;
}



void GameObject::render(float currentTime, glm::mat4& viewMatrix, ShaderProgram& shaderProgram)
{
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	// Render loaded model
	float scaleFactor = 1.f / modelObject->getHeight();
	glm::vec3 centerModelBase = modelObject->getCenter() - glm::vec3(0.f, -modelObject->getHeight() / 2.f, 0.f);

	modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.5f * fabs(sinf(3.f * currentTime / 1000.f)), 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(posObject.x, posObject.y, 0)); //esta establece la pos del objeto

	//rotar modelo: cuanto quieres rotar y en que eje quieres hacerlo
	modelMatrix = glm::rotate(modelMatrix, currentTime / 1000.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -centerModelBase);

	shaderProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	shaderProgram.setUniformMatrix3f("normalmatrix", normalMatrix);

	modelObject->render(shaderProgram);
}
