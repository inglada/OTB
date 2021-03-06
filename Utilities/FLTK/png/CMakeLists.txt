PROJECT(FLTKPNG)
INCLUDE_REGULAR_EXPRESSION("^png.*$")

INCLUDE_DIRECTORIES(${FLTKZLIB_SOURCE_DIR})

IF(ZLIB_INCLUDE_DIR)
  INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
ENDIF(ZLIB_INCLUDE_DIR)

INCLUDE_DIRECTORIES(${FLTKPNG_SOURCE_DIR})
INCLUDE_DIRECTORIES(${FLTKPNG_BINARY_DIR})

# source files for png
SET(PNG_SRCS
  pngget.c    pngrio.c    pngwrite.c
  png.c       pngmem.c    pngrtran.c  pngtrans.c  pngwtran.c
  pngerror.c  pngpread.c  pngrutil.c  pngwutil.c
  pngread.c   pngset.c   pngwio.c
  )

ADD_LIBRARY(fltk_png ${PNG_SRCS})
INSTALL(TARGETS fltk_png
        RUNTIME DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
        LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
        ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)


IF(UNIX)
  TARGET_LINK_LIBRARIES(fltk_png ${FLTK_ZLIB_LIBRARIES} -lm)
ELSE(UNIX)
  TARGET_LINK_LIBRARIES(fltk_png ${FLTK_ZLIB_LIBRARIES})
ENDIF(UNIX)

