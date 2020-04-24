cmake_minimum_required(VERSION 3.13)
project(opengl_lib VERSION 0.0.1 LANGUAGES CXX)

# Define program target
add_executable(opengl)
target_include_directories(opengl PUBLIC include)

# Add source directories
add_subdirectory(src)


# Add cmath library
target_link_libraries(opengl m)

# Add MVL library
#target_include_directories(opengl PUBLIC /home/antihydra/projects/mvl/mvl/include)
find_package(mvl CONFIG REQUIRED)
target_link_libraries(opengl mvl::mvl)


# Add OpenGL library
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
target_include_directories(opengl PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(opengl ${OPENGL_LIBRARIES})

# Add GLEW library
find_package(GLEW REQUIRED)
target_include_directories(opengl PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(opengl ${GLEW_LIBRARIES})

# Add SDL library
set(SDL2_INCLUDE_DIR /usr/include/SDL2)
set(SDL2_LIBRARY /usr/lib/libSDL2.so)
target_include_directories(opengl PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(opengl ${SDL2_LIBRARY})
