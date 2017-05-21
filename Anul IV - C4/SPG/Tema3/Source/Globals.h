#pragma once
#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>

#include <glew.h>
#include <wglew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\matrix_transform.hpp>

const glm::vec3 vec3_Up			= glm::vec3( 0.0f,  1.0f,  0.0f);
const glm::vec3 vec3_Down		= glm::vec3( 0.0f, -1.0f,  0.0f);
const glm::vec3 vec3_Left		= glm::vec3(-1.0f,  0.0f,  0.0f);
const glm::vec3 vec3_Right		= glm::vec3( 1.0f,  0.0f,  0.0f);
const glm::vec3 vec3_Forward	= glm::vec3( 0.0f,  0.0f, -1.0f);
const glm::vec3 vec3_Backward	= glm::vec3( 0.0f,  0.0f,  1.0f);

const glm::mat4 mat4_Identity = glm::mat4(1.0f);
const glm::mat3 mat3_Identity = glm::mat3(1.0f);

const float half_sqrt_2		= 0.707106f;

#define ZERO_MEM(a)			memset(a, 0, sizeof(a))
#define SIZEOF_ARRAY(a)		(sizeof(a) / sizeof(a[0]))
#define SAFE_FREE(p)		if (p) {delete p; p = NULL;}
#define SAFE_FREE_ARRAY(p)	if (p) {delete[] p; p = NULL;}

#define GLExitIfError()															\
{																				\
	GLenum Error = glGetError();                                                \
																				\
	if (Error != GL_NO_ERROR) {													\
		printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);		\
		exit(0);																\
	}																			\
}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

inline void log_vec4(glm::vec4 v, const char* const info = "") {
	std::cout << info << ": " << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
}

inline void log_vec3(glm::vec3 v, const char* const info = "") {
	std::cout << info << ": " << v.x << " " << v.y << " " << v.z << std::endl;
}