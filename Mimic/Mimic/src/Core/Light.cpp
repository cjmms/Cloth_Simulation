#include "Light.h"


Light::Light(glm::vec3 position, glm::vec3 intensity)
	:position(position), intensity(intensity)
{
	// disable material loading
	lightSphere = new Model("res/objects/sphere/sphere.obj");
}


Light::~Light()
{
	delete lightSphere;
}



glm::mat4 Light::getModelMatrix() const
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	return glm::scale(model, glm::vec3(0.5));
}