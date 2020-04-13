#pragma once

#include <GL/glew.h>
#include "SDL.h"

#include <iostream>
#include <string>

namespace ogl {

	// Print OpenGL info
	void print_opengl_info();


	// Manages a vertex + fragment shader program
	class ShaderProgram
	{
	public:
		ShaderProgram() : program(0) {}
		~ShaderProgram();

		GLint get_attrib(const std::string& name);
		GLint get_uniform(const std::string& name);

		void useProgram();
		void loadShaders(const std::string& vs_filename, const std::string& fs_filename);

	private:
		GLuint program;

		GLuint create_shader(const std::string& filename, GLenum type);
		std::string read_file(const std::string& filename);
	};
}
