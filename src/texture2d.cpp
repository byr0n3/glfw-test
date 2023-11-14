#include "include/texture2d.h"
#include "GL/glew.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

byrone::Texture2D::Texture2D() : id(0),
								 width(0), height(0),
								 internalFormat(GL_RGB), imageFormat(GL_RGB),
								 wrapS(GL_REPEAT), wrapT(GL_REPEAT),
								 filterMin(GL_LINEAR_MIPMAP_LINEAR), filterMax(GL_LINEAR),
								 mipmap(true) {
	glGenTextures(1, &this->id);
}

void byrone::Texture2D::Generate(GLsizei _width, GLsizei _height, unsigned char *data) {
	this->width = _width;
	this->height = _height;

	// create Texture
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(
			GL_TEXTURE_2D,
			0,
			this->internalFormat,
			this->width,
			this->height,
			0,
			this->imageFormat,
			GL_UNSIGNED_BYTE,
			data
	);

	// set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);

	if (this->mipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void byrone::Texture2D::GenerateFromMipmap(GLsizei _width,
										   GLsizei _height,
										   unsigned int fourCC,
										   unsigned int mipMapCount,
										   unsigned char *buffer) {
	this->width = _width;
	this->height = _height;

	unsigned int format;

	switch (fourCC) {
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;

		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;

		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;

		default:
			return;
	}

	glBindTexture(GL_TEXTURE_2D, this->id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// load the mipmap data per level
	for (GLint level = 0; level < mipMapCount && (_width || _height); ++level) {
		// not gonna lie, no clue how this calculation works but sure
		unsigned int size = ((_width + 3) / 4) * ((_height + 3) / 4) * blockSize;
		unsigned char *data = buffer + offset;

		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, _width, _height, 0, size, data);

		offset += size;
		_width /= 2;
		_height /= 2;

		if (_width < 1) {
			_width = 1;
		}

		if (_height < 1) {
			_height = 1;
		}
	}

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void byrone::Texture2D::Bind() const {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->id);
}
