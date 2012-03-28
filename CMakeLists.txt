CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Mechforce C)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules )

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
find_package(FreeImage REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

if( FREEIMAGE_INCLUDE_PATH AND FREEIMAGE_LIBRARY )
     message( STATUS "FreeImage found" )
else( FREEIMAGE_INCLUDE_PATH AND FREEIMAGE_LIBRARY )
     message( FATAL_ERROR "ERROR: FreeImage not found" )
endif( FREEIMAGE_INCLUDE_PATH AND FREEIMAGE_LIBRARY )

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

file(GLOB NetworkSourceFiles ${CMAKE_SOURCE_DIR}/src/Network/*.c)
file(GLOB NetworkHeaderFiles ${CMAKE_SOURCE_DIR}/src/Network/*.h)

file(GLOB SoundSourceFiles ${CMAKE_SOURCE_DIR}/src/Sound/*.c)
file(GLOB SoundHeaderFiles ${CMAKE_SOURCE_DIR}/src/Sound/*.h)

file(GLOB UtilsSourceFiles ${CMAKE_SOURCE_DIR}/src/Utils/*.c)
file(GLOB UtilsHeaderFiles ${CMAKE_SOURCE_DIR}/src/Utils/*.h)

file(GLOB CoreSourceFiles ${CMAKE_SOURCE_DIR}/src/Core/*.c)
file(GLOB CoreHeaderFiles ${CMAKE_SOURCE_DIR}/src/Core/*.h)

file(GLOB PrefsSourceFiles ${CMAKE_SOURCE_DIR}/src/Prefs/*.c)
file(GLOB PrefsHeaderFiles ${CMAKE_SOURCE_DIR}/src/Prefs/*.h)

set(SourceFiles ${SourceFiles} ${NetworkSourceFiles} ${SoundSourceFiles} ${UtilsSourceFiles} ${CoreSourceFiles} ${PrefsSourceFiles})
set(HeaderFiles ${HeaderFiles} ${NetworkHeaderFiles} ${SoundSourceFiles} ${UtilsSourceFiles} ${CoreSourceFiles} ${PrefsSourceFiles}) 

ADD_EXECUTABLE(Mechforce ${SourceFiles} ${HeaderFiles})

set_target_properties(Mechforce PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/System)

if(WIN32)
        TARGET_LINK_LIBRARIES(Mechforce mingw32 ftgl freetype freeimage SDLmain SDL SDL_mixer SDL_image opengl32 glu32 stdc++)
elseif (UNIX)
        TARGET_LINK_LIBRARIES(Mechforce GL GLU SDL SDL_mixer ftgl freetype freeimage)
endif()
