#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>

#include <GL/glew.h>
#include "SDL.h"

#include "opengl_library.h"
#include "sprite.h"
#include "mvl.h"
#include "affine.h"

ogl::ShaderProgram program, hud_program;
GLint attribute_position, hud_position;
GLint uniform_box_color, uniform_hud_color;
//GLint uniform_view_matrix;
//GLint uniform_proj_matrix;
GLint uniform_vp_matrix;
GLint uniform_hud_vp_matrix;

GLuint screenWidth = 640;
GLuint screenHeight = 480;

GLfloat camera_offset = 0.0;

ogl::Sprite<GLfloat,3> white_box, red_box, green_box, blue_box, purple_box;
mvl::Vector<GLfloat,3> box_center, box_axis1, box_axis2;
GLfloat box_size;

template<typename T, size_t N>
void print_vector(const mvl::Vector<T,N>& v)
{
	std::cout << "[";
	for (size_t i = 0; i < N-1; i++)
		std::cout << v[i] << ", ";
	std::cout << v[N-1] << "]" << std::endl;
}

int init_resources()
{
	program.loadShaders("square.v.glsl", "square.f.glsl");

	attribute_position = program.get_attrib("position");

	if (attribute_position  == -1)
		return 0;
	
	uniform_box_color = program.get_uniform("box_color");
	uniform_vp_matrix = program.get_uniform("vp_matrix");

	if (uniform_box_color == -1)
		return 0;

	if (uniform_vp_matrix == -1)
		return 0;
	

	hud_program.loadShaders("hud.v.glsl", "hud.f.glsl");
	hud_position = hud_program.get_attrib("position");

	if (hud_position == -1)
		return 0;
	
	uniform_hud_color = hud_program.get_uniform("hud_color");
	uniform_hud_vp_matrix = hud_program.get_uniform("vp_matrix");

	if (uniform_hud_color == -1)
		return 0;
	
	if (uniform_hud_vp_matrix == -1)
		return 0;

	// Define sprite vertices
	box_center[0] = 2;
	box_center[1] = -3;
	box_center[2] = 5;

	box_size = 0.5;

	box_axis1 = mvl::Vector<GLfloat,3>{0.0, 1.0,1.0};
	box_axis2 = mvl::Vector<GLfloat,3>{0.0,-1.0,1.0};

	normalize(box_axis1);
	normalize(box_axis2);

	// Define box coordiantes
	auto box_r = box_center + box_size * box_axis1;
	auto box_l = box_center - box_size * box_axis1;
	auto box_t = box_center + box_size * box_axis2;
	auto box_b = box_center - box_size * box_axis2;
	auto box_rt = box_center + box_size * ( box_axis1 + box_axis2);
	auto box_lt = box_center + box_size * (-box_axis1 + box_axis2);
	auto box_rb = box_center + box_size * ( box_axis1 - box_axis2);
	auto box_lb = box_center + box_size * (-box_axis1 - box_axis2);

	white_box.addVertices(std::vector<mvl::Vector<GLfloat,3>>{box_center, box_r, box_t, box_rt});
	  red_box.addVertices(std::vector<mvl::Vector<GLfloat,3>>{box_center, box_t, box_l, box_lt});
	green_box.addVertices(std::vector<mvl::Vector<GLfloat,3>>{box_center, box_l, box_b, box_lb});
	 blue_box.addVertices(std::vector<mvl::Vector<GLfloat,3>>{box_center, box_b, box_r, box_rb});

	mvl::Vector<GLfloat,3> hud_box_upper_left  {  -1,   -1,0};
	mvl::Vector<GLfloat,3> hud_box_upper_right {-0.7,   -1,0};
	mvl::Vector<GLfloat,3> hud_box_lower_left  {  -1, -0.7,0};
	mvl::Vector<GLfloat,3> hud_box_lower_right {-0.7, -0.7,0};

	purple_box.addVertices(std::vector<mvl::Vector<GLfloat,3>>{hud_box_upper_left, hud_box_upper_right, hud_box_lower_left, hud_box_lower_right});

	return 1;
}

void render(SDL_Window* window)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// World program
	program.useProgram();
	glEnableVertexAttribArray(attribute_position);

	// Define MVP matrices

	auto normal_vector = mvl::cross(box_axis1, box_axis2);

	mvl::Vector<GLfloat,3> pos = cos(camera_offset) * normal_vector + sin(camera_offset) * box_axis1;
//	mvl::Vector<GLfloat,3> pos = normal_vector;

	mvl::Vector<GLfloat,3> rtf = { box_size,  box_size, 10};
	mvl::Vector<GLfloat,3> lbn = {-box_size, -box_size, -1};

	auto view_matrix = aff::lookAt(box_center + ((GLfloat) 2) * pos, box_center + box_axis1, box_axis2);
	//auto proj_matrix = aff::orthographic(rtf, lbn);
	auto proj_matrix = aff::perspective<GLfloat>(90.0 * M_PI / 180.0, ((GLfloat) screenHeight) / ((GLfloat) screenWidth), 1.0, 10.0);
	glUniformMatrix4fv(uniform_vp_matrix, 1, GL_FALSE, (proj_matrix * view_matrix).toArray());

	// Draw boxes
	glUniform3f(uniform_box_color, 255, 255, 255);
	white_box.draw(attribute_position);

	glUniform3f(uniform_box_color, 255, 0, 0);
	red_box.draw(attribute_position);

	glUniform3f(uniform_box_color, 0, 255, 0);
	green_box.draw(attribute_position);

	glUniform3f(uniform_box_color, 0, 0, 255);
	blue_box.draw(attribute_position);

	glDisableVertexAttribArray(attribute_position);


	// HUD Program
	hud_program.useProgram();
	glEnableVertexAttribArray(hud_position);
	
	view_matrix = aff::lookAt(mvl::Vector<GLfloat,3>{0,0,-1}, mvl::Vector<GLfloat,3>(), mvl::Vector<GLfloat,3>{0,1,0});
	proj_matrix = aff::orthographic(mvl::Vector<GLfloat,3>{-1,-1,-1}, mvl::Vector<GLfloat,3>{1,1,1});
	glUniformMatrix4fv(uniform_hud_vp_matrix, 1, GL_FALSE, (proj_matrix * view_matrix).toArray());

	glUniform3f(uniform_hud_color, 255, 0, 255);
	purple_box.draw(hud_position);

	glDisableVertexAttribArray(hud_position);



	// Swap buffers
	SDL_GL_SwapWindow(window);
}

void free_resources()
{
}


void mainLoop(SDL_Window* window)
{
	while(true)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT) return;

			if (ev.type == SDL_WINDOWEVENT)
			{
				if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					screenWidth = ev.window.data1;
					screenHeight = ev.window.data2;

					std::cout << "Screen update" << std::endl;
					std::cout << "Width:\t" << screenWidth << std::endl;
					std::cout << "Height:\t" << screenHeight << std::endl;
				}
			}
		}

		camera_offset += 0.01;

		render(window);
	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("anti-aliasing",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screenWidth, screenHeight,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		std::cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << std::endl;
		return EXIT_FAILURE;
	}

	ogl::print_opengl_info();

	if (!init_resources())
		return EXIT_FAILURE;


	mainLoop(window);

	free_resources();
	return EXIT_SUCCESS;
}
