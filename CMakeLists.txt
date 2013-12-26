set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (citysim)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

if (WIN32)
  # X11 disabled by default for WIN32 (not cygwin)
  option (BUILD_X11 "Build a version linked against X11" OFF)
else ()
  option (BUILD_X11 "Build a version linked against X11" ON)
endif ()


if (CYGWIN)
  # SDL disabled by default for cygwin
  option (BUILD_SDL2 "Build a version linked against SDL2" OFF)
else ()
  option (BUILD_SDL2 "Build a version linked against SDL2" ON)
endif ()

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
set(CMAKE_CXX_FLAGS_RELEASESTATIC "-O4 -DNDEBUG -static-libstdc++ -static-libgcc")

set(SOURCES
  "src/city.cpp"
  "src/clock.cpp"
  "src/joblist.cpp"
  "src/logger.cpp"
  "src/main.cpp"
  ## Components
  "src/components/ai.cpp"
  "src/components/citizenname.cpp"
  "src/components/foodstuff.cpp"
  "src/components/furniture.cpp"
  "src/components/jobprovider.cpp"
  "src/components/movable.cpp"
  "src/components/pathfind.cpp"
  "src/components/room.cpp"
  "src/components/skills.cpp"
  ## Entities
  "src/entities/citizen.cpp"
  "src/entities/entity.cpp"
  "src/entities/garbage.cpp"
  "src/entities/workroom.cpp"
  ## Views
  "src/views/hud.cpp"
  "src/views/mapview.cpp"
  "src/views/viewstack.cpp"
  "src/views/mainview.cpp"
  "src/views/helpview.cpp"
  "src/views/unitview.cpp"
  "src/views/announceview.cpp"
  "src/views/designview.cpp"
  "src/views/statustext.cpp"
  )

if (BUILD_X11)
  find_package(X11)

  if (X11_FOUND)
    include_directories(${X11_INCLUDE_DIR})
    add_executable(alphacomplex-x11 ${SOURCES} "src/graphics/graphicsX.cpp")
    target_link_libraries(alphacomplex-x11 ${X11_LIBRARIES})
    set_target_properties(alphacomplex-x11 PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_X11=1")
  else ()
    message("WARNING!! X11 not found, will not build X11 executable.")
  endif ()
endif (BUILD_X11)

if (BUILD_SDL2)
  find_package(SDL2_ttf)
  if (SDL2_ttf_FOUND)
    include_directories(${SDL2_ttf_INCLUDE_DIRS})
    add_executable(alphacomplex-sdl ${SOURCES} "src/graphics/graphicsSDL.cpp")
    target_link_libraries(alphacomplex-sdl ${SDL2_ttf_LIBRARIES})
    set_target_properties(alphacomplex-sdl PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_SDL=1")
  else ()
    message("WARNING!! SDL2_ttf not found, will not build SDL executable.")
  endif ()
endif (BUILD_SDL2)
