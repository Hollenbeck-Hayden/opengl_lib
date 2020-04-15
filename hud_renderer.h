#pragma once

#include "opengl_library.h"
#include "sprite.h"

namespace ogl
{
	
	class HudRenderer : public Renderer
	{
	public:
		HudRenderer();

		virtual void calculateVP(Window& window);
		virtual void draw();

	private:
		GLint uniform_hud_color;
		Sprite<GLfloat,2> purple_box;
	};
};
