#include "opengl/test_renderer.h"

namespace ogl
{
	TestRenderer::TestRenderer()
		: Renderer("../data/shaders/square.v.glsl", "../data/shaders/square.f.glsl")
	{
		uniform_box_color = program.get_uniform("box_color");
		camera_offset = 0;

		// Define sprite vertices
		box_center[0] = 2;
		box_center[1] = -3;
		box_center[2] = 5;

		box_size = 0.5;

		box_axis1 = mvl::Vector<GLfloat,3>{(GLfloat) 0.0, (GLfloat) 1.0,(GLfloat) 1.0};
		box_axis2 = mvl::Vector<GLfloat,3>{(GLfloat) 0.0, (GLfloat)-1.0,(GLfloat) 1.0};

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
	}

	void TestRenderer::draw()
	{
		glUniform3f(uniform_box_color, 255, 255, 255);
		white_box.draw(shader_vertex_position);

		glUniform3f(uniform_box_color, 255, 0, 0);
		red_box.draw(shader_vertex_position);

		glUniform3f(uniform_box_color, 0, 255, 0);
		green_box.draw(shader_vertex_position);

		glUniform3f(uniform_box_color, 0, 0, 255);
		blue_box.draw(shader_vertex_position);
	}

	void TestRenderer::calculateVP(Window& window)
	{
		camera_offset += 0.01;
		auto normal_vector = mvl::cross(box_axis1, box_axis2);
		auto pos = cos(camera_offset) * normal_vector + sin(camera_offset) * box_axis1;
		view_matrix = aff::lookAt(box_center + ((GLfloat) 2) * pos, box_center + box_axis1, box_axis2);
		proj_matrix = aff::perspective<GLfloat>(90.0 * M_PI / 180.0, ((GLfloat) window.getHeight()) / ((GLfloat) window.getWidth()), 1.0, 10.0);
	}

	TestProgram::TestProgram()
		: window("Test Program", 640, 480)
	{
	}

	TestProgram::~TestProgram()
	{
	}

	void TestProgram::render()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		test_renderer.render(window);
		hud_renderer.render(window);

		window.swapBuffers();
	}

	bool TestProgram::pollEvents()
	{
		SDL_Event ev;

		while (SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					return false;

				case SDL_WINDOWEVENT:
					break;

				case SDL_MOUSEBUTTONDOWN:
					std::cout << "Mouse button down" << std::endl;
					std::cout << "Button that changed: ";
					switch (ev.button.button)
					{
						case SDL_BUTTON_LEFT:
							std::cout << "left";
							break;

						case SDL_BUTTON_RIGHT:
							std::cout << "right";
							break;

						case SDL_BUTTON_MIDDLE:
							std::cout << "middle";
							break;
					};
					std::cout << std::endl;
					std::cout << "\tx: " << ev.button.x << ", y: " << ev.button.y << std::endl;

					break;

				case SDL_MOUSEBUTTONUP:
					std::cout << "Mouse button up" << std::endl;
					break;

			};
		}

		return true;
	}
};
