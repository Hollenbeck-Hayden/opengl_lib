#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>
#include <list>

namespace ogl
{
	char* file_read(const char* filename);
	void print_log(GLuint object);

	class OGLError
	{
	public:
		OGLError(const std::string& msg);

		const std::string& getMessage() const;

	private:
		std::string message;
	};



	class Window
	{
	public:
		Window(const std::string& title, unsigned int size_x, unsigned int size_y);

		void swapWindow();
	
	private:
		SDL_Window* window;
	};



	class Program
	{
	public:
		Program();
		~Program();
		
		void createShader(const std::string& filename, GLenum type);
		void createProgram();

		void useProgram();

		GLint bindAttribute(const std::string& attribute_name);
		GLint bindUniform(const std::string& uniform_name);

	private:
		GLuint program;
		std::list<GLuint> shaders;
	};
};
