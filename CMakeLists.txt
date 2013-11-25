cmake_minimum_required (VERSION 2.6)
project (citysim)

include_directories(/opt/X11/include)
include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -I${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
set(CMAKE_CXX_FLAGS_RELEASESTATIC "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG -static-libstdc++ -static-libgcc")

find_package(X11 REQUIRED)

add_executable(alphacomplex
  src/main.cpp
  src/entity.cpp
  src/city.cpp
  src/pathfind.cpp
  src/garbage.cpp
  src/joblist.cpp
  src/job.cpp
  src/graphics.cpp
  src/citizen.cpp
  src/room.cpp
  src/hud.cpp
  src/workroom.cpp
  src/mapview.cpp)
target_link_libraries(alphacomplex ${X11_LIBRARIES})
