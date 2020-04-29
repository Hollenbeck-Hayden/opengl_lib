#include <cstdlib>

#include "SDL.h"

#include "opengl/opengl_library.h"
//#include "opengl/test_renderer.h"
#include "opengl/line_renderer.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	int exit_status = EXIT_SUCCESS;
	
	try
	{
		ogl::LineProgram prog;

		ogl::print_opengl_info();

		while (prog.pollEvents())
		{
			prog.render();
		}

	}
	catch(ogl::OglException& e)
	{
		std::cerr << e.what() << std::endl;
		exit_status = EXIT_FAILURE;
	}

	SDL_Quit();
	return exit_status;
}
