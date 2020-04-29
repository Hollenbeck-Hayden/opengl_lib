#pragma once

#include <cmath>
#include <vector>
#include <memory>

#include "opengl/opengl_library.h"
#include "mvl/mvl.h"
#include "mvl/affine.h"
#include "opengl/sprite.h"
#include "opengl/hud_renderer.h"

namespace ogl
{
	class LineRenderer : public Renderer
	{
	public:
		LineRenderer();
			
		virtual void draw();
		virtual void calculateVP(Window& window);
	
	private:
		unsigned int num_lines = 20;
		GLfloat spacing = 5;

		std::vector<mvl::Vector<GLfloat,2>> end_A,end_B;
		std::vector<GLfloat> widths;

		GLuint attribute_endpoint_A, attribute_endpoint_B, attribute_uv, attribute_thickness, attribute_color, uniform_resolution;
	};


	class LineProgram
	{
	public:

		LineProgram();
		~LineProgram();

		void render();
		bool pollEvents();
		
	private:
		Window window;
		LineRenderer line_renderer;
	};
};
