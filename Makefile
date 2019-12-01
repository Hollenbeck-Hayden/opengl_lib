CPPFLAGS=$(shell sdl2-config --cflags) $(EXTRA_CPPFLAGS)
LDLIBS=$(shell sdl2-config --libs) -lGLEW $(EXTRA_LDLIBS)
EXTRA_LDLIBS?=-lGL
INCLUDES=includes

all: triangle

triangle:
	g++ $(CPPFLAGS) triangle.cpp opengl_library.cpp $(LDLIBS) -I $(INCLUDES) -o triangle 

clean:
	rm -f *.o triangle

again: clean all

.PHONY: all clean again
