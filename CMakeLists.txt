cmake_minimum_required (VERSION 2.6)
project (citysim)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
set(CMAKE_CXX_FLAGS_RELEASESTATIC "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG -static-libstdc++ -static-libgcc")

find_package(X11 REQUIRED)

add_executable(citysim
  main.cpp
  entity.cpp
  city.cpp
  pathfind.cpp
  garbage.cpp
  joblist.cpp
  graphics.cpp
  elf.cpp
  room.cpp
  workroom.cpp)
target_link_libraries(citysim ${X11_LIBRARIES})
