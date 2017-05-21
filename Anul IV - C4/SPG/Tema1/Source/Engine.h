#pragma once
#pragma comment(lib, "glew32.lib") 

#include <cstdlib>
#include <cstdio>
#include <string>
#include <Windows.h>

#include "Globals.h"
#include "Game.h"

using namespace std;

/*
 *	Grafic Engine
 */
namespace Engine {
	void init();
	void setGameObject(Game *G);
	void run();
};

/**
 * Information about Window
 */
class WindowInfo {

public:
	WindowInfo() {
		name = "Window Name";
		width = 250;
		height = 480;
		start_position_x = 240;
		start_position_y = 400;
		reshapable = true;
	}

	WindowInfo(string name, int width, int height, int start_position_x, int start_position_y, bool is_reshapable) {
		this->name = name;								
		this->width = width;							
		this->height = height;	
		this->start_position_x = start_position_x;
		this->start_position_y = start_position_y;	
		this->reshapable = is_reshapable;
	}

public:
	string name;			// window name
	int width;				// window widht
	int height;				// window height
	int start_position_x;	// window startX position
	int start_position_y;	// window startY position
	bool reshapable;		// window can be resized
};


/**
 * Information about FrameBuffer
 */
class FramebufferInfo {

public:
	FramebufferInfo() {
		flags=GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;	
		msaa = false;
	}

	FramebufferInfo(bool color, bool depth, bool stencil, bool msaa) {
		flags = GLUT_DOUBLE;
		if(color) flags |= GLUT_RGBA | GLUT_ALPHA;
		if(depth) flags |= GLUT_DEPTH;
		if(stencil) flags |= GLUT_STENCIL;
		if(msaa) flags |= GLUT_MULTISAMPLE;
		this->msaa = msaa;
	}

public:
	unsigned int flags;		// GL_RGBA | GL_DEPTH | GL_RGB | GL_STENCIL | ..
	bool msaa;				// MSAA 4x
};


/**
 * OpenGL context information
 */
class ContextInfo {

public:
	ContextInfo() {
		major_version = 3;	// OpenGL version
		minor_version = 3;		
		core = true;		// use just core function of the selected context
	}

	ContextInfo(int major_version, int minor_version, bool core){
		this->major_version = major_version;		
		this->minor_version = minor_version;	
		this->core=core; 
	}

public:
	int major_version;
	int minor_version;
	bool core;
};