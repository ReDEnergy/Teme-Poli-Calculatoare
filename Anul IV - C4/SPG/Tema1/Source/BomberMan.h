#pragma once
#include "Globals.h"
#include "Game.h"

using namespace std;

class BomberMan : public Game {

private:
	glm::mat4 Projection;

public:
	BomberMan();
	~BomberMan();

	void displayFrame(float elapsed_time, float delta_time);

	void onKeyHold(const bool* const, float);
	void onSpecialKeyHold(const bool* const, float);
	void specialKeyPressed(int key_pressed, int mouse_x, int mouse_y);

	void mouseDrag(int mouse_x, int mouse_y) {};
	void mouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y); 
	void mouseClick(int button, int state, int mouse_x, int mouse_y) {}
	void mouseScroll(int wheel, int direction, int mouse_x, int mouse_y);
};