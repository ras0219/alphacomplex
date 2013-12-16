# - Try to find SDL2
# Once done this will define
#  SDL2_FOUND - System has SDL2
#  SDL2_INCLUDE_DIRS - The SDL2 include directories
#  SDL2_LIBRARIES - The libraries needed to use SDL2
#
# This cmake module is based on cmake code found on github...

include(LibFindMacros)

if (UNIX AND NOT APPLE)
  set(SDL2_PKGCONF_INCLUDE_DIRS)
  set(SDL2_PKGCONF_LIBRARY_DIRS)
  libfind_pkg_check_modules(SDL2_PKGCONF SDL2)

  #find_package (PkgConfig QUIET)

  # include (FindPkgConfig)
  #if (PkgConfig_FOUND)
  #  pkg_search_module (SDL2 REQUIRED sdl2)
  #endif ()

  find_path (SDL2_INCLUDE_DIR
    NAMES SDL.h
    PATH_SUFFIXES include/SDL2 include
    PATHS ${SDL2_PKGCONF_INCLUDE_DIRS}
  )

  find_path (SDL2_LIBRARY
    NAMES SDL2
    PATH_SUFFIXES lib64 lib
    PATHS ${SDL2_PKGCONF_LIBRARY_DIRS}
  )

  set (SDL2_PROCESS_INCLUDES SDL2_INCLUDE_DIR)
  set (SDL2_PROCESS_LIBS SDL2_LIBRARIES)

  libfind_process (SDL2)
elseif (APPLE)
  FIND_PATH(SDL2_INCLUDE_DIR SDL.h
    HINTS $ENV{SDL2DIR}
    PATH_SUFFIXES include/SDL2 include
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include/SDL2
    /usr/include/SDL2
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
    )
  MESSAGE("SDL2_INCLUDE_DIRS is ${SDL2_INCLUDE_DIRS}")

  FIND_LIBRARY(SDL2_LIBRARY_TEMP NAMES SDL2
    HINTS $ENV{SDL2DIR}
    PATH_SUFFIXES lib64 lib
    PATHS
    /sw
    /opt/local
    /opt/csw
    /opt
    )

  MESSAGE("SDL2_LIBRARY_TEMP is ${SDL2_LIBRARY_TEMP}")

  IF(NOT ${SDL2_INCLUDE_DIRS} MATCHES ".framework")
    # Non-OS X framework versions expect you to also dynamically link to
    # SDL2main. This is mainly for Windows and OS X. Other (Unix) platforms
    # seem to provide SDL2main for compatibility even though they don't
    # necessarily need it.
    FIND_LIBRARY(SDL2MAIN_LIBRARY
      NAMES SDL2main
      HINTS
      $ENV{SDL2DIR}
      PATH_SUFFIXES lib64 lib
      PATHS
      /sw
      /opt/local
      /opt/csw
      /opt
      )
  ENDIF(NOT ${SDL2_INCLUDE_DIRS} MATCHES ".framework")

  # SDL2 may require threads on your system.
  # The Apple build may not need an explicit flag because one of the
  # frameworks may already provide it.
  # But for non-OSX systems, I will use the CMake Threads package.
  IF(NOT APPLE)
    FIND_PACKAGE(Threads)
  ENDIF(NOT APPLE)

  # MinGW needs an additional library, mwindows
  # It's total link flags should look like -lmingw32 -lSDL2main -lSDL2 -lmwindows
  # (Actually on second look, I think it only needs one of the m* libraries.)
  IF(MINGW)
    SET(MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW")
  ENDIF(MINGW)

  IF(SDL2_LIBRARY_TEMP)
    # For SDL2main
    IF(SDL2MAIN_LIBRARY)
      SET(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
    ENDIF(SDL2MAIN_LIBRARY)

    # For OS X, SDL2 uses Cocoa as a backend so it must link to Cocoa.
    # CMake doesn't display the -framework Cocoa string in the UI even
    # though it actually is there if I modify a pre-used variable.
    # I think it has something to do with the CACHE STRING.
    # So I use a temporary variable until the end so I can set the
    # "real" variable in one-shot.
    IF(APPLE)
      SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} "-framework Cocoa")
    ENDIF(APPLE)

    # For threads, as mentioned Apple doesn't need this.
    # In fact, there seems to be a problem if I used the Threads package
    # and try using this line, so I'm just skipping it entirely for OS X.
    IF(NOT APPLE)
      SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
    ENDIF(NOT APPLE)

    # For MinGW library
    IF(MINGW)
      SET(SDL2_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL2_LIBRARY_TEMP})
    ENDIF(MINGW)

    # Set the final string here so the GUI reflects the final state.
    SET(SDL2_LIBRARIES ${SDL2_LIBRARY_TEMP} CACHE STRING "Where the SDL2 Library can be found")
    # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
    SET(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_TEMP}" CACHE INTERNAL "")

#    SET(SDL2_FOUND "YES")
  ENDIF(SDL2_LIBRARY_TEMP)

  include (FindPackageHandleStandardArgs)

  find_package_handle_standard_args (SDL2 DEFAULT_MSG SDL2_LIBRARIES SDL2_INCLUDE_DIRS)
  mark_as_advanced (SDL2_LIBRARIES SDL2_INCLUDE_DIRS)

endif ()
