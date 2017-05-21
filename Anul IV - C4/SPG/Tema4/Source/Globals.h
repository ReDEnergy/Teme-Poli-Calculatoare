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

// -------------------------------------------------------------------------
#define ZERO_MEM(a)			memset(a, 0, sizeof(a))
#define SIZEOF_ARRAY(a)		(sizeof(a) / sizeof(a[0]))
#define SAFE_FREE(p)		if (p) {delete p; p = NULL;}
#define SAFE_FREE_ARRAY(p)	if (p) {delete[] p; p = NULL;}

const float half_sqrt_2		= 0.707106f;

// -------------------------------------------------------------------------

const glm::vec3 vec3_Up			= glm::vec3( 0.0f,  1.0f,  0.0f);
const glm::vec3 vec3_Down		= glm::vec3( 0.0f, -1.0f,  0.0f);
const glm::vec3 vec3_Left		= glm::vec3(-1.0f,  0.0f,  0.0f);
const glm::vec3 vec3_Right		= glm::vec3( 1.0f,  0.0f,  0.0f);
const glm::vec3 vec3_Forward	= glm::vec3( 0.0f,  0.0f, -1.0f);
const glm::vec3 vec3_Backward	= glm::vec3( 0.0f,  0.0f,  1.0f);

const glm::mat4 mat4_Identity = glm::mat4(1.0f);
const glm::mat3 mat3_Identity = glm::mat3(1.0f);

// -------------------------------------------------------------------------
// Print glm::vec

inline void log_vec4(glm::vec4 v, const char* const info = "") {
	std::cout << info << ": " << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
}

inline void log_vec3(glm::vec3 v, const char* const info = "") {
	std::cout << info << ": " << v.x << " " << v.y << " " << v.z << std::endl;
}

// -------------------------------------------------------------------------
// Check for OpenGL Errors
// Returns 1 if an OpenGL error occurred, 0 otherwise.


#define CheckOpenGLError() GLCheckError(__FILE__, __LINE__)

inline int GLCheckError(char *file, int line)
{
    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        printf("\nOpenGL ERROR => \n\tFILE: %s \n\tLINE: %d\n\tERROR: %s\n", file, line, gluErrorString(glErr));
		std::cout << "-----------------------------------------------------\n" << std::endl;
        retCode = 1;
    }
    return retCode;
}