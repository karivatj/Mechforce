CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Mechforce C)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

ADD_DEFINITIONS(-Wall)

file(GLOB SourceFiles ./src/*.c)

set(SourceFiles ${SourceFiles})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/)

if(WIN32)

#Modify these paths so Cmake can find freetype and others in Windows. 

INCLUDE_DIRECTORIES("C:/Program Files/Qt/mingw/include")
INCLUDE_DIRECTORIES("C:/Program Files/Qt/mingw/include/freetype2")
endif()

ADD_EXECUTABLE(Mechforce ${SourceFiles})

set_target_properties(Mechforce PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/System)


if(WIN32)
	TARGET_LINK_LIBRARIES(Mechforce mingw32 freetype ftgl SDLmain SDL SDL_mixer SDL_image opengl32 glu32)
elseif (UNIX)
	TARGET_LINK_LIBRARIES(Mechforce GL GLU SDL SDL_mixer SDL_image ftgl freetype)
endif()
