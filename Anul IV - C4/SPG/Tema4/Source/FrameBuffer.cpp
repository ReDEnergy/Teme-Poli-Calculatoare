#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() {
	FBO = 0;
}

FrameBuffer::~FrameBuffer() {

}

void FrameBuffer::clean() {
	if (FBO)
		glDeleteFramebuffers(1, &FBO);
	SAFE_FREE_ARRAY(textures);
	SAFE_FREE_ARRAY(DrawBuffers)
}

void FrameBuffer::generate(int width, int height, int nr_textures) {
	
	// clean previous FBO
	clean();

	cout << "FBO: " << width << " * " << height << " textures attached: " << nr_textures << endl;

	this->width = width;
	this->height = height;
	this->nr_textures = nr_textures;
	DrawBuffers = new GLenum[nr_textures];

	// Add attachments to drawing buffer
	for (int i=0; i<nr_textures; i++)
		DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;

	// Create FrameBufferObject
	glGenFramebuffers (1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create attached textures
	textures = new Texture[nr_textures];
	for (int i=0; i<nr_textures; i++)
		textures[i].createFrameBufferTexture(width, height, i);

	// Create depth texture
	depth_texture.createDepthBufferTexture(width, height);
	
	glDrawBuffers(nr_textures, DrawBuffers);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "FRAMEBUFFER NOT COMPLETE" << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::bindTexture(int textureID, GLenum TextureUnit) {
	textures[textureID].bind(TextureUnit);
}

void FrameBuffer::bindDepthTexture(Shader *shader, int texture_index) {
	glUniform1i(shader->loc_textures[texture_index], texture_index);
	depth_texture.bind(GL_TEXTURE0 + texture_index);
}

void FrameBuffer::bindAllTextures(Shader *shader) {
	for (unsigned int i=0; i<nr_textures; i++) {
		glUniform1i(shader->loc_textures[i], i);
		textures[i].bind(GL_TEXTURE0 + i);
	}
}
