#include "Engine.h"
#include "Game.h"

#include "Shader.h"
#include "Texture.h"

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Object3D.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include "stb_image_write.hpp"
#include "HoughTransform.h"

#include "key_mapping.h"

glm::mat4 Projection = glm::perspective(40.0f, 1.6f, 0.1f, 5000.0f);
glm::mat4 LightProjection = glm::ortho(-80.0f, 80.0f, -50.0f, 50.0f, 0.01f, 500.0f);

Camera *camera;
Light phong_light;

Object3D Box;
Object3D Ground;
Object3D Tree;

Shader *DebugSP = new Shader();
Shader *SobelSP = new Shader();
Shader *GuassianSP = new Shader();
Shader *GrayscaleSP = new Shader();
Shader *NonMaxSurpSP = new Shader();
Shader *PostProcessSP = new Shader();
Shader *Render2Texture = new Shader();

Texture T4;
Texture TBlack;
Texture TWhite;
Texture TreeAlpha;

Terrain Grid;
FrameBuffer FBO;
FrameBuffer FBO_F1;
FrameBuffer FBO_F2;

Object3D ScreenQuad;
Object3D DebugSPView;
bool screenshot = false;
bool hough = true;

unsigned char* ss_image;

//int N = 1140;
//int M = 1560;

int N = 900;
int M = 1200;

FILE *F1;

Game::Game() {

	camera = new Camera(20.0f);
	camera->init();
	camera->moveToPosition(glm::vec3(0.0f, 2.0f, 50.0f));

	// -------------------------------------------------------------------------
	// Load Shaders
	// -------------------------------------------------------------------------

	GrayscaleSP->load("Shaders\\Filters\\Screen_VS.glsl", "Shaders\\Filters\\Grayscale_FS.glsl");
	GuassianSP->load("Shaders\\Filters\\Screen_VS.glsl", "Shaders\\Filters\\Guassian_FS.glsl");
	SobelSP->load("Shaders\\Filters\\Screen_VS.glsl", "Shaders\\Filters\\Sobel_FS.glsl");
	NonMaxSurpSP->load("Shaders\\Filters\\Screen_VS.glsl", "Shaders\\Filters\\Non_Max_Surp_FS.glsl");
	DebugSP->load("Shaders\\Debug_VS.glsl", "Shaders\\Debug_FS.glsl");
	Render2Texture->load("Shaders\\Render_to_Texture_VS.glsl", "Shaders\\Render_to_Texture_FS.glsl");
	PostProcessSP->load("Shaders\\PostProcess_VS.glsl", "Shaders\\PostProcess_FS.glsl");

	// -------------------------------------------------------------------------
	// Create Objects
	// -------------------------------------------------------------------------

	phong_light.move(vec3_Up, 50);

	Box = Object3D(Render2Texture, "concrete_wall.obj");
	Tree = Object3D(Render2Texture, "bamboo.obj");
	Tree.position = glm::vec3(20.0f, 0.0, 0.0);

	Ground = Object3D(Render2Texture, "ground.obj");
	Ground.scale = glm::vec3(2);

	// -------------------------------------------------------------------------
	// Load Textures
	// -------------------------------------------------------------------------

	T4.load2D("Textures\\tema4p.png");
	TWhite.load2D("Textures\\white.png");
	TBlack.load2D("Textures\\black.jpg");
	TreeAlpha.load2D("Textures\\bambooalpha.bmp");

	// -------------------------------------------------------------------------
	// Create FBO for rendering to texture
	// -------------------------------------------------------------------------

	ScreenQuad = Object3D(PostProcessSP, "screen_quad.obj");

	DebugSPView = Object3D(PostProcessSP, "screen_quad.obj");
	DebugSPView.scale = glm::vec3(0.4f);
	DebugSPView.position = glm::vec3(0.6f);

	FBO.generate(1920, 1080, 4);
	FBO_F1.generate(M, N, 2);
	FBO_F2.generate(M, N, 2);

	ss_image = new GLubyte[3 * N * M];
	HoughTransform::init(N, M);

	Engine::post_processing = true;

}


/**
 * Delete everthing
 */
Game::~Game() {
}

/**
 * Render Frame
 */
void Game::displayFrame(float elapsed_time, float delta_time) {

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0f);
	Engine::setDefaultFrameBuffer();

	// -------------------------------------------------------------------------
	// Render to Texture
	// -------------------------------------------------------------------------

	if (Engine::post_processing && !Engine::render_wire_frame) {
		FBO.bind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
	else {
		Engine::setDefaultFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// -------------------------------------------------------------------------
	// Render Scene
	// -------------------------------------------------------------------------
	glUseProgram(Render2Texture->program);
	glUniformMatrix4fv(Render2Texture->loc_view_matrix, 1, false, glm::value_ptr(camera->View));
	glUniformMatrix4fv(Render2Texture->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
	glUniform2f(Render2Texture->loc_resolution, (float)Engine::Window.width, (float)Engine::Window.height);

	glUniform1i(Render2Texture->loc_textures[1], 1);

	glUniform3f(Render2Texture->loc_eye_pos, camera->eye.x, camera->eye.y, camera->eye.z);
	glUniform3f(Render2Texture->loc_light_pos, phong_light.position.x, phong_light.position.y, phong_light.position.z);

	TreeAlpha.bind(GL_TEXTURE1);
	Tree.render(Render2Texture);

	TWhite.bind(GL_TEXTURE1);
	Ground.render(Render2Texture);
	Box.render(Render2Texture);

	TBlack.bind(GL_TEXTURE0);
	phong_light.render(Render2Texture);

	// -------------------------------------------------------------------------
	// Apply Grayscale Filtering
	// -------------------------------------------------------------------------

	FBO_F1.bind();
	glUseProgram(GrayscaleSP->program);
	glUniform2f(GrayscaleSP->loc_resolution, (float)M, (float)N);
	glUniform1i(GrayscaleSP->loc_textures[1], 1);
	T4.bind(GL_TEXTURE0);
	FBO.bindTexture(1, GL_TEXTURE1);
	ScreenQuad.render(GrayscaleSP);

	// -------------------------------------------------------------------------
	// Apply Guassian Filtering
	// -------------------------------------------------------------------------

	FBO_F2.bind();
	glUseProgram(GuassianSP->program);
	glUniform2f(GuassianSP->loc_resolution, (float)M, (float)N);
	FBO_F1.bindTexture(0, GL_TEXTURE0);
	ScreenQuad.render(GuassianSP);

	// -------------------------------------------------------------------------
	// Apply Sobel Filtering
	// -------------------------------------------------------------------------

	FBO_F1.bind();
	glUseProgram(SobelSP->program);
	glUniform2f(SobelSP->loc_resolution, (float)M, (float)N);
	FBO_F2.bindTexture(0, GL_TEXTURE0);
	ScreenQuad.render(SobelSP);

	// -------------------------------------------------------------------------
	// Apply Non-Maximum Surpression
	// -------------------------------------------------------------------------

	FBO_F2.bind();
	glUseProgram(NonMaxSurpSP->program);
	glUniform2f(NonMaxSurpSP->loc_resolution, (float)M, (float)N);
	FBO_F1.bindAllTextures(NonMaxSurpSP);
	ScreenQuad.render(NonMaxSurpSP);


	// -------------------------------------------------------------------------
	// Apply Hough Transform
	// -------------------------------------------------------------------------

	if (screenshot) {
		screenshot = false;

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0,  0,  M,  N, GL_RGB, GL_UNSIGNED_BYTE, ss_image);
		CheckOpenGLError();

		//int res = stbi_write_bmp("screenshot.bmp", M, N, 3, ss_image);
		//cout << "Image Write: " << res << endl;

		HoughTransform::detectLines(ss_image, 75, 105);
		cout << "Hough Line Transform Finished" << endl;

		HoughTransform::detectCircles(ss_image, 65, 100);

		HoughTransform::solveTheMostStupidHomeworkEver(ss_image);
		HoughTransform::generateTexture();

	}



	// -------------------------------------------------------------------------
	// Render to the default FrameBuffer
	// -------------------------------------------------------------------------

	if (Engine::post_processing && !Engine::render_wire_frame) {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Engine::Window.width, Engine::Window.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glDisable (GL_DEPTH_TEST);

		glUseProgram(PostProcessSP->program);
		glUniform2f(PostProcessSP->loc_resolution, (float)Engine::Window.width, (float)Engine::Window.height);

		if (hough) {
			HoughTransform::bindTexture(GL_TEXTURE0);
			FBO_F2.bindTexture(0, GL_TEXTURE1);
		}
		else 
			FBO_F2.bindAllTextures(PostProcessSP);

		ScreenQuad.render(PostProcessSP);
		glEnable (GL_DEPTH_TEST);
	}

	if (Engine::debug_mode) {
		glUseProgram(DebugSP->program);
		FBO_F1.bindAllTextures(DebugSP);
		//FBO.bindDepthTexture(DebugSP, 4);
		DebugSPView.render(DebugSP);
	}
}

/** ----------------------------------------------------------------------------
 * Handle keyboard events
 * ---------------------------------------------------------------------------*/
void Game::keyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {
	if (key_pressed == 'c') camera->log();
	if (key_pressed == 'h') hough = !hough;
	if (key_pressed == '.') { 
		HoughTransform::ThA_line += 10;
		cout << "ThA_line: " << HoughTransform::ThA_line << endl;
	}
	if (key_pressed == ',') {
		HoughTransform::ThA_line -= 10;
		cout << "ThA_line: " << HoughTransform::ThA_line << endl;
	}

	if (key_pressed == 'R')
		glutReshapeWindow(M, N);

	if (key_pressed == 'F')
		glutReshapeWindow(int(1.5 * M), int(1.5 * N));

}

void Game::onKeyHold(const bool* const key_states, float delta_time) {

	if (key_states['w']) camera->moveForward(delta_time);
	if (key_states['s']) camera->moveBackward(delta_time);
	if (key_states['a']) camera->moveLeft(delta_time);
	if (key_states['d']) camera->moveRight(delta_time);
	if (key_states['q']) camera->moveDown(delta_time);
	if (key_states['e']) camera->moveUp(delta_time);

	float move_speed = camera->move_speed * delta_time;

	if (key_states['8']) phong_light.move(glm::vec3(camera->forward.x, 0, camera->forward.z), move_speed);
	if (key_states['5']) phong_light.move(glm::vec3(-camera->forward.x, 0, -camera->forward.z), move_speed);
	if (key_states['6']) phong_light.move(glm::vec3(camera->right.x, 0, camera->right.z), move_speed);
	if (key_states['4']) phong_light.move(glm::vec3(-camera->right.x, 0, -camera->right.z), move_speed);
	if (key_states['9']) phong_light.move(vec3_Up, move_speed);
	if (key_states['7']) phong_light.move(vec3_Down, move_speed);

	if (key_states['+']) camera->increaseSpeed();
	if (key_states['-']) camera->decreaseSpeed();

	if (key_states['L']) camera->moveToPosition(phong_light.position);

	camera->computeViewMatrix();
}

/** ----------------------------------------------------------------------------
 * Handle special key events
 * ---------------------------------------------------------------------------*/

void Game::specialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {

	if(key_pressed == GLUT_KEY_F2) {
		screenshot = true;
	}

	// Reload all shaders
	if(key_pressed == GLUT_KEY_F5) {
		DebugSP->reloadShaders();
		SobelSP->reloadShaders();
		GuassianSP->reloadShaders();
		GrayscaleSP->reloadShaders();
		NonMaxSurpSP->reloadShaders();
		PostProcessSP->reloadShaders();
		Render2Texture->reloadShaders();
	}

	if(key_pressed == GLUT_KEY_F8)
		Light::toggleDebug();
}

void Game::onSpecialKeyHold(const bool* const key_states, float delta_time) {
	if (key_states[KEY::ARROW_TOP])		camera->rotateOX(-700 * delta_time);
	if (key_states[KEY::ARROW_BOTTOM])	camera->rotateOX( 700 * delta_time);
	if (key_states[KEY::ARROW_LEFT])	camera->rotateOY(-500 * delta_time);
	if (key_states[KEY::ARROW_RIGHT])	camera->rotateOY( 500 * delta_time);

	camera->computeViewMatrix();
}

/** ----------------------------------------------------------------------------
 * Handle mouse events
 * ---------------------------------------------------------------------------*/

void Game::mouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) {
	if (delta_x)	camera->rotateOY((float)delta_x);
	if (delta_y)	camera->rotateOX((float)delta_y);
}

void Game::mouseScroll(int wheel, int direction, int mouse_x, int mouse_y) {
	if (direction)
		camera->moveDown(direction / 10.0f);
	else
		camera->moveUp(direction / 10.0f);
}