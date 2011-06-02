#
# try to find glut library and include files
#
# GLUT_INCLUDE_DIR, where to find GL/glut.h, etc.
# GLUT_LIBRARIES, the libraries to link against to use GLUT.
# GLUT_FOUND, If false, do not try to use GLUT.

# also defined, but not for general use are
# GLUT_glut_LIBRARY, where to find the glut library.
# GLUT_Xmu_LIBRARY, where to find the Xmu library if available.
# GLUT_Xi_LIBRARY, where to find the Xi Library if available.


IF (WIN32)
  FIND_PATH( OPENGL_LIBRARY_DIR glut32.lib )
  IF(CYGWIN)

    FIND_PATH( GLUT_INCLUDE_DIR GL/glut.h
      /usr/include
      ${GLUT_INCLUDE_PATH}
    )

    FIND_LIBRARY( GLUT_glut_LIBRARY glut32
      ${OPENGL_LIBRARY_DIR}
      /usr/lib64
      /usr/lib
      /usr/lib/w32api
      /usr/local/lib
      /usr/X11R6/lib
    )


  ELSE(CYGWIN)

    FIND_PATH( GLUT_INCLUDE_DIR GL/glut.h
      ${GLUT_ROOT_PATH}/include
    )

    FIND_LIBRARY( GLUT_glut_LIBRARY glut
      ${GLUT_ROOT_PATH}/Release
      ${OPENGL_LIBRARY_DIR}
    )

  ENDIF(CYGWIN)

ELSE (WIN32)

  IF (APPLE)
# These values for Apple could probably do with improvement.
    FIND_PATH( GLUT_INCLUDE_DIR GL/glut.h
      ${OPENGL_LIBRARY_DIR}
    )
    SET(GLUT_glut_LIBRARY "-framework Glut" CACHE STRING "GLUT library for OSX")
  ELSE (APPLE)

    FIND_PATH( GLUT_INCLUDE_DIR GL/glut.h
      /usr/include
      /usr/include/GL
      /usr/local/include
      /usr/openwin/share/include
      /usr/openwin/include
      /usr/X11R6/include
      /usr/include/X11
      /opt/graphics/OpenGL/include
      /opt/graphics/OpenGL/contrib/libglut
      ${GLUT_INCLUDE_PATH}
    )

    FIND_LIBRARY( GLUT_glut_LIBRARY glut
      /usr/lib64
      /usr/lib
      /usr/local/lib
      /usr/openwin/lib
      /usr/X11R6/lib
    )

    FIND_LIBRARY( GLUT_Xi_LIBRARY Xi
      /usr/lib64
      /usr/lib
      /usr/local/lib
      /usr/openwin/lib
      /usr/X11R6/lib
    )

    FIND_LIBRARY( GLUT_Xmu_LIBRARY Xmu
      /usr/lib64
      /usr/lib
      /usr/local/lib
      /usr/openwin/lib
      /usr/X11R6/lib
    )

  ENDIF (APPLE)

ENDIF (WIN32)

SET( GLUT_FOUND "NO" )
IF(GLUT_INCLUDE_DIR)
  IF(GLUT_glut_LIBRARY)
    # Is -lXi and -lXmu required on all platforms that have it?
    # If not, we need some way to figure out what platform we are on.
    SET( GLUT_LIBRARIES
      ${GLUT_glut_LIBRARY}
      ${GLUT_Xmu_LIBRARY}
      ${GLUT_Xi_LIBRARY} 
    )
    SET( GLUT_FOUND "YES" )

#The following deprecated settings are for backwards compatibility with CMake1.4
    SET (GLUT_LIBRARY ${GLUT_LIBRARIES})
    SET (GLUT_INCLUDE_PATH ${GLUT_INCLUDE_DIR})

  ENDIF(GLUT_glut_LIBRARY)
ENDIF(GLUT_INCLUDE_DIR)

MARK_AS_ADVANCED(
  GLUT_INCLUDE_DIR
  GLUT_glut_LIBRARY
  GLUT_Xmu_LIBRARY
  GLUT_Xi_LIBRARY
  OPENGL_LIBRARY_DIR
)
