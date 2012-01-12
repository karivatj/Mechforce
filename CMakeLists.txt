CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Mechforce C)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules )

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

if( NOT SDL_FOUND )
     message( FATAL_ERROR "ERROR: SDL not found" )
else()
     message( STATUS "SDL found" )
endif( NOT SDL_FOUND )

if( NOT SDLIMAGE_FOUND )
     message( FATAL_ERROR "ERROR: SDL_image not found" )
else()
    message( STATUS "SDL_image found" )
endif( NOT SDLIMAGE_FOUND )

if( NOT SDLMIXER_FOUND )
     message( FATAL_ERROR "ERROR: SDL_mixer not found" )
else()
    message( STATUS "SDL_mixer found" )
endif( NOT SDLMIXER_FOUND )

ADD_DEFINITIONS(-Wall -g)

file(GLOB SourceFiles ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB HeaderFiles ${CMAKE_SOURCE_DIR}/src/*.h)

set(SourceFiles ${SourceFiles})
set(HeadereFiles ${HeaderFiles})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/)

ADD_EXECUTABLE(Mechforce ${SourceFiles} ${HeaderFiles})

set_target_properties(Mechforce PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/System)

if(WIN32)
        TARGET_LINK_LIBRARIES(Mechforce mingw32 ftgl freetype freeimage SDLmain SDL SDL_mixer SDL_image opengl32 glu32 stdc++)
elseif (UNIX)
        TARGET_LINK_LIBRARIES(Mechforce GL GLU SDL SDL_mixer SDL_image ftgl freetype freeimage)
endif()
