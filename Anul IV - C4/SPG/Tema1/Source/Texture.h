#pragma once
#include <Magick++.h>
#include "Globals.h"

class Texture
{
public:
	Texture();
	~Texture();

	bool load2D(const std::string& FileName);

	void bind(GLenum TextureUnit);

private:
	GLenum texture_target;
	GLuint texture_obj;
	Magick::Image* TextureImage;
};

enum MIN_FILTER {
	MIN_NEAREST = GL_NEAREST,
	MIN_LINEAR = GL_LINEAR,
	MIN_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	MIN_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	MIN_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	MIN_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum MAG_FILTER {
	MAG_NEAREST = GL_NEAREST,
	MAG_LINEAR = GL_LINEAR
};

enum WRAPPING_MODE {

	WM_REPEAT = GL_REPEAT,
	WM_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	WM_MIRRORED_REPEAT = GL_MIRRORED_REPEAT
};

enum TEXTURE_TYPE {
	NORMAL_TEXTURE,
	CUBE_TEXTURE
};