set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (citysim)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

option (BUILD_X11 "Build a version linked against X11" ON)
option (BUILD_SDL2 "Build a version linked against SDL2" ON)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
set(CMAKE_CXX_FLAGS_RELEASESTATIC "-O4 -DNDEBUG -static-libstdc++ -static-libgcc")

set(SOURCES
  "src/main.cpp"
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
  "src/skills.cpp"
  "src/statustext.cpp"
  ## Entity Component Systems
  "src/entity.cpp"
  "src/citizenname.cpp"
  "src/ai.cpp"
  "src/movable.cpp"
  )

if (BUILD_X11)
  find_package(X11)

  if (X11_FOUND)
    include_directories(${X11_INCLUDE_DIR})
    add_executable(alphacomplex-x11 ${SOURCES} "src/graphicsX.cpp")
    target_link_libraries(alphacomplex-x11 ${X11_LIBRARIES})
    set_target_properties(alphacomplex-x11 PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_X11=1")
  else ()
    message("WARNING!! X11 not found, will not build X11 executable.")
  endif ()
endif (BUILD_X11)

if (BUILD_SDL2)
  find_package(SDL2)
  find_package(SDL2_ttf)
  if (SDL2_FOUND AND SDL2_ttf_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_ttf_INCLUDE_DIRS})
    add_executable(alphacomplex-sdl ${SOURCES} "src/graphicsSDL.cpp")
    target_link_libraries(alphacomplex-sdl ${SDL2_LIBRARIES} ${SDL2_ttf_LIBRARIES})
    set_target_properties(alphacomplex-sdl PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_SDL=1")
  else ()
    message("WARNING!! SDL2 or SDL2_ttf not found, will not build SDL executable.")
  endif ()
endif (BUILD_SDL2)
