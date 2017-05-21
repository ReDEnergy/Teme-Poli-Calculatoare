#include "Texture.h"

Texture::Texture() {

}

Texture::~Texture() {

}

bool Texture::load2D(const std::string& FileName)
{
	texture_target = GL_TEXTURE_2D;
	Magick::Blob blob;

	try {
		TextureImage = new Magick::Image(FileName);
		TextureImage->write(&blob, "RGBA");
	}
	catch (Magick::Error& Error) {
		std::cout << "Error loading texture '" << FileName << "': " << Error.what() << std::endl;
		return false;
	}

	glGenTextures(1, &texture_obj);
	glBindTexture(texture_target, texture_obj);

	glTexImage2D(texture_target, 0, GL_RGBA, TextureImage->columns(), TextureImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(texture_target);

	glBindTexture(texture_target, 0);
	return true;
}

void Texture::bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(texture_target, texture_obj);
}
