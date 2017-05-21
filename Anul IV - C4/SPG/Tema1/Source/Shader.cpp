#include "Shader.h"
#include "Globals.h"

Shader::Shader() {
}

Shader::~Shader()
{
	glDeleteProgram(program);
	// glDeleteShader();
	SAFE_FREE_ARRAY(states);
}

void Shader::init() {

	loc_model_matrix	= glGetUniformLocation(program, "Model");
	loc_view_matrix		= glGetUniformLocation(program, "View");
	loc_projection_matrix = glGetUniformLocation(program, "Projection");
	loc_textures[0]		= glGetUniformLocation(program, "u_texture");
	loc_eye_position	= glGetUniformLocation(program, "eye_position");	
	loc_light_position	= glGetUniformLocation(program, "light_position");	
	loc_material		= glGetUniformBlockIndex(program, "Material");	
}

unsigned int Shader::load(const string &vertex_shader_file, 
						  const string &fragment_shader_file) {
	
	vector<unsigned int> shaders;
	shaders.push_back( Shader::createShader(vertex_shader_file, GL_VERTEX_SHADER));
	shaders.push_back( Shader::createShader(fragment_shader_file, GL_FRAGMENT_SHADER));

	cout << "Shader Loader : " << endl;
	cout << "\tvertex shader = " << vertex_shader_file << endl;
	cout << "\tframgnet shader = " << fragment_shader_file << endl;
	
	program = Shader::createProgram(shaders);
	return program;
}

unsigned int Shader::load(const string &vertex_shader_file,
						  const string &geometry_shader_file,
						  const string &fragment_shader_file)
{
	vector<unsigned int> shaders;
	shaders.push_back( Shader::createShader(vertex_shader_file, GL_VERTEX_SHADER));
	shaders.push_back( Shader::createShader(geometry_shader_file, GL_GEOMETRY_SHADER));
	shaders.push_back( Shader::createShader(fragment_shader_file, GL_FRAGMENT_SHADER));

	cout << "Shader Loader : " << endl;
	cout << "\tvertex shader = " << vertex_shader_file << endl;
	cout << "\tgeometry shader = " << geometry_shader_file << endl;
	cout << "\tframgnet shader = " << fragment_shader_file << endl;

	program = Shader::createProgram(shaders);
	return program;
}

unsigned int Shader::createShader(const string &shader_file, GLenum shader_type) 
{
	string shader_code;
	ifstream file(shader_file.c_str(), ios::in);

	if(!file.good()) {
		cout << "Shader Loader: Could not open file: " << shader_file << endl;
		terminate();
	}

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

		cout << "Shader Loader: COMPILE ERROR" << str_shader_type << endl;
		cout << &shader_log[0] << endl;

		return 0;
	}

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