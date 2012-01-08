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
  FIND_PATH(FTGL_INCLUDE_DIR FTGL/ftgl.h PATHS /usr/local/include /usr/include  C:\\QtSDK\\mingw\\include)
  FIND_LIBRARY(FTGL_LIBRARY ftgl PATHS /usr/local/lib /usr/lib   C:\\QtSDK\\mingw\\lib)
  
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