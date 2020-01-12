#
#
# Try to find the FTGL libraries
# Once done this will define
#
# FTGL_FOUND          - system has ftgl
# FTGL_INCLUDE_DIR    - path to FTGL/FTGL.h
# FTGL_LIBRARY      - the library that must be included
#
#

IF (FTGL_LIBRARY AND FTGL_INCLUDE_DIR)
  SET(FTGL_FOUND "YES")
ELSE (FTGL_LIBRARY AND FTGL_INCLUDE_DIR)
  
FIND_PATH(FTGL_INCLUDE_DIR FTGL/ftgl.h PATHS 
"C:/CodeBlocks/MinGW/include"
/usr/local/include /usr/include 
"C:/Qt/Qt5.0.2/5.0.2/mingw47_32/include")
  
FIND_LIBRARY(FTGL_LIBRARY NAMES libftgl ftgl libftgl.a libftgl_static libftgl_static.a ftgl_static PATHS
"C:/CodeBlocks/MinGW/lib"
/usr/local/lib /usr/lib 
"C:/Qt/Qt5.0.2/5.0.2/mingw47_32/lib")
  
IF (FTGL_INCLUDE_DIR AND FTGL_LIBRARY)
    SET(FTGL_FOUND "YES")
  ELSE (FTGL_INCLUDE_DIR AND FTGL_LIBRARY)
    SET(FTGL_FOUND "NO")
  ENDIF (FTGL_INCLUDE_DIR AND FTGL_LIBRARY)
ENDIF (FTGL_LIBRARY AND FTGL_INCLUDE_DIR)

IF (FTGL_FOUND)
  MESSAGE(STATUS "Found FTGL libraries at ${FTGL_LIBRARY} and includes at ${FTGL_INCLUDE_DIR}")
ELSE (FTGL_FOUND)
  IF (FTGL_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find FTGL libraries")
  ENDIF (FTGL_FIND_REQUIRED)
ENDIF (FTGL_FOUND)