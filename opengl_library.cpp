#include "opengl_library.h"
#include <iostream>

namespace ogl
{
	char* file_read(const char* filename)
	{
		SDL_RWops* rw = SDL_RWFromFile(filename, "rb");
		if (rw == NULL) return NULL;

		Sint64 res_size = SDL_RWsize(rw);
		char* res = (char*) malloc(res_size + 1);

		Sint64 nb_read_total = 0, nb_read = 1;
		char* buf = res;
		while (nb_read_total < res_size && nb_read != 0)
		{
			nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
			nb_read_total += nb_read;
			buf += nb_read;
		}

		SDL_RWclose(rw);
		if (nb_read_total != res_size)
		{
			free(res);
			return NULL;
		}

		res[nb_read_total] = '\0';
		return res;
	}

	void print_log(GLuint object)
	{
		GLint log_length = 0;
		if (glIsShader(object)) glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
		if (glIsProgram(object)) glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
		else
		{
			std::cerr << "printLog: Not a shader or a program" << std::endl;
			return;
		}

		char* log = (char*) malloc(log_length);

		if (glIsShader(object)) glGetShaderInfoLog(object, log_length, NULL, log);
		else if (glIsProgram(object)) glGetProgramInfoLog(object, log_length, NULL, log);

		std::cerr << log;
		free(log);
	}

	Program::Program()
	{}

	Program::~Program()
	{
		glDeleteProgram(program);
	}

	void Program::createShader(const std::string& filename, GLenum type)
	{
		const GLchar* source = file_read(filename.c_str());
		if (source == NULL)
			throw OGLError("Opening " + filename + ": " + std::string(SDL_GetError()));

		GLuint res = glCreateShader(type);
		const GLchar* sources[] = {"#version 120\n", source};
		glShaderSource(res, 2, sources, NULL);
		free((void*) source);

		glCompileShader(res);
		GLint compile_ok = GL_FALSE;
		glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
		if (compile_ok == GL_FALSE) {
			print_log(res);
			glDeleteShader(res);
			throw OGLError("Failed to compile shader sourced from " + filename);
		}

		shaders.push_back(res);
	}

	void Program::createProgram()
	{
		GLint link_ok = GL_FALSE;

		program = glCreateProgram();
		for (const auto& shader : shaders)
			glAttachShader(program, shader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
		if (!link_ok) {
			ogl::print_log(program);
			throw OGLError("glLinkProgram");
		}
	}

	void Program::useProgram()
	{
		glUseProgram(program);
	}

	GLint Program::bindAttribute(const std::string& attribute_name)
	{
		GLint attribute = glGetAttribLocation(program, attribute_name.c_str());
		if (attribute == -1)
			throw OGLError("Could not bind attribute " + attribute_name);
		return attribute;
	}

	GLint Program::bindUniform(const std::string& uniform_name)
	{
		GLint uniform = glGetUniformLocation(program, uniform_name.c_str());
		if (uniform == -1)
			throw OGLError("Could not bind uniform " + uniform_name);
		return uniform;
	}

	OGLError::OGLError(const std::string& msg)
	{
		message = msg;
	}

	const std::string& OGLError::getMessage() const
	{
		return message;
	}

	Window::Window(const std::string& title, unsigned int size_x, unsigned int size_y)
	{
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size_x, size_y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (window == NULL)
			throw OGLError("Could create window: " + std::string(SDL_GetError()));

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);

		if (SDL_GL_CreateContext(window) == NULL)
			throw OGLError("SDL_GL_CreateContext: " + std::string(SDL_GetError()));

		GLenum glew_status = glewInit();
		if (glew_status != GLEW_OK)
			throw OGLError("glewInit: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(glew_status))));
	}

	void Window::swapWindow()
	{
		SDL_GL_SwapWindow(window);
	}
};
