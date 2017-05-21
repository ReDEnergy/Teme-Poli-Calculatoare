#include "Camera.h"

// Constructor camera
Camera::Camera(float distance)
	: target_dist(distance),
	initial_dist(distance)
{
}

Camera::~Camera() {}

// Initializare camera
void Camera::init() {

	eye		= glm::vec3(0.0f, 0.0f, 10.0f);
	forward	= glm::vec3(0.0f, 0.0f, -1.0f);
	right	= glm::vec3(1.0f, 0.0f, 0.0f);
	up		= glm::vec3(0.0f, 1.0f, 0.0f);

	angle_OX = 0;
	angle_OY = 0;

	limit_up_TPS =  0.95f * (float) M_PI / 2;
	limit_up_FPS = -0.95f * (float) M_PI / 2;
	limit_up_TOP =  0.99f * (float) M_PI / 2;
	limit_down	 = -0.95f * (float) M_PI / 2;

	type = CameraType::FirstPerson;

	move_speed = 25.0f;
	min_speed = 5.0f;
	max_speed = 500.0f;

	sensivity_OX = 0.003f;
	sensivity_OY = 0.003f;
	target_max_dist = 200.0f;	
}

/*
 *	Glut Camera Render
 */

void Camera::computeViewMatrix() {
	glm::vec3 center = eye + forward;
	View = glm::lookAt(eye, center, up);
}


void Camera::increaseSpeed() {
	if (move_speed > max_speed)
		return;
	move_speed += 1.0f;
}

void Camera::decreaseSpeed() {
	if (move_speed <= min_speed)
		return;
	move_speed -= 1.0f;
}


glm::vec3 vec3RotateOY(const glm::vec3 V, float angle) {
	glm::vec3 R;
	float rx = V.x * cos(angle) - V.z * sin(angle);
	float rz = V.x * sin(angle) + V.z * cos(angle);

	R.x = rx;
	R.y = V.y;
	R.z = rz;
	return R;
}

/*
 * First Person Camera View
 */
void Camera::rotateFPS_OY(float angle) {

	forward = vec3RotateOY(forward, angle);
	right = vec3RotateOY(right, angle);
	up = vec3RotateOY(up, angle);

	angle_OY += angle;
}

void Camera::rotateFPS_OX(float angle) {

	if (angle_OX + angle >= limit_up_TPS && type != TopView)
		return;

	if (angle_OX + angle <= limit_down && type == ThirdPerson)
		return;

	if (angle_OX + angle <= limit_up_FPS && type == FirstPerson)
		return;

	angle_OX += angle;

	up = up * cos(angle) + forward * sin(angle);
	forward = glm::cross(up, right);
}

void Camera::rotateFPS_OZ(float angle) {
	right = right * cos(angle) + up * sin(angle);
	up = glm::cross(right, forward);
}

/*
 * Move Camera Eye Poition
 */
void Camera::translateForward(float dist) {
	eye += forward * dist;
}

void Camera::translateRight(float dist) {
	eye += right * dist;
}

void Camera::translateUp(float dist) {
	eye += glm::vec3(0, 1, 0) * dist;
}

void Camera::translateAfterVector(glm::vec3 V, float dist) {
	eye += V * dist;
}

/*
 * 3rd Person Camera View
 */
void Camera::rotateTPS_OX(float angle) 
{
	translateForward(target_dist);
	rotateFPS_OX(angle);
	translateForward(-target_dist);
}

void Camera::rotateTPS_OY(float angle) 
{
	translateForward(target_dist);
	rotateFPS_OY(angle);
	translateForward(-target_dist);
}

void Camera::rotateTPS_OZ(float angle)
{
	translateForward(target_dist);
	rotateFPS_OZ(angle);
	translateForward(-target_dist);
}


/*
 * Move Camera after target
 */

void Camera::moveForward(float time) {
	glm::vec3 front = glm::vec3(forward);

	front.y = 0;
	front = glm::normalize(front);
	eye += front * time * move_speed;
}

void Camera::moveBackward(float time) {
	moveForward(-time);
}

void Camera::moveRight(float time) {
	glm::vec3 side = right;

	side.y = 0;
	side = glm::normalize(side);
	eye += side * time * move_speed;
}

void Camera::moveLeft(float time) {
	moveRight(-time);
}

void Camera::moveUp(float time) {
	translateUp(time * move_speed);
}

void Camera::moveDown(float time) {
	moveUp(-time);
}

// Repozitionare camera la coordonatele date
void Camera::moveToPosition(glm::vec3 pos) {

	glm::vec3 OX = glm::vec3(1, 0, 0);
	glm::vec3 OY = glm::vec3(0, 1, 0);
	glm::vec3 OZ = glm::vec3(0, 0, 1);

	int mode = type;

	switchView(FirstPerson);

	translateAfterVector(OX, pos.x - eye.x);
	translateAfterVector(OY, pos.y - eye.y);
	translateAfterVector(OZ, pos.z - eye.z);

	switchView(mode);
}


/*
 *	Update Camera View
 */

void Camera::rotateOX(float delta) {
	if (delta == 0)				return;
	if (type == TopView)		return;
	if (type == FirstPerson)	rotateFPS_OX(delta * sensivity_OX);
	if (type == ThirdPerson)	rotateTPS_OX(-delta * sensivity_OX);
}

void Camera::rotateOY(float delta) {
	if (delta == 0)				return;
	if (type == TopView)		rotateTPS_OY(-delta * sensivity_OY);
	if (type == FirstPerson)	rotateFPS_OY(delta * sensivity_OY);
	if (type == ThirdPerson)	rotateTPS_OY(-delta * sensivity_OY);
}

void Camera::moveCloser(float add) {
	if (type == FirstPerson)	
		return;

	if (target_dist + add <= 0.5f)
		return;

	if (target_dist + add > target_max_dist)
		return;

	target_dist += add;
	eye -= forward * add;
}


/*
 * Update Camera Mode
 */

void Camera::switchView(int camera_type)
{
	if (camera_type == type)
		return;

	if (camera_type == FirstPerson) {
		type = ThirdPerson;
		moveCloser(initial_dist - target_dist);
	}

	if (camera_type == ThirdPerson) {

		if (type == TopView) {
			type = ThirdPerson;
			rotateTPS_OX(-angle_OX + (float) M_PI/4);
			return;
		}

		type = FirstPerson;
	}

	// Switch to TopView
	if (camera_type == TopView) {

		if (type == ThirdPerson)
			eye = eye + forward * target_dist;

		type = TopView;

		rotateFPS_OX(-angle_OX + limit_up_TOP);
		rotateFPS_OY(-angle_OY);
		eye = eye - forward * target_dist;
		
		moveCloser(target_max_dist/2 - target_dist);
		return;
	}


	// Switch to FirstPerson
	if (type == ThirdPerson) {
		// glutSetCursor(0);
		type = FirstPerson;
		eye = eye + forward * target_dist;
		rotateFPS_OX(-angle_OX);
		return;
	}

	// Switch to ThirdPerson
	if (type == FirstPerson) {
		// glutSetCursor(GLUT_CURSOR_NONE);
		type = ThirdPerson;
		rotateFPS_OX(-angle_OX + (float) M_PI/16);
		eye = eye - forward * target_dist;
		return;
	}
}

void Camera::log() {
	cout << "Camera properties : " << endl;
	log_vec3(eye, "eye");
	log_vec3(forward, "forward");
	log_vec3(right, "right");
	log_vec3(up, "up");
	cout << "--------------------------------------------------" << endl;
}
