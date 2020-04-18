#pragma once

#include <cmath>
#include <list>
#include <memory>

#include "opengl/opengl_library.h"
#include "mvl/mvl.h"
#include "aff/affine.h"
#include "opengl/sprite.h"
#include "opengl/hud_renderer.h"

namespace ogl
{
	class TestRenderer : public Renderer
	{
	public:
		TestRenderer();
			
		virtual void draw();
		virtual void calculateVP(Window& window);
	
	private:
		GLint uniform_box_color;
		Sprite<GLfloat,3> white_box, red_box, green_box, blue_box;

		GLfloat camera_offset;
		mvl::Vector<GLfloat,3> box_center, box_axis1, box_axis2;
		GLfloat box_size;
	};


	class TestProgram
	{
	public:

		TestProgram();
		~TestProgram();

		void render();
		bool pollEvents();
		
	private:
		Window window;
		TestRenderer test_renderer;
		HudRenderer hud_renderer;
	};
};
