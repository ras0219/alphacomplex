cmake_minimum_required (VERSION 2.6)
project (citysim)

option (BUILD_X11 "Build a version linked against X11" ON)
option (BUILD_SDL2 "Build a version linked against SDL2" OFF)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
set(CMAKE_CXX_FLAGS_RELEASESTATIC "${CMAKE_CXX_FLAGS_RELEASESTATIC} -O4 -DNDEBUG -static-libstdc++ -static-libgcc")

set(SOURCES 
  "src/main.cpp"
  "src/entity.cpp"
  "src/city.cpp"
  "src/pathfind.cpp"
  "src/garbage.cpp"
  "src/joblist.cpp"
  "src/job.cpp"
  "src/citizen.cpp"
  "src/room.cpp"
  "src/hud.cpp"
  "src/workroom.cpp"
  "src/mapview.cpp"
  "src/viewstack.cpp"
  "src/mainview.cpp"
  "src/helpview.cpp"
  "src/unitview.cpp"
  "src/announceview.cpp"
  "src/designview.cpp"
  "src/clock.cpp"
  "src/skills.cpp")


##TO-DO: THROW AN ERROR IF ONE OF THEM IS NOT SELECTED, OR EMPTY GRAPHICS
if(BUILD_X11)
  find_package(X11 REQUIRED)

  add_executable(alphacomplex-x11 ${SOURCES} "src/graphicsX.cpp")
  target_link_libraries(alphacomplex-x11 ${X11_LIBRARIES})
  set_target_properties(alphacomplex-x11 PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_X11=1")
endif (BUILD_X11)

if(BUILD_SDL2)
  IF(NOT APPLE)
    INCLUDE(FindPkgConfig)
    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

    add_executable(alphacomplex-sdl ${SOURCES} "src/graphicsSDL.cpp")
    target_link_libraries(alphacomplex-sdl ${SDL2_LIBRARIES} "-lSDL2_ttf")
    set_target_properties(alphacomplex-sdl PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_SDL=1")
  ENDIF(NOT APPLE)
  IF(APPLE)
    FIND_PATH(SDL2_INCLUDE_DIR SDL.h
      HINTS
      $ENV{SDL2DIR}
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
    #MESSAGE("SDL2_INCLUDE_DIR is ${SDL2_INCLUDE_DIR}")

    FIND_LIBRARY(SDL2_LIBRARY_TEMP
      NAMES SDL2
      HINTS
      $ENV{SDL2DIR}
      PATH_SUFFIXES lib64 lib
      PATHS
      /sw
      /opt/local
      /opt/csw
      /opt
      )

    #MESSAGE("SDL2_LIBRARY_TEMP is ${SDL2_LIBRARY_TEMP}")

    IF(NOT SDL2_BUILDING_LIBRARY)
      IF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
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
      ENDIF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
    ENDIF(NOT SDL2_BUILDING_LIBRARY)

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

    SET(SDL2_FOUND "NO")
    IF(SDL2_LIBRARY_TEMP)
      # For SDL2main
      IF(NOT SDL2_BUILDING_LIBRARY)
        IF(SDL2MAIN_LIBRARY)
          SET(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
        ENDIF(SDL2MAIN_LIBRARY)
      ENDIF(NOT SDL2_BUILDING_LIBRARY)

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

      SET(SDL2_FOUND "YES")
    ENDIF(SDL2_LIBRARY_TEMP)

    INCLUDE(FindPackageHandleStandardArgs)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 REQUIRED_VARS SDL2_LIBRARIES SDL2_INCLUDE_DIR)
    set(SDL2_LIBRARIES ${SDL2_LIBRARIES} "-framework SDL2_ttf")
    set(SOURCES ${SOURCES} "src/graphicsSDL.cpp")
    include_directories(${SDL2_INCLUDE_DIR} "/Library/Frameworks/SDL2_ttf.framework/Headers/")

    add_executable(alphacomplex-sdl ${SOURCES} "src/graphicsSDL.cpp")
    target_link_libraries(alphacomplex-sdl ${SDL2_LIBRARIES} "-framework SDL2_ttf")
    set_target_properties(alphacomplex-sdl PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_SDL=1")
  ENDIF(APPLE)
endif (BUILD_SDL2)
