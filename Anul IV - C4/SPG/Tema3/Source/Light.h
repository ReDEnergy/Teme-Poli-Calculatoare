#pragma once
#include "Globals.h"
#include "Shader.h"

enum LightType {
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT
};

class Light {

public:
	Light();
	~Light();

	void render(Shader *shader);
	void move(const glm::vec3 dir, float delta_time);
	void computeViewMatrix();

	static void toggleDebug();

private:
	static bool debug;	
	static glm::vec3 ambient_color;
	bool enabled;	

public:
	LightType type;
	glm::mat4 Model;
	glm::mat4 View;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 side;

};