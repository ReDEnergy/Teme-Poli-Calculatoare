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

#include "key_mapping.h"

glm::mat4 Projection = glm::perspective(66.0f, 1.6f, 0.1f, 5000.0f);
glm::mat4 LightProjection = glm::ortho(-80.0f, 80.0f, -50.0f, 50.0f, 0.01f, 500.0f);

Camera *camera;
Light *lights;
Light phong_light;

Object3D BMan;
Object3D Box;
Object3D Tree;

Shader *DebugSP = new Shader();
Shader *Depth2Texture = new Shader();
Shader *PostProcessSP = new Shader();
Shader *Render2Texture = new Shader();

Texture TBlack;
Texture TWhite;
Texture TreeAlpha;

Terrain Grid;
FrameBuffer FBO1;
FrameBuffer FBO2;
FrameBuffer FBO_Depth;

Object3D ScreenQuad;
Object3D DebugSPView;
bool normal_map_computed = false;

void increseFactor(GLuint &factor, GLuint diff) {
	factor += diff;
}

void decreaseFactor(GLuint &factor, GLuint diff) {
	if (factor - diff <= 0)
		return;
	factor -= diff;
}

Game::Game() {

	camera = new Camera(20.0f);
	camera->init();
	camera->moveToPosition(glm::vec3(0.0f, 2.0f, 20.0f));

	// -------------------------------------------------------------------------
	// Load Shaders
	// -------------------------------------------------------------------------

	DebugSP->load("Shaders\\Debug_VS.glsl", "Shaders\\Debug_FS.glsl");
	Depth2Texture->load("Shaders\\Depth_to_Texture_VS.glsl", "Shaders\\Depth_to_Texture_FS.glsl");
	Render2Texture->load("Shaders\\Render_to_Texture_VS.glsl", "Shaders\\Render_to_Texture_FS.glsl");
	PostProcessSP->load("Shaders\\PostProcess_VS.glsl", "Shaders\\PostProcess_FS.glsl");

	// -------------------------------------------------------------------------
	// Create Objects
	// -------------------------------------------------------------------------

	phong_light.move(vec3_Up, 50);

	lights = new Light[3];
	lights[0].position = glm::vec3(20.0f, 5.0f, 30.0f);
	lights[0].move(vec3_Up, 0);

	lights[1].position = glm::vec3(-10.0f, 3.0f, -20.0f);
	lights[1].move(vec3_Up, 0);

	lights[2].position = glm::vec3(3.0f, 10.0f, 0.0f);
	lights[2].move(vec3_Up, 0);

	BMan = Object3D(Render2Texture, "Bman.obj");
	Tree = Object3D(Render2Texture, "bamboo.obj");
	Tree.position = glm::vec3(20.0f, 0.0, 0.0);

	Box = Object3D(Render2Texture, "ground.obj");
	Box.scale = glm::vec3(2);

	TWhite.load2D("Textures\\white.png");
	TBlack.load2D("Textures\\black.jpg");
	TreeAlpha.load2D("Textures\\bambooalpha.bmp");

	//Grid.generate(256, 256);
	//Grid.scale = glm::vec3(5.0f);

	// -------------------------------------------------------------------------
	// Load Textures
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Create FBO for rendering to texture
	// -------------------------------------------------------------------------

	ScreenQuad = Object3D(Render2Texture, "screen_quad.obj");
	DebugSPView = Object3D(PostProcessSP, "screen_quad.obj");
	DebugSPView.scale = glm::vec3(0.4f);
	DebugSPView.position = glm::vec3(0.6f);

	FBO1.generate(1920, 1080, 4);
	FBO2.generate(4096, 4096, 1);
	FBO_Depth.generate(4096, 4096, 0);
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

	glCullFace(GL_BACK);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0f);
	Engine::setDefaultFrameBuffer();

	// -------------------------------------------------------------------------
	// Render to Texture
	// -------------------------------------------------------------------------

	FBO_Depth.bind();
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glCullFace(GL_FRONT);

	glUseProgram(Depth2Texture->program);
	glUniformMatrix4fv(Depth2Texture->loc_view_matrix, 1, false, glm::value_ptr(phong_light.View));
	glUniformMatrix4fv(Depth2Texture->loc_projection_matrix, 1, false, glm::value_ptr(LightProjection));
	glUniform1i(Depth2Texture->loc_textures[1], 1);
	TWhite.bind(GL_TEXTURE1);
	BMan.render(Depth2Texture);
	TreeAlpha.bind(GL_TEXTURE1);
	Tree.render(Depth2Texture);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glCullFace(GL_BACK);

	// ---- ---- ---- ---- ----

	if (Engine::post_processing && !Engine::render_wire_frame) {
		FBO1.bind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
	else {
		Engine::setDefaultFrameBuffer();
	}

	// -------------------------------------------------------------------------
	// Render Scene
	// -------------------------------------------------------------------------
	glUseProgram(Render2Texture->program);
	glUniformMatrix4fv(Render2Texture->loc_view_matrix, 1, false, glm::value_ptr(camera->View));
	glUniformMatrix4fv(Render2Texture->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
	glUniformMatrix4fv(Render2Texture->loc_light_view_matrix, 1, false, glm::value_ptr(phong_light.View));
	glUniformMatrix4fv(Render2Texture->loc_light_projection_matrix, 1, false, glm::value_ptr(LightProjection));
	glUniform1i(Render2Texture->loc_textures[1], 1);

	glUniform3f(Render2Texture->loc_eye_pos, camera->eye.x, camera->eye.y, camera->eye.z);
	glUniform3f(Render2Texture->loc_light_pos, phong_light.position.x, phong_light.position.y, phong_light.position.z);
	FBO_Depth.bindDepthTexture(Render2Texture, 7);

	TreeAlpha.bind(GL_TEXTURE1);
	Tree.render(Render2Texture);

	TWhite.bind(GL_TEXTURE1);
	BMan.render(Render2Texture);
	Box.render(Render2Texture);

	TBlack.bind(GL_TEXTURE0);
	phong_light.render(Render2Texture);

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

		FBO1.bindAllTextures(PostProcessSP);
		FBO_Depth.bindDepthTexture(DebugSP, 7);
		ScreenQuad.render(PostProcessSP);
		glEnable (GL_DEPTH_TEST);
	}

	if (Engine::debug_mode) {

		glUseProgram(DebugSP->program);
		//FBO1.bindAllTextures(DebugSP);
		//FBO1.bindDepthTexture(DebugSP, 4);
		//DebugSPView.position = glm::vec3(0.6f);
		//DebugSPView.render(DebugSP);

		FBO_Depth.bindDepthTexture(DebugSP, 0);
		//DebugSPView.position = glm::vec3(-0.6f);
		DebugSPView.render(DebugSP);
	}
}

/** ----------------------------------------------------------------------------
 * Handle keyboard events
 * ---------------------------------------------------------------------------*/
void Game::keyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {
	if (key_pressed == 'c') camera->log();
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

	if (key_states['i']) increseFactor(Grid.displacement_factor, 1);
	if (key_states['o']) decreaseFactor(Grid.displacement_factor, 1);

	if (key_states['L']) camera->moveToPosition(phong_light.position);

	camera->computeViewMatrix();
}

/** ----------------------------------------------------------------------------
 * Handle special key events
 * ---------------------------------------------------------------------------*/

void Game::specialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {

	// Reload all shaders
	if(key_pressed == GLUT_KEY_F5) {
		DebugSP->reloadShaders();
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