#pragma once
#include <vector>
#include <fstream>
#include <string>

#include "Globals.h"

#define max_2D_textures 8

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

	void init();

	unsigned int load(const string &vertex_shader_file,
				const string &fragment_shader_file);

	unsigned int load(const string &vertex_shader_file, 
				const string &geometry_shader_file, 
				const string &fragment_shader_file);

private:
	static unsigned int createShader(const string &shader_file, GLenum shader_type);
	static unsigned int createProgram(const vector<unsigned int> &shader_objects);

public:
	GLuint program;

	GLint loc_textures[max_2D_textures];
	GLint loc_model_matrix;
	GLint loc_view_matrix;
	GLint loc_projection_matrix;

	GLint loc_eye_position;
	GLint loc_light_position;
	GLint loc_material;

	GLint nr_states; 
	GLuint *states;

};