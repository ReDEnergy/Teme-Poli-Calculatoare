#pragma once
#include "Globals.h"

enum CameraType {
	FirstPerson,
	ThirdPerson,
	TopView
};

using namespace std;

class Camera {
	public:
		Camera(float distance);
		~Camera();

		void init();
		void computeViewMatrix();
		void Camera::log();

		// Rotatii FPS
		void rotateFPS_OX(float angle);
		void rotateFPS_OY(float angle);
		void rotateFPS_OZ(float angle);

		// Rotatii TPS
		void rotateTPS_OX(float angle);
		void rotateTPS_OY(float angle);
		void rotateTPS_OZ(float angle);

		// Rotatie generala
		void rotateOX(float delta);
		void rotateOY(float delta);

		// Zoom
		void moveCloser(float add);
		void switchView(int camera_type);

		// Speed
		void increaseSpeed();
		void decreaseSpeed();

		// Translatare dupa vectorii camerei
		void translateForward(float dist);
		void translateRight(float dist);
		void translateUp(float dist);
		void translateAfterVector(glm::vec3, float dist);

		// Translatare pe plan orizontal
		void moveForward(float time);
		void moveBackward(float time);
		void moveRight(float time);
		void moveLeft(float time);
		void moveUp(float time);
		void moveDown(float time);
		void moveToPosition(glm::vec3);

	public:
		// Vectori camera
		glm::mat4 View;
		glm::vec3 eye;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 target;

		// Mod vizualizare
		int type;

		// Unghiuri de rotatie
		float angle_OX;
		float angle_OY;

		// Distanta targe/zoom maxim/
		float const initial_dist;
		float target_dist;
		float target_max_dist;

		// Limite de rotatie
		float limit_up_FPS;
		float limit_up_TPS;
		float limit_up_TOP;
		float limit_down;

		// Viteza de deplasare si rotatie
		float move_speed;
		float min_speed;
		float max_speed;
		float sensivity_OX;
		float sensivity_OY;
};