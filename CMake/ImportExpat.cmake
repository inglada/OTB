MESSAGE(STATUS "Importing Expat...")
FIND_PACKAGE(EXPAT)
IF(EXPAT_FOUND)
  OPTION(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." ON)
ELSE(EXPAT_FOUND)
  OPTION(OTB_USE_EXTERNAL_EXPAT "Use an outside build of EXPAT." OFF)
ENDIF(EXPAT_FOUND)

MARK_AS_ADVANCED(OTB_USE_EXTERNAL_EXPAT)

IF(OTB_USE_EXTERNAL_EXPAT)
    FIND_PACKAGE(EXPAT)

    IF (NOT EXPAT_FOUND)
		FIND_PATH(EXPAT_INCLUDE_DIR NAMES expat.h PATHS ${OSGEO4W_INCLUDE})
		FIND_LIBRARY(EXPAT_LIBRARY NAMES expat libexpat PATHS ${OSGEO4W_LIB})
	ENDIF(NOT EXPAT_FOUND)
	
    IF(NOT EXPAT_INCLUDE_DIR OR NOT EXPAT_LIBRARY)
        MESSAGE(FATAL_ERROR
                  "Cannot build OTB project without lib EXPAT.  Please set EXPAT_DIR or set OTB_USE_EXTERNAL_EXPAT OFF to use INTERNAL EXPAT set on OTB/Utilities repository.")
    ENDIF(NOT EXPAT_INCLUDE_DIR OR NOT EXPAT_LIBRARY)
    
    MESSAGE(STATUS "  Using Expat external version")
    MESSAGE(STATUS "  Expat includes : ${EXPAT_INCLUDE_DIR}")
    MESSAGE(STATUS "  Expat library  : ${EXPAT_LIBRARY}")
ELSE(OTB_USE_EXTERNAL_EXPAT)
    ADD_DEFINITIONS(-DOTB_USE_INTERNAL_EXPAT)
    INCLUDE_DIRECTORIES(BEFORE "${OTB_SOURCE_DIR}/Utilities/otbexpat")
    INCLUDE_DIRECTORIES(BEFORE "${OTB_BINARY_DIR}/Utilities/otbexpat")
    IF(NOT BUILD_SHARED_LIBS)
      ADD_DEFINITIONS(-DXML_STATIC)
    ENDIF(NOT BUILD_SHARED_LIBS)
    
    MESSAGE(STATUS "  Using Expat internal version")
ENDIF(OTB_USE_EXTERNAL_EXPAT)
