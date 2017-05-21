#include "Light.h"

using namespace std;

glm::vec3 Light::ambient_color = glm::vec3(1.0f, 1.0f, 1.0f);
bool Light::debug = true;

Light::Light() 
{
	enabled = true;

	position = glm::vec3(1.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	side = glm::vec3(1.0f, 0.0f, 0.0f);

	Model = mat4_Identity;
	View = mat4_Identity;
}

Light::~Light() {}

void Light::render(Shader *shader) {
	if (!debug)
		return;

	glUniformMatrix4fv(shader->loc_model_matrix, 1, false, glm::value_ptr(Model));
	glutSolidSphere(1.0f, 32, 32);
}

void Light::move(const glm::vec3 dir, float delta_time) {
	position += glm::normalize(dir) * delta_time;
	Model = glm::translate(mat4_Identity, position);
	computeViewMatrix();
}

void Light::toggleDebug() {
	debug = !debug;
}

void Light::computeViewMatrix() {
	glm::vec3 center = glm::vec3(0.0);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	side = glm::cross(direction, up); 
	View = glm::lookAt(position, center, up);
}