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
	class TestRenderer : public Renderer
	{
	public:
		TestRenderer();
			
		virtual void draw();
		virtual void calculateVP(Window& window);
	
	private:
		const unsigned int num_lines = 100;

		std::vector<Sprite<GLfloat,2>> lines;
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
