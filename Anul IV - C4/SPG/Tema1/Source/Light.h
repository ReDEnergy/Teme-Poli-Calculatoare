#pragma once
#include "Globals.h"

enum LightType {
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT
};

class Light {

public:
	Light();
	~Light();

	void toggleState();
	void toggleState(bool state);
	void render();
	void move(const glm::vec3 dir, float delta_time);
	glm::mat4 Model;

	static void toggleDebug();

private:
	static bool debug;	
	static glm::vec3 ambient_color;
	bool enabled;	

public:

	LightType type;

	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;

};