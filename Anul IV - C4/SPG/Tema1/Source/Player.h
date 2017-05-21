#pragma once

#include <fstream>
#include "Globals.h"
#include "Object3D.h"

/*
 * Class Player
 */

class Player : public Object3D {

public:
	Player(Shader *shader_program, const string& mesh_file);
	~Player();

	void moveForward(float delta_time);
	void moveBackword(float delta_time);
	void moveLeft(float delta_time);
	void moveRight(float delta_time);
	glm::vec3 getExpectedPos(glm::vec3 dir, float delta_time);
	void move(glm::vec3 dir, float delta_time);
	void moveToDestination(float delta_time);


public:
	bool state;
	bool alive;
	glm::vec3 move_direction;
	glm::vec3 start_position;
	int cellX;
	int cellY;
};