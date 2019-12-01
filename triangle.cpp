#include <cstdlib>
#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "opengl_library.h"

GLint attribute_coord2d, attribute_v_color;
GLuint vbo_cube_vertices, vbo_cube_colors, ibo_cube_elements;

ogl::Program program;

bool init_resources(void)
{
	GLfloat cube_vertices[] = {
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0
	};
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	GLfloat cube_colors[] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0
	};
	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

	GLushort cube_elements[] = {
		0,1,2,
		2,3,0,
		1,5,6,
		6,2,1,
		7,6,5,
		5,4,7,
		4,0,3,
		3,7,4,
		4,5,1,
		1,0,4,
		3,2,6,
		6,7,3,
	};
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	try
	{
		program.createShader("triangle.v.glsl", GL_VERTEX_SHADER);
		program.createShader("triangle.f.glsl", GL_FRAGMENT_SHADER);
		program.createProgram();

		attribute_coord2d = program.bindAttribute("coord2d");
		attribute_v_color = program.bindAttribute("v_color");
	}
	catch(ogl::OGLError& err)
	{
		cerr << "Initialization Error: " << err.getMessage() << std::endl;
		return false;
	}

	return true;
}

void render(ogl::Window& window)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program.useProgram();

	glEnableVertexAttribArray(attribute_coord2d);
	glEnableVertexAttribArray(attribute_v_color);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glVertexAttribPointer(attribute_v_color, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_v_color);

	window.swapWindow();
}

void free_resources()
{
	glDeleteBuffers(1, &vbo_cube_vertices);
	glDeleteBuffers(1, &vbo_cube_colors);
}

void logic()
{
}

void mainLoop(ogl::Window& window)
{
	while(true)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				return;
			}
		}
		logic();
		render(window);
	}
}

ogl::Window createWindow()
{
	try
	{
		return ogl::Window("My second triangle", 640, 480);
	}
	catch(ogl::OGLError& error)
	{
		cerr << "Window Creation: " << error.getMessage() << std::endl;
		exit(1);
	}
}

int main(int argc, char** argv)
{
	ogl::Window window = createWindow();

	if (!init_resources())
		return EXIT_FAILURE;
	
	mainLoop(window);

	free_resources();
	return EXIT_FAILURE;
}
