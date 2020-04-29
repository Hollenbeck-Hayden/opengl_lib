#include "opengl/line_renderer.h"

#include <iostream>

namespace ogl
{
	LineRenderer::LineRenderer()
		: Renderer("../data/shaders/line.v.glsl", "../data/shaders/line.f.glsl")
	{
		attribute_endpoint_A = program.get_attrib("pA");
		attribute_endpoint_B = program.get_attrib("pB");
		attribute_uv = program.get_attrib("uv");
		attribute_thickness = program.get_attrib("thickness");
		uniform_resolution = program.get_uniform("resolution");

		for (unsigned int i = 0; i < num_lines; i++)
		{
			GLfloat x = spacing * (GLfloat) i;
			end_A.push_back(mvl::Vector<GLfloat,2>{x,0});
			end_B.push_back(mvl::Vector<GLfloat,2>{x,1});
			widths.push_back(0.2 * (GLfloat)(i+1));
			std::cout << widths.back() << std::endl;
		}
	}

	void LineRenderer::draw()
	{
		std::vector<GLfloat> endpoints_A;
		std::vector<GLfloat> endpoints_B;
		std::vector<GLuint> uv;
		std::vector<GLfloat> thicknesses;

		for (unsigned int n = 0; n < num_lines; n++)
		{
			auto A = end_A[n];
			auto B = end_B[n];
			auto thickness = widths[n];

			// Attributes for triangle 1
			for (unsigned int i = 0; i < 3; i++)
			{
				endpoints_A.push_back(A[0]);
				endpoints_A.push_back(A[1]);
				endpoints_B.push_back(B[0]);
				endpoints_B.push_back(B[1]);
				thicknesses.push_back(thickness);
			}
			uv.push_back(0); uv.push_back(0);
			uv.push_back(0); uv.push_back(1);
			uv.push_back(1); uv.push_back(0);

			// Attributes for triangle 2
			for (unsigned int i = 0; i < 3; i++)
			{
				endpoints_A.push_back(A[0]);
				endpoints_A.push_back(A[1]);
				endpoints_B.push_back(B[0]);
				endpoints_B.push_back(B[1]);
				thicknesses.push_back(thickness);
			}
			uv.push_back(0); uv.push_back(0);
			uv.push_back(1); uv.push_back(0);
			uv.push_back(1); uv.push_back(1);
		}

		// Enable vertex attribute arrays
		glEnableVertexAttribArray(attribute_endpoint_A);
		glEnableVertexAttribArray(attribute_endpoint_B);
		glEnableVertexAttribArray(attribute_uv);
		glEnableVertexAttribArray(attribute_thickness);

		// Set attributes
		glVertexAttribPointer(attribute_endpoint_A, 2, GL_FLOAT, GL_FALSE, 0, endpoints_A.data());
		glVertexAttribPointer(attribute_endpoint_B, 2, GL_FLOAT, GL_FALSE, 0, endpoints_B.data());
		glVertexAttribPointer(attribute_uv,         2, GL_UNSIGNED_INT, GL_FALSE, 0,          uv.data());
		glVertexAttribPointer(attribute_thickness,  1, GL_FLOAT, GL_FALSE, 0, thicknesses.data());

		// Draw line segment
		glDrawArrays(GL_TRIANGLES, 0, 6 * num_lines);

		// Disable vertex attribute arrays
		glDisableVertexAttribArray(attribute_endpoint_A);
		glDisableVertexAttribArray(attribute_endpoint_B);
		glDisableVertexAttribArray(attribute_uv);
		glDisableVertexAttribArray(attribute_thickness);
	}

	void LineRenderer::calculateVP(Window& window)
	{
		view_matrix = aff::lookAt(aff::z_axis<GLfloat,3>(), mvl::Vector<GLfloat,3>(), aff::y_axis<GLfloat,3>());
		GLfloat size = spacing * num_lines / 2;
		//proj_matrix = aff::orthographic(mvl::Vector<GLfloat,3>{2*size,size,1}, mvl::Vector<GLfloat,3>{0,-size,-1});
		proj_matrix = aff::orthographic(mvl::Vector<GLfloat,3>{2*size - spacing/2,2,1}, mvl::Vector<GLfloat,3>{-spacing/2,-2,-1});
		glUniform2f(uniform_resolution, window.getWidth(), window.getHeight());
	}




	LineProgram::LineProgram()
		: window("Line Program", 640, 480)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	LineProgram::~LineProgram()
	{
	}

	void LineProgram::render()
	{
		glClearColor(255, 255, 255, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		line_renderer.render(window);

		window.swapBuffers();
	}

	bool LineProgram::pollEvents()
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
			};
		}

		return true;
	}
};
