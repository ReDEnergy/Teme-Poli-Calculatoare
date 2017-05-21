#include "BomberMan.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Object3D.h"
#include "Map.h"
#include "Player.h"

Shader *shader;
Camera *camera;
Light *light;
Map *GameMap;
Object3D *Ground;

BomberMan::BomberMan() {

	camera = new Camera(20.0f);
	camera->init();
	camera->moveUp(3);
	camera->moveForward(1.5f);
	camera->moveRight(0.5f);
	camera->rotateOX(480);

	Projection = glm::perspective(66.0f, 16.0f / 9.0f, 0.01f, 5000.0f);

	shader = new Shader();
	shader->load("Shaders\\shader_vs.glsl", "Shaders\\shader_fs.glsl");
	shader->init();

	light = new Light();
	light->move(vec3_Up, 3);

	GameMap = new Map(15, 15);
	GameMap->genBombermanMap();
	GameMap->print();
	GameMap->init(shader);

	Ground = new Object3D(shader, "ground.obj");
}

BomberMan::~BomberMan() {
	delete shader;
}

void BomberMan::displayFrame(float elapsed_time, float delta_time) {

	GameMap->update(elapsed_time, delta_time);

	//foloseste shaderul
	glUseProgram(shader->program);

	//trimite variabile uniforme la shader
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->View));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
	glUniform3f(shader->loc_eye_position, camera->eye.x, camera->eye.y, camera->eye.z);
	glUniform3f(shader->loc_light_position, light->position.x, light->position.y, light->position.z);
	glUniform1i(shader->loc_textures[0], 0);

	GameMap->render();
	Ground->render();

	glUniformMatrix4fv(shader->loc_model_matrix, 1, false, glm::value_ptr(light->Model));
	light->render();
}

void BomberMan::specialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {
	if(key_pressed == GLUT_KEY_F5) {
		cout << "=====================================================" << endl;
		glDeleteProgram(shader->program);
		shader->load("Shaders\\shader_vs.glsl", "Shaders\\shader_fs.glsl");
	}

	if(key_pressed == GLUT_KEY_F8)
		light->toggleDebug();
}

void BomberMan::onKeyHold(const bool* const key_states, float delta_time) {

	if (key_states['i']) camera->moveForward(delta_time);
	if (key_states['k']) camera->moveBackward(delta_time);
	if (key_states['j']) camera->moveLeft(delta_time);
	if (key_states['l']) camera->moveRight(delta_time);
	if (key_states['u']) camera->moveDown(delta_time);
	if (key_states['o']) camera->moveUp(delta_time);
	if (key_states['c']) camera->log();

	if (key_states['8']) light->move(vec3_Forward, delta_time);
	if (key_states['2']) light->move(vec3_Backward, delta_time);
	if (key_states['6']) light->move(vec3_Right, delta_time);
	if (key_states['4']) light->move(vec3_Left, delta_time);
	if (key_states['9']) light->move(vec3_Up, delta_time);
	if (key_states['3']) light->move(vec3_Down, delta_time);

	if (key_states['w']) {
		if (GameMap->moveHero(vec3_Forward, delta_time) == false) {
			camera->moveForward(delta_time);
			light->move(vec3_Forward, delta_time);
		}
	}

	if (key_states['s']) {
		if (GameMap->moveHero(vec3_Backward, delta_time) == false) {
			camera->moveBackward(delta_time);
			light->move(vec3_Backward, delta_time);
		}
	}

	if (key_states['a']) {
		if (GameMap->moveHero(vec3_Left, delta_time) == false) {
			camera->moveLeft(delta_time);
			light->move(vec3_Left, delta_time);
		}
	}
	if (key_states['d']) {
		if (GameMap->moveHero(vec3_Right, delta_time) == false) {
			camera->moveRight(delta_time);
			light->move(vec3_Right, delta_time);
		}
	}

	if (key_states[' '])
		GameMap->placeBomb(GameMap->hero->position);

	camera->computeViewMatrix();
}

void BomberMan::onSpecialKeyHold(const bool* const key_states, float delta_time) {
	if (key_states[100]) camera->rotateOY(-5);
	if (key_states[101]) camera->rotateOX(-7);
	if (key_states[103]) camera->rotateOX(7);
	if (key_states[102]) camera->rotateOY(5);

	camera->computeViewMatrix();
}

void BomberMan::mouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) {
	if (delta_x)	camera->rotateOY((float)delta_x);
	if (delta_y)	camera->rotateOX((float)delta_y);
}

void BomberMan::mouseScroll(int wheel, int direction, int mouse_x, int mouse_y) {
	if (direction)
		camera->moveDown(direction / 10.0f);
	else
		camera->moveUp(direction / 10.0f);
}