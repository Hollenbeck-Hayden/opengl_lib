CPPFLAGS=$(shell sdl2-config --cflags) $(EXTRA_CPPFLAGS)
LDLIBS=$(shell sdl2-config --libs) -lGLEW $(EXTRA_LDLIBS)
EXTRA_LDLIBS?=-lGL
INCLUDES=../../mvl/mvl/src/

all: opengl

opengl:
	g++ $(CPPFLAGS) main.cpp opengl_library.cpp hud_renderer.cpp test_renderer.cpp $(LDLIBS) -I $(INCLUDES) -o opengl

clean:
	rm -f *.o opengl

again: clean all

.PHONY: all clean again
