# $Id$
#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(OTB_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_BINARY_DIR})


# Include directories from the Boost build tree.
IF(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${Boost_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Utilities/BGL
    ${OTB_SOURCE_DIR}/Utilities/BGL/boost
)
ENDIF(OTB_USE_EXTERNAL_BOOST)



# These directories are always needed.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_SOURCE_DIR}/Code/Common
  ${OTB_SOURCE_DIR}/Code/BasicFilters
  ${OTB_SOURCE_DIR}/Code/IO
  ${OTB_SOURCE_DIR}/Code/ChangeDetection
  ${OTB_SOURCE_DIR}/Code/FeatureExtraction
  ${OTB_SOURCE_DIR}/Code/Learning
  ${OTB_SOURCE_DIR}/Code/MultiScale
  ${OTB_SOURCE_DIR}/Code/SpatialReasoning
  ${OTB_SOURCE_DIR}/Code/DisparityMap
  ${OTB_SOURCE_DIR}/Code/Visu
  ${OTB_SOURCE_DIR}/Code/Visualization
  ${OTB_SOURCE_DIR}/Code/Gui
  ${OTB_SOURCE_DIR}/Code/Fusion
  ${OTB_SOURCE_DIR}/Code/Projections
  ${OTB_SOURCE_DIR}/Code/Radiometry
  ${OTB_SOURCE_DIR}/Code/SARPolarimetry
  ${OTB_SOURCE_DIR}/Code/Markov
  ${OTB_SOURCE_DIR}/Utilities/otbsvm
  ${OTB_SOURCE_DIR}/Utilities/otbossim
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim
  ${OTB_SOURCE_DIR}/Utilities/otbossimplugins
  #Temporary until the SAR plugins are fully integrated:
#   ${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim/projection/otb
#   ${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim/projection
# Add otbossimplugins/ossim/otb into the include directories path
  ${OTB_SOURCE_DIR}/Utilities/otbossimplugins/ossim
  ${OTB_SOURCE_DIR}/Utilities/dxflib
  ${OTB_SOURCE_DIR}/Utilities/InsightJournal
  ${OTB_SOURCE_DIR}/Utilities/otb6S
  ${OTB_SOURCE_DIR}/Utilities/tinyXMLlib
#  ${OTB_SOURCE_DIR}/Utilities/otbgalib
  ${OTB_SOURCE_DIR}/Utilities/otbkml
  ${OTB_SOURCE_DIR}/Utilities/otbkml/src
  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party
#  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3
#  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3/contrib
  ${OTB_SOURCE_DIR}/Utilities/otbliblas/include
  ${OTB_SOURCE_DIR}/Utilities/otbedison/segm
  ${OTB_SOURCE_DIR}/Utilities/otbedison/prompt
  ${OTB_SOURCE_DIR}/Utilities/otbedison/edge
  ${OTB_SOURCE_DIR}/Utilities/otbsiftfast
  ${OTB_BINARY_DIR}/Code/Visualization

)

IF(NOT OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party/boost_1_34_1)
ENDIF(NOT OTB_USE_EXTERNAL_BOOST)

IF(OTB_COMPILE_JPEG2000)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
        ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg
        ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg/libopenjpeg)
ENDIF(OTB_COMPILE_JPEG2000)

#-----------------------------------------------------------------------------
# Include directories from the ossim build tree
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
        ${OTB_BINARY_DIR}/Utilities/otbossim/include
)


#-----------------------------------------------------------------------------
# Include directories from the GDAL build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${GDAL_INCLUDE_DIRS} )
#-----------------------------------------------------------------------------
# Include directories from the CURL build tree.
IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

#-----------------------------------------------------------------------------
# Include directories from the MAPNIK build tree.
IF(OTB_USE_MAPNIK)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${MAPNIK_INCLUDE_DIR} ${FREETYPE2_INCLUDE_DIR})
ENDIF(OTB_USE_MAPNIK)

#-----------------------------------------------------------------------------
# Include directories from the ITK build tree.
IF(OTB_USE_EXTERNAL_ITK)
#        INCLUDE(${ITK_SOURCE_DIR}/Utilities/itkThirdParty.cmake)
# 	INCLUDE(${ITK_SOURCE_DIR}/itkIncludeDirectories.cmake)
#	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
#	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib
# 	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib/opt)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
   			 ${ITK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_ITK)
	IF(NOT ITK_INSTALL_INCLUDE_DIR)
  		SET(ITK_INSTALL_INCLUDE_DIR "/include/otb/Utilities/ITK")
	ENDIF(NOT ITK_INSTALL_INCLUDE_DIR)
        INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkThirdParty.cmake)
 	INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/itkIncludeDirectories.cmake)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
# 2008/12/18: Added ITK include dir
        ${ITK_INCLUDE_DIRS_BUILD_TREE}
        ${ITK_INCLUDE_DIRS_BUILD_TREE_CXX}
        ${ITK_INCLUDE_DIRS_SYSTEM}
	${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/v3p/netlib
	${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/v3p/netlib/opt)
ENDIF(OTB_USE_EXTERNAL_ITK)

#For FLTK header file
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${FLTK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
                ${OTB_SOURCE_DIR}/Utilities/FLTK
            ${OTB_SOURCE_DIR}/Utilities/FLTK/src
            ${OTB_BINARY_DIR}/Utilities/FLTK )
ENDIF(OTB_USE_EXTERNAL_FLTK)


#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in OTB.  These include
# directories will NOT be available to user projects.
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX)

SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX} )

#-----------------------------------------------------------------------------
# Include FLTK variables.
#IF(NOT OTB_USE_EXTERNAL_FLTK)
#        IF(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#                MESSAGE(FATAL_ERROR "Impossible to find the file ${FLTK_BINARY_DIR}/FLTKConfig.cmake. You must generate FLTK with cmake process.")
#        ELSE(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#                INCLUDE(${FLTK_BINARY_DIR}/FLTKConfig.cmake)
#        ENDIF(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#ELSE(OTB_USE_EXTERNAL_FLTK)
#        IF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
#                INCLUDE(${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake)
#        ENDIF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
#ENDIF(NOT OTB_USE_EXTERNAL_FLTK)



#-----------------------------------------------------------------------------
# Include directories from libraries build tree.


#For GLU header file
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${OTB_GLU_INCLUDE_PATH} )

#For GDAL header file
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${GDAL_INCLUDE_DIRS} )

#For EXPAT header file
IF(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  	        ${EXPAT_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbexpat)
ENDIF(OTB_USE_EXTERNAL_EXPAT)

# For OpenThreads
IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbopenthreads/OpenThreads/include
	)
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

#For ITK header file
IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS_BUILD_TREE}
   			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} 	)
ENDIF(OTB_USE_EXTERNAL_ITK)







#-----------------------------------------------------------------------------
# Include directories from the install tree.
# Alls install dirs must been RELATIVE PATH, except for the external libraries, specify by the ABSOLUE PATH
SET(OTB_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}${OTB_INSTALL_INCLUDE_DIR}")
#           SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS} )

#           SET(OTB_INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/otb")
#           SET(OTB_INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
#           SET(OTB_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/otb")

SET(OTB_INCLUDE_RELATIVE_DIRS )
SET(OTB_INCLUDE_ABSOLUE_DIRS )

#For BOOST header file
IF(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${Boost_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_BOOST)
  SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
        Utilities/BGL
        Utilities/BGL/boost)
ENDIF(OTB_USE_EXTERNAL_BOOST)


SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
  Common
  BasicFilters
  IO
  ChangeDetection
  FeatureExtraction
  Learning
  MultiScale
  SpatialReasoning
  DisparityMap
  Visu
  Visualization
  Gui
  Projections
  Radiometry
  SARPolarimetry
  Markov
  Fusion
  Utilities
  Utilities/ITK
  Utilities/otbsvm
  Utilities/otbossim
  Utilities/otbossim/include
  Utilities/otbossim/include/ossim
  Utilities/otbossimplugins
  Utilities/InsightJournal
  Utilities/otb6S
  Utilities/tinyXMLlib
#  Utilities/otbgalib
  Utilities/otbkml
  Utilities/otbkml/src
  Utilities/otbkml/third_party
  Utilities/otbkml/third_party/boost_1_34_1
# Utilities/otbkml/third_party/zlib-1.2.3
# Utilities/otbkml/third_party/zlib-1.2.3/contrib
  Utilities/otbliblas/include
  Utilities/otbedison
  Utilities/otbsiftfast
)

IF(OTB_COMPILE_JPEG2000)
  SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
        Utilities/otbopenjpeg
        Utilities/otbopenjpeg/libopenjpeg)
ENDIF(OTB_COMPILE_JPEG2000)

#For GDAL header file
SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  		${GDAL_INCLUDE_DIRS} )

#For EXPAT header file
IF(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	        ${EXPAT_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
                Utilities/otbexpat)
ENDIF(OTB_USE_EXTERNAL_EXPAT)

#For CURL header file
IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

#For MAPNIK header file
IF(OTB_USE_MAPNIK)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${MAPNIK_INCLUDE_DIR} )
ENDIF(OTB_USE_MAPNIK)

# For OpentTreads  header file
IF(OTB_USE_EXTERNAL_OPENTHREADS)
  SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
    ${OPENTHREADS_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_OPENTHREADS)
        SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
                Utilities/otbopenthreads/OpenThreads/include)
ENDIF(OTB_USE_EXTERNAL_OPENTHREADS)


#For FLTK header file
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  		        ${FLTK_INCLUDE_DIRS})
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
  		        Utilities/FLTK)
ENDIF(OTB_USE_EXTERNAL_FLTK)

#For GLU header file
SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  		${OTB_GLU_INCLUDE_PATH} )

#For ITK header file
IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)

    #Add prefix path "Utilities/ITK" For ITK_INCLUDE_RELATIVE_DIRS paths
    FOREACH(DIR ${ITK_INCLUDE_RELATIVE_DIRS})
        LIST(APPEND OTB_INCLUDE_RELATIVE_DIRS Utilities/ITK/${DIR})
    ENDFOREACH(DIR)


# JULIEN Seems that ITK_INCLUDE_DIRS_CONFIG replaces ITK_INCLUDE_DIRS_INSTALL_TREE
#             SET(ITK_INCLUDE_DIRS  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK/)
#             FOREACH(DIR ${ITK_INCLUDE_RELATIVE_DIRS})
#               LIST(APPEND ITK_INCLUDE_DIRS ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK/${DIR})
#             ENDFOREACH(DIR)
#             IF(ITK_INCLUDE_DIRS_SYSTEM)
#               LIST(APPEND ITK_INCLUDE_DIR ${ITK_INCLUDE_DIRS_SYSTEM})
#             ENDIF(ITK_INCLUDE_DIRS_SYSTEM)


#THOMAS
#                SET(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
#           			${ITK_INCLUDE_DIRS_INSTALL_TREE}
#JULIEN
#           			${ITK_INCLUDE_DIRS}
#           )
#			${ITK_INCLUDE_DIRS_BUILD_TREE}
#			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} )

ENDIF(OTB_USE_EXTERNAL_ITK)
