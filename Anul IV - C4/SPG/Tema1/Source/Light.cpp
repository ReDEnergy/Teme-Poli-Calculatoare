#include "Light.h"

using namespace std;

glm::vec3 Light::ambient_color = glm::vec3(1.0f, 1.0f, 1.0f);
bool Light::debug = true;

Light::Light() 
{
	enabled = true;

	position = glm::vec3(0, 0, 0);
	direction = glm::vec3(0, -1, 0);

	glm::mat4 Model = mat4_Identity;

}

void Light::render() {
	if (debug)
		glutSolidSphere(0.2f, 16, 16);
}

void Light::toggleState() {
	enabled = !enabled;
}

void Light::toggleState(bool state) {
	enabled = state;
}

void Light::move(const glm::vec3 dir, float delta_time) {
	position += dir * delta_time * 5.0f;
	Model = glm::translate(mat4_Identity, position);
}

void Light::toggleDebug() {
	debug = !debug;
}