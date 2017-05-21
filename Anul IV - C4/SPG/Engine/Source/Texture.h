#pragma once
#include "stb_image.hpp"
#include "Globals.h"

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	bool load2D(const char* file_name, GLenum wrapping_mode = GL_CLAMP_TO_EDGE);
	void create2DTexture(const unsigned char* img, int width, int height, int bpp);
	void create2DTextureFloat(const float* data, int width, int height, int bpp);
	void createFrameBufferTexture(int width, int height, int target_id);
	void createDepthBufferTexture(int width, int height);
	void setParameters(GLenum mag_filter, GLenum min_filter, GLenum wrapping_mode);
	void bind(GLenum TextureUnit);

private:
	GLuint textureID;
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