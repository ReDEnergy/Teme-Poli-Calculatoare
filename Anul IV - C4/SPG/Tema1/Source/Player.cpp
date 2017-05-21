#include "Player.h"

Player::Player(Shader *shader_program, const string& mesh_file)
	: Object3D(shader_program, mesh_file)
{
	state = false;
	alive = true;
}

Player::~Player() {
}

void Player::moveForward(float delta_time) {
	position += vec3_Forward * move_speed * delta_time;

	angle += (180 - angle) / 4;
}

void Player::moveBackword(float delta_time) {
	position += vec3_Backward * move_speed * delta_time;
	angle += (0 - angle) / 4;
}

void Player::moveLeft(float delta_time) {
	position += vec3_Left * move_speed * delta_time;
	angle += (270 - angle) / 4;
}

void Player::moveRight(float delta_time) {
	position += vec3_Right * move_speed * delta_time;
	angle += (90 - angle) / 4;
}

void Player::move(glm::vec3 dir, float delta_time) {
	if (dir == vec3_Forward)	moveForward(delta_time);
	if (dir == vec3_Backward)	moveBackword(delta_time);
	if (dir == vec3_Left)		moveLeft(delta_time);
	if (dir == vec3_Right)		moveRight(delta_time);
}

glm::vec3 Player::getExpectedPos(glm::vec3 dir, float delta_time) {
	dir.y = 0;
	dir = glm::normalize(dir);

	return position + dir * move_speed * delta_time;
}

void Player::moveToDestination(float delta_time) {
	position += move_direction * move_speed * delta_time;
	
	float dist = glm::distance(position, start_position);

	if (dist > 2.0f) {
		position = start_position + move_direction * 2.0f;
		cellX += (int)move_direction.x;
		cellY += (int)move_direction.z;
		state = false;
	}
}