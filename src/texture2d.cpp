#include <include/texture2d.h>
#include <src/debug.h>

byrone::Texture2D::Texture2D() : id(0),
								 width(0), height(0),
								 internalFormat(GL_RGB), imageFormat(GL_RGB),
								 wrapS(GL_REPEAT), wrapT(GL_REPEAT),
								 filterMin(GL_LINEAR), filterMax(GL_LINEAR) {
	GL_CHECK(glGenTextures(1, &this->id))
}

void byrone::Texture2D::Generate(GLsizei _width, GLsizei _height, unsigned char *data) {
	this->width = _width;
	this->height = _height;

	// create Texture
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->id))
	GL_CHECK(glTexImage2D(
			GL_TEXTURE_2D,
			0,
			this->internalFormat,
			this->width,
			this->height,
			0,
			this->imageFormat,
			GL_UNSIGNED_BYTE,
			data
	))

	// set Texture wrap and filter modes
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS))
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT))
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin))
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax))

	// unbind texture
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
}

void byrone::Texture2D::Bind() const {
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->id))
}

void byrone::Texture2D::Unbind() const {
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0))
}
