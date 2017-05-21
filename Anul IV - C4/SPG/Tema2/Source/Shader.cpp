#include "Shader.h"
#include "Globals.h"

Shader::Shader() {
}

Shader::~Shader()
{
	glDeleteProgram(program);
	shader_files.reserve(5);
}

void Shader::getUniforms() {

	loc_model_matrix	= glGetUniformLocation(program, "Model");
	loc_view_matrix		= glGetUniformLocation(program, "View");
	loc_projection_matrix = glGetUniformLocation(program, "Projection");

	loc_eye_pos	= glGetUniformLocation(program, "eye_position");
	loc_light_pos = glGetUniformLocation(program, "light_position");
	loc_resolution = glGetUniformLocation(program, "window_resolution");
	loc_material = glGetUniformBlockIndex(program, "Material");	

	loc_lod_factor = glGetUniformLocation(program, "lod_factor");
	loc_tess_inner_factor = glGetUniformLocation(program, "tess_inner_factor");
	loc_tess_outer_factor = glGetUniformLocation(program, "tess_outer_factor");
	loc_displacement_factor = glGetUniformLocation(program, "displacement_factor");

	char buffer[64];
	for (int i = 0; i < MAX_2D_TEXTURES; i++) {
		sprintf_s(buffer, "u_texture_%d", i);
		loc_textures[i]	 = glGetUniformLocation(program, buffer);
	}
}

void Shader::reloadShaders() {
	int size = (int)shader_files.size();

	glDeleteProgram(program);

	switch (size)
	{
		case 2:
			load(shader_files[0], shader_files[1]);
			break;
		case 3:
			load(shader_files[0], shader_files[1], shader_files[2]);
			break;
		case 4:
			load(shader_files[0], shader_files[1], shader_files[2], shader_files[3]);
			break;
		default:
			break;
	}
}

unsigned int Shader::load(const string vertex_shader_file, 
						  const string fragment_shader_file) {

	shader_files.clear();
	shader_files.push_back(string(vertex_shader_file));
	shader_files.push_back(string(fragment_shader_file));

	cout << "PROGRAM:" << endl;

	vector<unsigned int> shaders;
	shaders.push_back( Shader::createShader(vertex_shader_file, GL_VERTEX_SHADER));
	shaders.push_back( Shader::createShader(fragment_shader_file, GL_FRAGMENT_SHADER));

	program = Shader::createProgram(shaders);

	getUniforms();

	return program;
}

unsigned int Shader::load(const string vertex_shader_file,
						  const string geometry_shader_file,
						  const string fragment_shader_file)
{

	shader_files.clear();
	shader_files.push_back(string(vertex_shader_file));
	shader_files.push_back(string(geometry_shader_file));
	shader_files.push_back(string(fragment_shader_file));

	cout << "PROGRAM:" << endl;

	vector<unsigned int> shaders;
	shaders.push_back( Shader::createShader(vertex_shader_file, GL_VERTEX_SHADER));
	shaders.push_back( Shader::createShader(geometry_shader_file, GL_GEOMETRY_SHADER));
	shaders.push_back( Shader::createShader(fragment_shader_file, GL_FRAGMENT_SHADER));

	program = Shader::createProgram(shaders);
	getUniforms();
	return program;
}

unsigned int Shader::load(const string vertex_shader_file,
						const string tessellation_control_shader_file,
						const string tessellation_evaluation_shader_file,
						const string fragment_shader_file)
{
	
	shader_files.clear();
	shader_files.push_back(string(vertex_shader_file));
	shader_files.push_back(string(tessellation_control_shader_file));
	shader_files.push_back(string(tessellation_evaluation_shader_file));
	shader_files.push_back(string(fragment_shader_file));

	cout << "PROGRAM:" << endl;

	vector<unsigned int> shaders;
	shaders.push_back( createShader(vertex_shader_file, GL_VERTEX_SHADER));
	shaders.push_back( createShader(tessellation_control_shader_file, GL_TESS_CONTROL_SHADER));
	shaders.push_back( createShader(tessellation_evaluation_shader_file, GL_TESS_EVALUATION_SHADER));
	shaders.push_back( createShader(fragment_shader_file, GL_FRAGMENT_SHADER));

	program = Shader::createProgram(shaders);
	getUniforms();
	return program;
}

unsigned int Shader::createShader(const string &shader_file, GLenum shader_type) 
{
	string shader_code;
	ifstream file(shader_file.c_str(), ios::in);

	if(!file.good()) {
		cout << "\tCould not open file: " << shader_file << endl;
		terminate();
	}

	cout << "\tFILE = " << shader_file;

	file.seekg(0, ios::end);
	shader_code.resize((unsigned int)file.tellg());
	file.seekg(0, ios::beg);
	file.read(&shader_code[0], shader_code.size());
	file.close();

	int info_log_length = 0;
	int compile_result = 0;
	unsigned int gl_shader_object;
			
	// create new shader object
	gl_shader_object = glCreateShader(shader_type);				
	const char *shader_code_ptr = shader_code.c_str();
	const int shader_code_size = (int) shader_code.size();

	glShaderSource(gl_shader_object, 1, &shader_code_ptr, &shader_code_size);	
	glCompileShader(gl_shader_object);
	glGetShaderiv(gl_shader_object, GL_COMPILE_STATUS, &compile_result);					
			
	// LOG COMPILE ERRORS
	if(compile_result == GL_FALSE) {

		string str_shader_type = "";

		if(shader_type == GL_VERTEX_SHADER)
			str_shader_type="vertex shader";
		if(shader_type == GL_TESS_CONTROL_SHADER)
			str_shader_type="tess control shader";
		if(shader_type == GL_TESS_EVALUATION_SHADER)
			str_shader_type="tess evaluation shader";
		if(shader_type == GL_GEOMETRY_SHADER)
			str_shader_type="geometry shader";
		if(shader_type == GL_FRAGMENT_SHADER)
			str_shader_type="fragment shader";
		if(shader_type == GL_COMPUTE_SHADER)
			str_shader_type="compute shader";

		glGetShaderiv(gl_shader_object, GL_INFO_LOG_LENGTH, &info_log_length);		
		vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(gl_shader_object, info_log_length, NULL, &shader_log[0]);	

		cout << "\n-----------------------------------------------------\n";
		cout << "\nERROR: " << str_shader_type << "\n\n";
		cout << &shader_log[0] << "\n";
		cout << "-----------------------------------------------------" << endl;

		return 0;
	}

	cout << "\t ..... COMPILED " << endl;

	return gl_shader_object;
}

unsigned int Shader::createProgram(const vector<unsigned int> &shader_objects)
{
	int info_log_length = 0;
	int link_result = 0;
	vector<unsigned int>::const_iterator it;

	//build OpenGL program object and link all the OpenGL shader objects
	unsigned int gl_program_object = glCreateProgram();

	for(it = shader_objects.begin(); it != shader_objects.end(); it++)
		glAttachShader(gl_program_object, (*it));

	glLinkProgram(gl_program_object);												
	glGetProgramiv(gl_program_object, GL_LINK_STATUS, &link_result);

	// LOG LINK ERRORS
	if(link_result == GL_FALSE) {														

		glGetProgramiv(gl_program_object, GL_INFO_LOG_LENGTH, &info_log_length);		
		vector<char> program_log(info_log_length);
		glGetProgramInfoLog(gl_program_object, info_log_length, NULL, &program_log[0]);

		cout << "Shader Loader : LINK ERROR" << endl;
		cout << &program_log[0] << endl;

		return 0;
	}
			
	// Delete the shader objects because we do not need them any more 
	for(it = shader_objects.begin(); it != shader_objects.end(); it++)
		glDeleteShader(*it);
			
	return gl_program_object;
}