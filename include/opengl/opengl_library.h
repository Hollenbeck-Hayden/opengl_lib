#pragma once

#include <GL/glew.h>
#include "SDL.h"
#include "mvl/mvl.h"
#include "mvl/affine.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

namespace ogl {

	// Print OpenGL info
	void print_opengl_info();

	// Initialize GLEW
	void initGlew();

	// OG Library Exception
	class OglException : public std::runtime_error
	{
	public:
		OglException(const std::string& what_arg) : std::runtime_error(what_arg) {}
	};

	// SDL Window
	// Manages a single instances of an SDL Window
	// Expects SDL to be already initialized
	class Window
	{
	public:
		Window(const std::string& title, int width, int height);
		~Window();

		int getWidth();
		int getHeight();

		void swapBuffers();

	private:
		SDL_Window* window;
	};

	// Manages a vertex + fragment shader program
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vs_filename, const std::string& fs_filename);
		~ShaderProgram();

		GLint get_attrib(const std::string& name);
		GLint get_uniform(const std::string& name);

		void useProgram();

	private:
		GLuint program;

		GLuint create_shader(const std::string& filename, GLenum type);
		std::string read_file(const std::string& filename);
		void loadShaders(const std::string& vs_filename, const std::string& fs_filename);
	};

	// Manages a shader program and the objects it needs to
	// render
	class Renderer
	{
	public:
		Renderer(const std::string& vs_filename, const std::string& fs_filename);

		virtual void render(Window& window);
		virtual void calculateVP(Window& window) {}
		virtual void draw() {}

	protected:
		ShaderProgram program;

		//GLint shader_vertex_position;
		GLint shader_uniform_vp_matrix;

		aff::HomMatrix<GLfloat,3> proj_matrix;
		aff::HomMatrix<GLfloat,3> view_matrix;
	};
}
