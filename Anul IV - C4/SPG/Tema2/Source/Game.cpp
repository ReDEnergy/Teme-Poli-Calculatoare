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

glm::mat4 Projection = glm::perspective(66.0f, 16.0f / 10.0f, 0.1f, 5000.0f);



Camera *camera;
Light *lights;
Light phong_light;

Object3D BMan;

Shader *heightmap = new Shader();
Shader *post_process_sp = new Shader();
Shader *render_2_texture = new Shader();
Shader *height_2_normal_map = new Shader();

Texture height_map;
Texture terrain_texture;
Texture terrain_normals;

Terrain Grid;
FrameBuffer FBO1;
FrameBuffer FBO2;

Object3D FBO_Quad;
bool render2FrameBuffer = false;
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
	camera->moveToPosition(glm::vec3(0.0f, 2.0f, 10.0f));

	// -------------------------------------------------------------------------
	// Create Shaders
	// -------------------------------------------------------------------------

	heightmap->load("Shaders\\heightmap_vs.glsl", 
					"Shaders\\HeightMap_TessCtrl.glsl",
					"Shaders\\HeightMap_TessEval.glsl",
					"Shaders\\HeightMap_FS.glsl");
	render_2_texture->load("Shaders\\Render_to_Texture_VS.glsl", "Shaders\\Render_to_Texture_FS.glsl");
	post_process_sp->load("Shaders\\PostProcess_VS.glsl", "Shaders\\PostProcess_FS.glsl");
	height_2_normal_map->load("Shaders\\Height_to_NormalMap_VS.glsl", "Shaders\\Height_to_NormalMap_FS.glsl");


	// -------------------------------------------------------------------------
	// Create Objects
	// -------------------------------------------------------------------------

	phong_light.move(vec3_Up, 75);

	lights = new Light[3];
	lights[0].position = glm::vec3(20.0f, 5.0f, 30.0f);
	lights[0].move(vec3_Up, 0);

	lights[1].position = glm::vec3(-10.0f, 3.0f, -20.0f);
	lights[1].move(vec3_Up, 0);

	lights[2].position = glm::vec3(3.0f, 10.0f, 0.0f);
	lights[2].move(vec3_Up, 0);

	BMan = Object3D(render_2_texture, "Bman.obj");

	Grid.generate(256, 256);
	Grid.scale = glm::vec3(5.0f);

	// -------------------------------------------------------------------------
	// Load Textures
	// -------------------------------------------------------------------------

	height_map.load2D("Textures\\heightmap.png");
	terrain_texture.load2D("Textures\\explosion.jpg");
	terrain_normals.load2D("Textures\\terrain_normals.png");

	// -------------------------------------------------------------------------
	// Create FBO for rendering to texture
	// -------------------------------------------------------------------------

	FBO_Quad = Object3D(render_2_texture, "screen_quad.obj");
	FBO1.generate(1920, 1080, 4);
	FBO2.generate(4096, 4096, 1);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Engine::Window.width, Engine::Window.height);

	// -------------------------------------------------------------------------
	// Compute NormalMap from HeightMap
	// -------------------------------------------------------------------------

	if (!normal_map_computed) {
		normal_map_computed = true;
		FBO2.bind();

		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUseProgram(height_2_normal_map->program);
		glUniform1i(height_2_normal_map->loc_textures[0], 0);
		height_map.bind(GL_TEXTURE0);
		FBO_Quad.render(height_2_normal_map);

		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	// -------------------------------------------------------------------------
	// Render to Texture
	// -------------------------------------------------------------------------

	if (render2FrameBuffer) {
		FBO1.bind();

		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
	else
		glViewport(0, 0, Engine::Window.width, Engine::Window.height);


	// -------------------------------------------------------------------------
	// Render Scene
	// -------------------------------------------------------------------------
	glUseProgram(render_2_texture->program);

	glUniformMatrix4fv(render_2_texture->loc_view_matrix, 1, false, glm::value_ptr(camera->View));
	glUniformMatrix4fv(render_2_texture->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
	glUniform1i(render_2_texture->loc_textures[0], 0);

	BMan.render(render_2_texture);

	phong_light.render(render_2_texture);

	glUseProgram(heightmap->program);

	glUniformMatrix4fv(heightmap->loc_view_matrix, 1, false, glm::value_ptr(camera->View));
	glUniformMatrix4fv(heightmap->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
	glUniform3f(heightmap->loc_eye_pos, camera->eye.x, camera->eye.y, camera->eye.z);
	glUniform3f(heightmap->loc_light_pos, phong_light.position.x, phong_light.position.y, phong_light.position.z);
	for (int i=0; i<4; i++)
		glUniform1i(heightmap->loc_textures[i], i);

	height_map.bind(GL_TEXTURE0);
	terrain_texture.bind(GL_TEXTURE1);
	terrain_normals.bind(GL_TEXTURE2);
	if (normal_map_computed)
		FBO2.bindTexture(0, 3);

	Grid.render(heightmap);

	// -------------------------------------------------------------------------
	// Render to the default FrameBuffer
	// -------------------------------------------------------------------------

	if (render2FrameBuffer) {

		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, Engine::Window.width, Engine::Window.height);

		// NEED REVIEW
		//glEnable (GL_BLEND);			// could reject background frags!
		//glBlendEquation (GL_FUNC_ADD);
		//glBlendFunc (GL_ONE, GL_ONE);	// addition each time

		//glEnable(GL_CULL_FACE);

		//glDisable (GL_DEPTH_TEST);
		//glDepthMask (GL_FALSE);

		glUseProgram(post_process_sp->program);

		glUniformMatrix4fv(post_process_sp->loc_view_matrix, 1, false, glm::value_ptr(camera->View));
		glUniformMatrix4fv(post_process_sp->loc_projection_matrix, 1, false, glm::value_ptr(Projection));
		glUniform3f(post_process_sp->loc_eye_pos, camera->eye.x, camera->eye.y, camera->eye.z);
		glUniform2f(post_process_sp->loc_resolution, (float)Engine::Window.width, (float)Engine::Window.height);

		for (int i=0; i<4; i++) {
			glUniform1i(post_process_sp->loc_textures[i], i);
			FBO1.bindTexture(i, i);

		}

		//for (int i=0; i<3; i++) {
		//	glUniform3f(post_process_sp->loc_light_pos, lights[i].position.x, lights[i].position.y, lights[i].position.z);
		//	lights[i].render(post_process_sp);
		//}

		//Render DepthBuffer
		//fb_depth_texture.bind(GL_TEXTURE0);

		glUniformMatrix4fv(post_process_sp->loc_view_matrix, 1, false, glm::value_ptr(mat4_Identity));
		glUniformMatrix4fv(post_process_sp->loc_projection_matrix, 1, false, glm::value_ptr(mat4_Identity));
		FBO_Quad.render(post_process_sp);

		//glEnable (GL_DEPTH_TEST);
		//glDepthMask (GL_TRUE);
		//glDisable (GL_BLEND);

		glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
}

/** ----------------------------------------------------------------------------
 * Handle keyboard events
 * ---------------------------------------------------------------------------*/
void Game::keyPressed(unsigned char key_pressed, int mouse_x, int mouse_y) {

	if (key_pressed == 'c') camera->log();

	if (key_pressed == 'j') increseFactor(Grid.tess_inner_factor, 1);
	if (key_pressed == 'k') increseFactor(Grid.tess_outer_factor, 1);
	if (key_pressed == 'n') decreaseFactor(Grid.tess_inner_factor, 1);
	if (key_pressed == 'm') decreaseFactor(Grid.tess_outer_factor, 1);


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

	camera->computeViewMatrix();
}

/** ----------------------------------------------------------------------------
 * Handle special key events
 * ---------------------------------------------------------------------------*/

void Game::specialKeyPressed(int key_pressed, int mouse_x, int mouse_y) {

	if (key_pressed == GLUT_KEY_F4) {
		render2FrameBuffer = !render2FrameBuffer;
		cout << "Render to FrameBuffer: " << render2FrameBuffer << endl;
	}

	// Reload all shaders
	if(key_pressed == GLUT_KEY_F5) {
		heightmap->reloadShaders();
		post_process_sp->reloadShaders();
		render_2_texture->reloadShaders();
		height_2_normal_map->reloadShaders();
	}

	if(key_pressed == GLUT_KEY_F8)
		Light::toggleDebug();
}

void Game::onSpecialKeyHold(const bool* const key_states, float delta_time) {
	if (key_states[KEY::ARROW_TOP])		camera->rotateOX(-7);
	if (key_states[KEY::ARROW_BOTTOM])	camera->rotateOX( 7);
	if (key_states[KEY::ARROW_LEFT])	camera->rotateOY(-5);
	if (key_states[KEY::ARROW_RIGHT])	camera->rotateOY( 5);

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