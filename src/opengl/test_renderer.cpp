#include "opengl/test_renderer.h"

namespace ogl
{
	TestRenderer::TestRenderer()
		: Renderer("../data/shaders/point.v.glsl", "../data/shaders/point.f.glsl")
	{
		points.setPrimitiveType(GL_POINTS);

		std::vector<mvl::Vector<GLfloat,2>> centers;
		for (unsigned int i = 0; i < num_points; i++)
			centers.push_back(mvl::Vector<GLfloat,2>{2.0 * radius * (GLfloat) i, 0});

		points.addVertices(centers);
		

		//for (const auto& center : centers)
		//{
		//	auto ul = center + mvl::Vector<GLfloat,2>{-1,-1};
		//	auto ur = center + mvl::Vector<GLfloat,2>{-1, 1};
		//	auto ll = center + mvl::Vector<GLfloat,2>{ 1,-1};
		//	auto lr = center + mvl::Vector<GLfloat,2>{ 1, 1};
		//	boxes.push_back(Sprite<GLfloat,2>());
		//	boxes.back().addVertices(std::vector<mvl::Vector<GLfloat,2>>{ul,ur,ll,lr});
		//}
	}

	void TestRenderer::draw()
	{
		glUniform1f(uniform_radius, radius);
		points.draw(shader_vertex_position);

		//for (auto&& box : boxes)
		//	box.draw(shader_vertex_position);
	}

	void TestRenderer::calculateVP(Window& window)
	{
		view_matrix = aff::lookAt(aff::z_axis<GLfloat,3>(), mvl::Vector<GLfloat,3>(), aff::y_axis<GLfloat,3>());
//		proj_matrix = aff::orthographic(mvl::Vector<GLfloat,3>{2.0 * radius * (GLfloat)(num_points + 2), -2.0, 1.0},
//						mvl::Vector<GLfloat,3>{-1.0,2.0,-1.0});
		GLfloat size = 2 * radius * (GLfloat) num_points;
		proj_matrix = aff::orthographic(mvl::Vector<GLfloat,3>{size - radius,size/2,1}, mvl::Vector<GLfloat,3>{-radius,-size/2,-1});
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
		glClearColor(255, 255, 255, 1);
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
					switch(ev.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							std::cout << "Window " << ev.window.windowID << " resized to " << ev.window.data1 << "x" << ev.window.data2 << std::endl;
							glViewport(0,0,ev.window.data1,ev.window.data2);
					};
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
