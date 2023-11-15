#ifndef BYRONE_TEXTURE_2D
#define BYRONE_TEXTURE_2D

#include "GL/glew.h"

namespace byrone {
	class Texture2D {
	public:
		unsigned int id;
		GLsizei width, height;

		Texture2D();

		// format of texture object
		GLint internalFormat;
		// format of loaded image
		GLint imageFormat;

		// wrapping mode on S axis
		GLint wrapS;
		// wrapping mode on T axis
		GLint wrapT;

		// filtering mode if texture pixels < screen pixels
		GLint filterMin;
		// filtering mode if texture pixels > screen pixels
		GLint filterMax;

		// generates texture from image data
		void Generate(GLsizei width, GLsizei height, unsigned char *data);

		// binds the texture as the current active GL_TEXTURE_2D texture object
		void Bind() const;
	};
}

#endif
