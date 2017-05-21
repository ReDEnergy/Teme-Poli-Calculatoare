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

#include "key_mapping.h"

glm::mat4 Projection = glm::perspective(40.0f, 1.6f, 0.1f, 5000.0f);
glm::mat4 LightProjection = glm::ortho(-80.0f, 80.0f, -50.0f, 50.0f, 0.01f, 500.0f);

Camera *camera;
Light phong_light;
Light *lights;

Object3D Box;
Object3D Ground;
Object3D Oildrum;
Object3D Billboard;
Object3D Bridge;
Object3D Drawer;
Object3D Teapot;

Shader *DebugSP = new Shader();
Shader *Composition = new Shader();
Shader *PostProcessSP = new Shader();
Shader *Render2Texture = new Shader();
Shader *SSAO = new Shader();
Shader *Blur = new Shader();
Shader *DLight = new Shader();

Texture TBlack;
Texture TWhite;
Texture RandT1;
Texture RandT2;
Texture Noise;

FrameBuffer FBO;
FrameBuffer FBO_Light;
FrameBuffer FBO_SSAO;
FrameBuffer FBO_Blur;

Object3D ScreenQuad;
Object3D DebugSPView;

bool screenshot = false;
unsigned char* ss_image;

glm::vec2 wsize = glm::vec2(1280, 720);

int kernelSize = 16;
float rad = 1.5;
glm::vec3 kernel[64];

float lerp(float v0, float v1, float t) {
	return v0+(v1-v0)*t;
}

Game::Game() {

	camera = new Camera(20.0f);
	camera->init();
	camera->moveToPosition(glm::vec3(0.0f, 2.0f, 50.0f));

	// -------------------------------------------------------------------------
	// Load Shaders
	// -------------------------------------------------------------------------

	DebugSP->load("Shaders\\Debug.VS.glsl", "Shaders\\Debug.FS.glsl");
	Render2Texture->load("Shaders\\Render_to_Texture.VS.glsl", "Shaders\\Render_to_Texture.FS.glsl");
	PostProcessSP->load("Shaders\\Screen.VS.glsl", "Shaders\\PostProcess.FS.glsl");
	SSAO->load("Shaders\\Screen.VS.glsl", "Shaders\\SSAO.FS.glsl");
	Composition->load("Shaders\\Screen.VS.glsl", "Shaders\\Composition.FS.glsl");
	Blur->load("Shaders\\Screen.VS.glsl", "Shaders\\Filters\\Blur.FS.glsl");
	DLight->load("Shaders\\Deferred.Light.VS.glsl", "Shaders\\Deferred.Light.FS.glsl");

	// -------------------------------------------------------------------------
	// Objects
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Lights
	
	phong_light.move(vec3_Up, 50);

	lights = new Light[100];

	float offset = 30;
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++) {
			lights[i * 10 + j].position = glm::vec3((i - 5) * offset + 15, 10.0f, (j - 5) *  offset + 15);
			lights[i * 10 + j].genRandomLightColor();
		}

	//lights[0].position = glm::vec3(-70.0f, 10.0f, -60.0f);
	//lights[0].move(vec3_Up, 0);
	//lights[0].genRandomLightColor();

	//lights[1].position = glm::vec3(-10.0f, 3.0f, -20.0f);
	//lights[1].move(vec3_Up, 0);
	//lights[1].genRandomLightColor();

	//lights[2].position = glm::vec3(-50.0f, 10.0f, 40.0f);
	//lights[2].move(vec3_Up, 0);
	//lights[2].genRandomLightColor();

	// -------------------------------------------------------------------------
	// Objects

	Box = Object3D(Render2Texture, "concrete_wall.obj");
	Box.position = glm::vec3(-100.0f, 0.0, -140.0f);
	Bridge = Object3D(Render2Texture, "bridge.obj");
	Bridge.position = glm::vec3(100.0f, 0.0, 0.0);
	Drawer = Object3D(Render2Texture, "Drawer.obj");
	Drawer.position = glm::vec3(-50.0f, 0.0, 0.0);
	Oildrum = Object3D(Render2Texture, "oildrum.obj");
	Oildrum.position = glm::vec3(20.0f, 0.0, 30.0f);
	Teapot =  Object3D(Render2Texture, "teapot.obj");
	Teapot.angle = 50;
	Teapot.rotation = glm::vec3(0, 1, 0);
	//Billboard = Object3D(Render2Texture, "billboard.obj");

	Ground = Object3D(Render2Texture, "ground.obj");
	Ground.scale = glm::vec3(5);


	// -------------------------------------------------------------------------
	// Load Textures
	// -------------------------------------------------------------------------

	TWhite.load2D("Textures\\white.png");
	TBlack.load2D("Textures\\black.jpg");
	RandT1.load2D("Textures\\noise.png", GL_REPEAT);
	RandT2.load2D("Textures\\radom.jpg", GL_REPEAT);

	// -------------------------------------------------------------------------
	// Create FBO for rendering to texture
	// -------------------------------------------------------------------------

	ScreenQuad = Object3D(PostProcessSP, "screen_quad.obj");
	DebugSPView = Object3D(PostProcessSP, "screen_quad.obj");
	DebugSPView.scale = glm::vec3(0.4f);
	DebugSPView.position = glm::vec3(0.6f);

	FBO.generate((int)wsize.x, (int)wsize.y, 5);
	FBO_Light.generate((int)wsize.x, (int)wsize.y, 1);
	FBO_SSAO.generate((int)wsize.x/2, (int)wsize.y/2, 1);
	FBO_Blur.generate((int)wsize.x/2, (int)wsize.y/2, 1);
	CheckOpenGLError();

	//ss_image = new GLubyte[3 * wsize.x * wsize.y];
	Engine::post_processing = true;

	// -------------------------------------------------------------------------
	// SSAO 

	float scale;
	for (int i = 0; i < kernelSize; ++i) {
		kernel[i] = glm::vec3(
			(float(rand()) / RAND_MAX) * 2 - 1.0f, 
			(float(rand()) / RAND_MAX) * 2 - 1.0f, 
			float(rand()) / RAND_MAX);
		kernel[i] = glm::normalize(kernel[i]);
		kernel[i] *= float(rand()) / RAND_MAX;

		scale = float(i) / kernelSize;
		scale = lerp(0.1f, 1.0f, scale * scale);
		kernel[i] *= scale;
	}

	float noise[3 * 16];

	for (int i = 0; i < kernelSize; ++i) {
		noise[3 * i] = (float(rand()) / RAND_MAX) * 2 - 1.0f;
		noise[3 * i + 1] = (float(rand()) / RAND_MAX) * 2 - 1.0f;
		noise[3 * i + 2] = 0.0f;
		noise[i] = glm::normalize(noise[i]);
	}

	Noise.create2DTextureFloat(noise, 4, 4, 3);
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

	//glClearColor(0.4f, 0.8f, 0.5f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

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
	glUniform3f(Render2Texture->loc_eye_pos, camera->eye.x, camera->eye.y, camera->eye.z);
	glUniform3f(Render2Texture->loc_light_pos, phong_light.position.x, phong_light.position.y, phong_light.position.z);
	glUniform1i(Render2Texture->loc_textures[1], 1);

	Ground.render(Render2Texture);

	int offset = 30;
	for (int i=-4; i<4; i++) {
		for (int j=-4; j<4; j++) {
			Teapot.position = glm::vec3(i * offset, 1.0f, j *  offset);
			Teapot.render(Render2Texture);
		}
	}

	Drawer.render(Render2Texture);
	Box.render(Render2Texture);
	Oildrum.render(Render2Texture);
	Bridge.render(Render2Texture);

	TBlack.bind(GL_TEXTURE0);
	phong_light.renderDebug(Render2Texture);
	for (int i = 0; i < 100; i++)
		lights[i].renderDebug(Render2Texture);


	// -------------------------------------------------------------------------
	// Render to the default FrameBuffer
	// -------------------------------------------------------------------------

	if (Engine::post_processing && !Engine::render_wire_frame) {

		// --------------------------------------------------------------------
		// Deferred Lighting
		FBO_Light.bind();
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glUseProgram(DLight->program);
		FBO_Light.sendResolution(DLight);
		glUniformMatrix4fv(DLight->loc_view_matrix, 1, false, glm::value_ptr(camera->View));
		glUniformMatrix4fv(DLight->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
		glUniform3f(DLight->loc_eye_pos, camera->eye.x, camera->eye.y, camera->eye.z);

		glUniform1i(DLight->loc_textures[0], 0);
		glUniform1i(DLight->loc_textures[1], 1);

		FBO.bindTexture(1, GL_TEXTURE0);
		FBO.bindTexture(2, GL_TEXTURE1);

		for (int i = 0; i < 100; i++)
			lights[i].renderDeferred(DLight);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		// --------------------------------------------------------------------
		// Screen Space Ambient Occlusion (SSAO)
		FBO_SSAO.bind();
		glDepthMask(GL_FALSE);
		glUseProgram(SSAO->program);
		FBO_SSAO.sendResolution(SSAO);
		glUniformMatrix4fv(SSAO->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
		glUniform1i(SSAO->loc_kernel_size, kernelSize);
		glUniform3fv(SSAO->loc_kernel, kernelSize * 3, glm::value_ptr(kernel[0]));
		glUniform1f(SSAO->loc_u_rad, rad);
		glUniform1i(SSAO->loc_textures[5], 5);
		glUniform1i(SSAO->loc_textures[6], 6);

		RandT1.bind(GL_TEXTURE5);
		Noise.bind(GL_TEXTURE6);
		FBO.bindAllTextures(SSAO);
		FBO.bindDepthTexture(SSAO, 7);
		ScreenQuad.render(SSAO);

		// --------------------------------------------------------------------
		// Blur the AO
		FBO_Blur.bind();
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(Blur->program);
		FBO_Blur.sendResolution(Blur);
		glUniform1i(Blur->loc_textures[0], 0);
		FBO_SSAO.bindTexture(0, GL_TEXTURE0);
		ScreenQuad.render(Blur);

		// --------------------------------------------------------------------
		// Render to the screen
		// Here goes what you see on the screen

		Engine::setDefaultFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glUseProgram(Composition->program);
		glUniform2f(Composition->loc_resolution, (float)Engine::Window.width, (float)Engine::Window.height);
		glUniform1i(Composition->loc_textures[5], 5);
		glUniform1i(Composition->loc_textures[6], 6);

		FBO.bindAllTextures(Composition);
		FBO_Blur.bindTexture(0, GL_TEXTURE5);
		FBO_Light.bindTexture(0, GL_TEXTURE6);
		FBO.bindDepthTexture(Composition, 7);

		ScreenQuad.render(Composition);
		// --------------------------------------------------------------------
	}

	if (Engine::debug_mode) {
		glUseProgram(DebugSP->program);
		glDisable(GL_DEPTH_TEST);
		FBO.bindAllTextures(DebugSP);
		glUniform1i(DebugSP->loc_textures[6], 6);
		FBO_Light.bindTexture(0, GL_TEXTURE6);
		FBO.bindDepthTexture(DebugSP, 7);
		DebugSPView.render(DebugSP);
		glEnable(GL_DEPTH_TEST);
	}

}

/** ----------------------------------------------------------------------------
 * Handle keyboard events
 * ---------------------------------------------------------------------------*/
void Game::keyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {
	if (key_pressed == 'c') camera->log();
	// glutReshapeWindow(M, N);

	if (key_pressed == 'i') {
		rad = rad * 1.1f;
		cout << "Radius : " << rad << endl;
	}
	if (key_pressed == 'o') {
		rad = rad / 1.1f;
		cout << "Radius : " << rad << endl;
	}
	if (key_pressed == 'j') {
		kernelSize -= 1;
		cout << "KernelSize : " << kernelSize << endl;
	}
	if (key_pressed == 'k') {
		kernelSize += 1;
		cout << "KernelSize : " << kernelSize << endl;
	}


	if (key_pressed == 'l') {
		cout << Engine::Window.width << " " << Engine::Window.height << endl;
	}

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
		PostProcessSP->reloadShaders();
		Render2Texture->reloadShaders();
		SSAO->reloadShaders();
		Composition->reloadShaders();
		Blur->reloadShaders();
		DLight->reloadShaders();
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