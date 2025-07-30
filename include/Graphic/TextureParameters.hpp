#ifndef TEXTUREPARAMETERS_HPP
#define TEXTUREPARAMETERS_HPP

#ifdef TARGET_OS_OSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

struct TextureParameters {
	GLint wrapS = GL_REPEAT;
	GLint wrapT = GL_REPEAT;

	GLint minFilter = GL_LINEAR;
	GLint magFilter = GL_LINEAR;

	void apply() {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	}
};

#endif
