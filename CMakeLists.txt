cmake_minimum_required (VERSION 2.6)
project (citysim)

set (GRAPHICS "X11")

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
set(CMAKE_CXX_FLAGS_RELEASESTATIC "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG -static-libstdc++ -static-libgcc")

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
  "src/mapview.cpp")


##TO-DO: THROW AN ERROR IF ONE OF THEM IS NOT SELECTED, OR EMPTY GRAPHICS
if(GRAPHICS STREQUAL "X11")
  find_package(X11 REQUIRED)
  set(SOURCES ${SOURCES} "src/graphicsX.cpp")
endif ()

if(GRAPHICS STREQUAL "SDL2")
  INCLUDE(FindPkgConfig)
  PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
  INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
  set(SOURCES ${SOURCES} "src/graphicsSDL.cpp")
endif ()

add_executable(alphacomplex ${SOURCES})


target_link_libraries(alphacomplex ${X11_LIBRARIES} ${SDL2_LIBRARIES} "-lSDL2_ttf")
