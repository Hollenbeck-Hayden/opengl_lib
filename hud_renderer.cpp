#include "mvl.h"
#include "affine.h"
#include "hud_renderer.h"

namespace ogl
{
	HudRenderer::HudRenderer()
		: Renderer("hud.v.glsl", "hud.f.glsl")
	{
		uniform_hud_color = program.get_uniform("hud_color");

		mvl::Vector<GLfloat,2> ul{(GLfloat)   0, (GLfloat)  0};
		mvl::Vector<GLfloat,2> ur{(GLfloat) 100, (GLfloat)  0};
		mvl::Vector<GLfloat,2> ll{(GLfloat)   0, (GLfloat) 50};
		mvl::Vector<GLfloat,2> lr{(GLfloat) 100, (GLfloat) 50};

		purple_box.addVertices(std::vector<mvl::Vector<GLfloat,2>>{ul, ur, ll, lr});
	}

	void HudRenderer::calculateVP(Window& window)
	{
		auto eye = (mvl::Vector<GLfloat,3>{(GLfloat) window.getWidth(),(GLfloat) window.getHeight(), (GLfloat) 0} / (GLfloat) 2.0) + aff::z_axis<GLfloat,3>();
		view_matrix = aff::lookTowards(eye, -aff::z_axis<GLfloat,3>(), aff::y_axis<GLfloat,3>());
		proj_matrix = aff::orthographic(mvl::Vector<GLfloat,3>{ ((GLfloat) window.getWidth())/2, -((GLfloat) window.getHeight())/2, (GLfloat) -1},
						mvl::Vector<GLfloat,3>{-((GLfloat) window.getWidth())/2,  ((GLfloat) window.getHeight())/2, (GLfloat)  1});
	}

	void HudRenderer::draw()
	{
		glUniform3f(uniform_hud_color, 255, 0, 255);
		purple_box.draw(shader_vertex_position);
	}
};
