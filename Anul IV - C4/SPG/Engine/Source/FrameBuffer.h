#pragma once

#include <vector>
#include "Globals.h"
#include "Shader.h"
#include "Texture.h"

using namespace std;

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	void clean();
	void generate(int width, int height, int nr_textures);

	void bind();
	void bindTexture(int textureID, GLenum TextureUnit);
	void bindAllTextures(Shader *shader);
	void bindDepthTexture(Shader *shader, int texture_index);
	void sendResolution(Shader *shader);

public:

private:
	GLuint FBO;
	GLenum *DrawBuffers;		// TEST IF IS NECESSARY TO DECLARE
	GLuint depthRenderbuffer;

	Texture *textures;
	Texture depth_texture;

	GLsizei width;
	GLsizei height;
	GLuint nr_textures;

};