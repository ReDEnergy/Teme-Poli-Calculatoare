#pragma once

class World {
public:
	World() {}
	virtual ~World() {}

	virtual void windowReshape(int width, int height) {}

	virtual void beginFrame() {}
	virtual void displayFrame(float elapsed_time, float delta_time) {}
	virtual void endFrame() {};

	virtual void keyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {}
	virtual void keyReleased(unsigned char key_released, int mouse_x, int mouse_y) {}
	virtual void onKeyHold(const bool* const key_states, float delta_time) {}

	virtual void specialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {}
	virtual void specialKeyReleased(int key_released, int mouse_x, int mouse_y) {}
	virtual void onSpecialKeyHold(const bool* const special_key_states, float delta_time) {}

	virtual void mouseDrag(int mouse_x, int mouse_y) {}
	virtual void mouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) {}
	virtual void mouseClick(int button, int state, int mouse_x, int mouse_y) {}
	virtual void mouseScroll(int wheel, int direction, int mouse_x, int mouse_y) {}
};