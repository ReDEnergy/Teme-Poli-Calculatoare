#include "Texture.h"

GLint pixel_format[5] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA};

Texture::Texture() {

}

Texture::~Texture() {
}

bool Texture::load2D(const char* file_name)
{
	GLenum textarget = GL_TEXTURE_2D;

	int width, height, bpp;
	unsigned char *data = stbi_load(file_name, &width, &height, &bpp, 0);

	if (data == NULL) {
		cout << "Error loading texture: " << file_name << endl;
		cout << width << " * " << height << " bpp: " << bpp << endl;
		return false;
	}

	cout << "Loaded " << file_name << endl;
	cout << "\t" << width << " * " << height << " bpp: " << bpp << endl;

	glGenTextures(1, &textureID);
	glBindTexture(textarget, textureID);

	setParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	glTexImage2D(textarget, 0, GL_RGBA, width, height, 0, pixel_format[bpp], GL_UNSIGNED_BYTE, (void*)data);
	glGenerateMipmap(textarget);

	glBindTexture(textarget, 0);

	stbi_image_free(data);
	return true;
}

void Texture::create2DTexture(const unsigned char* img, int width, int height, int bpp) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	setParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixel_format[bpp], GL_UNSIGNED_BYTE, (void*)img);
	CheckOpenGLError();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::createFrameBufferTexture(int width, int height, int target_id)
{
	GLenum textarget = GL_TEXTURE_2D;

	glGenTextures(1, &textureID);
	glBindTexture(textarget, textureID);

	setParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	glTexImage2D(textarget, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + target_id, textarget, textureID, 0);
	CheckOpenGLError();

	glBindTexture(textarget, 0);
}

void Texture::createDepthBufferTexture(int width, int height)
{
	GLenum textarget = GL_TEXTURE_2D;

	glGenTextures(1, &textureID);
	glBindTexture(textarget, textureID);

	setParameters(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
	CheckOpenGLError();

	glBindTexture(textarget, 0);
}


void Texture::bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::setParameters(GLenum mag_filter, GLenum min_filter, GLenum wrapping_mode)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping_mode);
}