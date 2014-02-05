CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Mechforce CXX)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules )

if(WIN32)
	SET(QT_INCLUDE_DIR "C:\\Qt\\Qt5.0.2\\5.0.2\\mingw47_32\\include")
	INCLUDE_DIRECTORIES(SYSTEM ${QT_INCLUDE_DIR})
endif()

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

if( NOT SDLMIXER_FOUND )
     message( FATAL_ERROR "ERROR: SDL_mixer not found" )
else()
    message( STATUS "SDL_mixer found" )
endif( NOT SDLMIXER_FOUND )

ADD_DEFINITIONS(-Wall -g)

file(GLOB SourceFiles ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HeaderFiles ${CMAKE_SOURCE_DIR}/src/*.h)

file(GLOB NetworkSourceFiles ${CMAKE_SOURCE_DIR}/src/Network/*.cpp)
file(GLOB NetworkHeaderFiles ${CMAKE_SOURCE_DIR}/src/Network/*.h)

file(GLOB SoundSourceFiles ${CMAKE_SOURCE_DIR}/src/Sound/*.cpp)
file(GLOB SoundHeaderFiles ${CMAKE_SOURCE_DIR}/src/Sound/*.h)

file(GLOB UtilsSourceFiles ${CMAKE_SOURCE_DIR}/src/Utils/*.cpp)
file(GLOB UtilsHeaderFiles ${CMAKE_SOURCE_DIR}/src/Utils/*.h)

file(GLOB CoreSourceFiles ${CMAKE_SOURCE_DIR}/src/Core/*.cpp)
file(GLOB CoreHeaderFiles ${CMAKE_SOURCE_DIR}/src/Core/*.h)

file(GLOB PrefsSourceFiles ${CMAKE_SOURCE_DIR}/src/Prefs/*.cpp)
file(GLOB PrefsHeaderFiles ${CMAKE_SOURCE_DIR}/src/Prefs/*.h)

set(SourceFiles ${SourceFiles} ${NetworkSourceFiles} ${SoundSourceFiles} ${UtilsSourceFiles} ${CoreSourceFiles} ${PrefsSourceFiles})
set(HeaderFiles ${HeaderFiles} ${NetworkHeaderFiles} ${SoundSourceFiles} ${UtilsSourceFiles} ${CoreSourceFiles} ${PrefsSourceFiles}) 

ADD_EXECUTABLE(Mechforce ${SourceFiles} ${HeaderFiles})

set_target_properties(Mechforce PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/System)

if(WIN32)
        TARGET_LINK_LIBRARIES(Mechforce mingw32 opengl32 glu32 ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${FTGL_LIBRARY} ${FREETYPE_LIBRARY} ${FREEIMAGE_LIBRARY} stdc++)
elseif (UNIX)
        TARGET_LINK_LIBRARIES(Mechforce GL GLU SDL SDL_mixer ftgl freetype freeimage)
endif()
