#pragma once
#include <fstream>
#include <vector>
#include <string>

#include "Globals.h"

#define MAX_2D_TEXTURES 8
#define MAX_LIGHTS 64
#define INVALID_LOC -1

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

	unsigned int load(const string vertex_shader_file,
				const string fragment_shader_file);

	unsigned int load(const string vertex_shader_file, 
				const string geometry_shader_file, 
				const string fragment_shader_file);

	unsigned int load(const string vertex_shader_file,
					const string tessellation_control_shader_file,
					const string tessellation_evaluation_shader_file,
					const string fragment_shader_file);

	void reloadShaders();

private:
	void getUniforms();

	static unsigned int createShader(const string &shader_file, GLenum shader_type);
	static unsigned int createProgram(const vector<unsigned int> &shader_objects);

public:
	GLuint program;

	GLint loc_textures[MAX_2D_TEXTURES];
	GLint loc_model_matrix;
	GLint loc_view_matrix;
	GLint loc_projection_matrix;

	GLint loc_light_pos;
	GLint loc_light_color;
	GLint loc_light_view_matrix;
	GLint loc_light_projection_matrix;

	GLint loc_eye_pos;
	GLint loc_material;
	GLint loc_resolution;
	GLint loc_use_alpha_mask;
	GLint loc_kernel_size;
	GLint loc_kernel;
	GLint loc_u_rad;

	GLint loc_tess_inner_factor;
	GLint loc_tess_outer_factor;
	GLint loc_displacement_factor;
	GLint loc_lod_factor;

private:
	vector<string> shader_files;
};