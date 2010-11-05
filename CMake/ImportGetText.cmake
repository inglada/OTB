MESSAGE(STATUS "Importing GetText...")

OPTION(OTB_USE_GETTEXT "Get Text Library." ON)

IF(OTB_USE_GETTEXT)
    
    FIND_PACKAGE(Gettext)
    IF(GETTEXT_FOUND)
      FIND_LIBRARY(GETTEXT_LIBRARY gettextlib DOC "GetText library")
      IF(APPLE OR WIN32)
        FIND_LIBRARY(GETTEXT_INTL_LIBRARY intl DOC "GetText intl library")
      ENDIF(APPLE OR WIN32)
    
      IF(GETTEXT_LIBRARY)
          SET(OTB_I18N 1)
          MESSAGE(STATUS
                  "  GetText found, configuring internationalization")
          SET(OTB_LANG $ENV{LANG} CACHE STRING "OTB internationalization (Experimental)")#might want to get the Locale from the system here
          IF(NOT OTB_LANG)
            SET(OTB_LANG "en_EN.UTF-8" CACHE STRING "OTB internationalization (Experimental)" FORCE)
          ENDIF(NOT OTB_LANG)
          SET(OTB_LANG_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/I18n)
          ADD_SUBDIRECTORY(I18n)
          FIND_PATH(GETTEXT_INCLUDE_DIR
            libintl.h
            DOC "Path to gettext include directory (where libintl.h can be found)")
          MARK_AS_ADVANCED(GETTEXT_INCLUDE_DIR)
        
          IF(GETTEXT_INCLUDE_DIR)
            INCLUDE_DIRECTORIES(${GETTEXT_INCLUDE_DIR})
          ENDIF(GETTEXT_INCLUDE_DIR)
      ENDIF(GETTEXT_LIBRARY)
      
      #MESSAGE(STATUS "  Enabling GetText support")
      MESSAGE(STATUS "  GetText includes: ${GETTEXT_INCLUDE_DIR}")
      MESSAGE(STATUS "  GetText library : ${GETTEXT_LIBRARY}")
      IF(APPLE OR WIN32)
        MESSAGE(STATUS "  GetText intl library  : ${GETTEXT_INTL_LIBRARY}")
      ENDIF(APPLE OR WIN32)
    
    ELSE(GETTEXT_FOUND)
      SET(OTB_I18N 0)
      MESSAGE(STATUS
                  "  Gettext not found, internationalization will not be available")
    ENDIF(GETTEXT_FOUND)
    
ELSE(OTB_USE_GETTEXT)
    MESSAGE(STATUS "  Disabling GetText support")
ENDIF(OTB_USE_GETTEXT)

