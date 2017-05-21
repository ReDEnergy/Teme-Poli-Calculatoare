#include "Object3D.h"

Object3D::Object3D() {
}

Object3D::Object3D(Shader *shader_program, const string& mesh_file) {

	shader = shader_program;
	mesh = new Mesh();
	mesh->shader = shader;
	mesh->LoadMesh(mesh_file);

	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	scale = glm::vec3(1.0f);
	angle = 0;
	move_speed = 5;

}

Object3D::~Object3D() {
}

void Object3D::update() {
}

void Object3D::render() {
	computeModelMatrix();
	glUniformMatrix4fv(shader->loc_model_matrix, 1, false, glm::value_ptr(Model));
	mesh->Render();
}

void Object3D::render(Shader *render_shader) {
	computeModelMatrix();
	glUniformMatrix4fv(render_shader->loc_model_matrix, 1, false, glm::value_ptr(Model));
	mesh->Render();
}

void Object3D::computeModelMatrix() {
	Model = glm::translate(mat4_Identity, position);
	Model = glm::rotate(Model, angle, rotation);
	Model = glm::scale(Model, scale);
}