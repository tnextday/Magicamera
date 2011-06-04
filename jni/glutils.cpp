
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glutils.h"

#ifdef _WIN32
void OutputDebugText(const char* sz, ...){
	char szData[512] = {0};
	va_list args;
	va_start(args, sz);
	_vsnprintf(szData, sizeof(szData)-1, sz, args);
	va_end(args);
	OutputDebugString(szData);
}
#endif

void printGLString(const char *name, GLenum s) {
	const char *v = (const char *) glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}


const char* glErrorString(GLenum errcode)
{

	if (errcode == GL_NO_ERROR)
		return "GL_NO_ERROR";
	else if (errcode == GL_INVALID_ENUM)
		return"GL_INVALID_ENUM";
	else if (errcode == GL_INVALID_VALUE)
		return "GL_INVALID_VALUE";
	else if (errcode == GL_INVALID_OPERATION)
		return "GL_INVALID_OPERATION";
	else if (errcode == GL_OUT_OF_MEMORY)
		return "GL_OUT_OF_MEMORY";
	else
		return "Unknow Error Code";
}



void checkGlError(const char* op) {
	int i = 0;
	for (GLint error = glGetError(); error; error
		= glGetError()) {
			LOGI("after %s glError (0x%x[%s]) No. %d\n", op, error, glErrorString(error), i);
			i++;
	}
}

bool checkIfSupportsExtension(const char *extension){
	const char *extensions = (const char *) glGetString(GL_EXTENSIONS);
	return strstr(extensions, extension) != NULL;
}



GLuint loadShader(GLenum shaderType, const char* pSource) {
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n",
						shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}