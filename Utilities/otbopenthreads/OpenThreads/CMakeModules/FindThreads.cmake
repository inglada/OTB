# - This module determines the thread library of the system.
# The following variables are set
#  CMAKE_THREAD_LIBS_INIT     - the thread library
#  CMAKE_USE_SPROC_INIT       - are we using sproc?
#  CMAKE_USE_WIN32_THREADS_INIT - using WIN32 threads?
#  CMAKE_USE_PTHREADS_INIT    - are we using pthreads
#  CMAKE_HP_PTHREADS_INIT     - are we using hp pthreads

# This module is a modification of the version found in the official
# CMake distribution. The problem is that the built in version's Sproc
# handling is undesirable for us. The official module assumes you use
# Sproc if it exists on your system. We assume you might have both Pthreads
# and Sproc on your system in which case the user needs to choose.
# Furthermore, we prefer to provide Pthreads as the default option instead
# of Sproc.

INCLUDE (CheckIncludeFile)
INCLUDE (CheckIncludeFiles)
INCLUDE (CheckLibraryExists)

# Do we have sproc?
IF(CMAKE_SYSTEM MATCHES IRIX)
  CHECK_INCLUDE_FILES("sys/types.h;sys/prctl.h"  CMAKE_HAVE_SPROC_H)
ENDIF(CMAKE_SYSTEM MATCHES IRIX)

CHECK_INCLUDE_FILE("pthread.h" CMAKE_HAVE_PTHREAD_H)
IF(CMAKE_HAVE_PTHREAD_H)
    # We have pthread.h
    # Let's check for the library now.
    SET(CMAKE_HAVE_THREADS_LIBRARY)
    IF(NOT THREADS_HAVE_PTHREAD_ARG)
      # Do we have -lpthreads
      CHECK_LIBRARY_EXISTS(pthreads pthread_create "" CMAKE_HAVE_PTHREADS_CREATE)
      IF(CMAKE_HAVE_PTHREADS_CREATE)
        SET(CMAKE_THREAD_LIBS_INIT "-lpthreads")
        SET(CMAKE_HAVE_THREADS_LIBRARY 1)
      ENDIF(CMAKE_HAVE_PTHREADS_CREATE)
      # Ok, how about -lpthread
      CHECK_LIBRARY_EXISTS(pthread pthread_create "" CMAKE_HAVE_PTHREAD_CREATE)
      IF(CMAKE_HAVE_PTHREAD_CREATE)
        SET(CMAKE_THREAD_LIBS_INIT "-lpthread")
        SET(CMAKE_HAVE_THREADS_LIBRARY 1)
      ENDIF(CMAKE_HAVE_PTHREAD_CREATE)
      IF(CMAKE_SYSTEM MATCHES "SunOS.*")
        # On sun also check for -lthread
        CHECK_LIBRARY_EXISTS(thread thr_create "" CMAKE_HAVE_THR_CREATE)
        IF(CMAKE_HAVE_THR_CREATE)
          SET(CMAKE_THREAD_LIBS_INIT "-lthread")
          SET(CMAKE_HAVE_THREADS_LIBRARY 1)
        ENDIF(CMAKE_HAVE_THR_CREATE)
      ENDIF(CMAKE_SYSTEM MATCHES "SunOS.*")
    ENDIF(NOT THREADS_HAVE_PTHREAD_ARG)

    IF(NOT CMAKE_HAVE_THREADS_LIBRARY)
      # If we did not found -lpthread, -lpthread, or -lthread, look for -pthread
      IF("THREADS_HAVE_PTHREAD_ARG" MATCHES "^THREADS_HAVE_PTHREAD_ARG")
        MESSAGE(STATUS "Check if compiler accepts -pthread")
        TRY_RUN(THREADS_PTHREAD_ARG THREADS_HAVE_PTHREAD_ARG
          ${CMAKE_BINARY_DIR}
          ${CMAKE_ROOT}/Modules/CheckForPthreads.c
          CMAKE_FLAGS -DLINK_LIBRARIES:STRING=-pthread
          OUTPUT_VARIABLE OUTPUT)
        IF(THREADS_HAVE_PTHREAD_ARG)
          IF(THREADS_PTHREAD_ARG MATCHES "^2$")
            MESSAGE(STATUS "Check if compiler accepts -pthread - yes")
          ELSE(THREADS_PTHREAD_ARG MATCHES "^2$")
            MESSAGE(STATUS "Check if compiler accepts -pthread - no")
            FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log 
              "Determining if compiler accepts -pthread returned ${THREADS_PTHREAD_ARG} instead of 2. The compiler had the following output:\n${OUTPUT}\n\n")
          ENDIF(THREADS_PTHREAD_ARG MATCHES "^2$")
        ELSE(THREADS_HAVE_PTHREAD_ARG)
          MESSAGE(STATUS "Check if compiler accepts -pthread - no")
          FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log 
            "Determining if compiler accepts -pthread failed with the following output:\n${OUTPUT}\n\n")
        ENDIF(THREADS_HAVE_PTHREAD_ARG)
      ENDIF("THREADS_HAVE_PTHREAD_ARG" MATCHES "^THREADS_HAVE_PTHREAD_ARG")
      IF(THREADS_HAVE_PTHREAD_ARG)
        SET(CMAKE_THREAD_LIBS_INIT "-pthread")
      ELSE(THREADS_HAVE_PTHREAD_ARG)
      ENDIF(THREADS_HAVE_PTHREAD_ARG)
    ENDIF(NOT CMAKE_HAVE_THREADS_LIBRARY)
ELSE(CMAKE_HAVE_PTHREAD_H)
	IF(CMAKE_HAVE_SPROC_H)
	  # We have sproc but not pthread so the answer is obvious.
	  SET(CMAKE_USE_SPROC_INIT 1)
	ENDIF(CMAKE_HAVE_SPROC_H)
ENDIF(CMAKE_HAVE_PTHREAD_H)

IF(CMAKE_THREAD_LIBS_INIT)
  SET(CMAKE_USE_PTHREADS_INIT 1)
ENDIF(CMAKE_THREAD_LIBS_INIT)

IF(CMAKE_SYSTEM MATCHES "Windows")
  SET(CMAKE_USE_WIN32_THREADS_INIT 1)
ENDIF(CMAKE_SYSTEM MATCHES "Windows")

IF(CMAKE_USE_PTHREADS_INIT)
  IF(CMAKE_SYSTEM MATCHES "HP-UX-*")
    SET(CMAKE_THREAD_LIBS_INIT "-lcma")
    SET(CMAKE_USE_PTHREADS_INIT 1)
    SET(CMAKE_HP_PTHREADS_INIT 1)
  ENDIF(CMAKE_SYSTEM MATCHES "HP-UX-*")

  IF(CMAKE_SYSTEM MATCHES "OSF1-V*")
    SET(CMAKE_USE_PTHREADS_INIT 0)
    SET(CMAKE_THREAD_LIBS_INIT )
  ENDIF(CMAKE_SYSTEM MATCHES "OSF1-V*")

  IF(CMAKE_SYSTEM MATCHES "CYGWIN_NT*")
    SET(CMAKE_USE_PTHREADS_INIT 1)
    SET(CMAKE_THREAD_LIBS_INIT )
    SET(CMAKE_USE_WIN32_THREADS_INIT 0)
  ENDIF(CMAKE_SYSTEM MATCHES "CYGWIN_NT*")
ENDIF(CMAKE_USE_PTHREADS_INIT)

