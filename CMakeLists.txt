set (CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
cmake_minimum_required (VERSION 2.6)
project (citysim)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

include_directories(include)

if (!WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
  set(CMAKE_CXX_FLAGS_RELEASESTATIC "-O4 -DNDEBUG -static-libstdc++ -static-libgcc")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP")
endif()

set(SOURCES
  ## Components
  "src/components/ai.cpp"
  "src/components/citizenname.cpp"
  "src/components/foodstuff.cpp"
  "src/components/furniture.cpp"
  "src/components/jobprovider.cpp"
  "src/components/movable.cpp"
  "src/components/needsai.cpp"
  "src/components/pathfind.cpp"
  "src/components/room.cpp"
  "src/components/sequenceai.cpp"
  "src/components/skills.cpp"
  "src/components/item.cpp"
  ## Entities
  "src/entities/citizen.cpp"
  "src/entities/entity.cpp"
  "src/entities/foods.cpp"
  "src/entities/garbage.cpp"
  "src/entities/workroom.cpp"
  "src/entities/filestorage.cpp"
  "src/entities/subsystem.cpp"
  ## Hydroponics
  "src/hydroponics/hydroponics.cpp"
  "src/hydroponics/hydroponicsai.cpp"
  "src/hydroponics/hydroponicsjobs.cpp"
  "src/hydroponics/hydroponics_table.cpp"
  ## UnitViews
  "src/views/unitviews/assignview.cpp"
  "src/views/unitviews/needsview.cpp"
  "src/views/unitviews/skillsview.cpp"
  ## Views
  "src/views/announceview.cpp"
  "src/views/baseview.cpp"
  "src/views/designview.cpp"
  "src/views/helpview.cpp"
  "src/views/hud.cpp"
  "src/views/mainview.cpp"
  "src/views/mapview.cpp"
  "src/views/navhelper.cpp"
  "src/views/scrollable.cpp"
  "src/views/statustext.cpp"
  "src/views/unitview.cpp"
  "src/views/viewstack.cpp"
  ##
  "src/point.cpp"
  "src/city.cpp"
  "src/citygen.cpp"
  "src/clock.cpp"
  "src/cursor.cpp"
  "src/joblist.cpp"
  "src/logger.cpp"
  "src/main.cpp"
  )

##
#  Build shared source (alphacomplex-base)
##
  
add_library(alphacomplex-base ${SOURCES})

##
#  Build Ncurses version (alphacomplex-nc)
##

if (WIN32)
  # Ncurses disabled by default for WIN32 (not cygwin)
  option (BUILD_NCURSES "Build a ncurses version" OFF)
else ()
  option (BUILD_NCURSES "Build a ncurses version" ON)
endif ()

if (BUILD_NCURSES)
  find_package(Ncurses)

  if (NCURSES_FOUND)
    include_directories(${NCURSES_INCLUDE_DIR})
    add_executable(alphacomplex-nc "src/graphics/graphicsNcurses.cpp")
    target_link_libraries(alphacomplex-nc alphacomplex-base ${NCURSES_LIBRARIES})
    set_target_properties(alphacomplex-nc PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_NCURSES=1")
  else ()
    message("WARNING!! Ncurses not found, will not build Ncurses executable.")
  endif ()
endif (BUILD_NCURSES)


##
#  Build X11 version (alphacomplex-x11)
##

if (WIN32)
  # X11 disabled by default for WIN32 (not cygwin)
  option (BUILD_X11 "Build a version linked against X11" OFF)
else ()
  option (BUILD_X11 "Build a version linked against X11" ON)
endif ()

if (BUILD_X11)
  find_package(X11)

  if (X11_FOUND)
    include_directories(${X11_INCLUDE_DIR})
    add_executable(alphacomplex-x11 "src/graphics/graphicsX.cpp")
    target_link_libraries(alphacomplex-x11 alphacomplex-base ${X11_LIBRARIES})
    set_target_properties(alphacomplex-x11 PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_X11=1")
  else ()
    message("WARNING!! X11 not found, will not build X11 executable.")
  endif ()
endif (BUILD_X11)


##
#  Build SDL2 version (alphacomplex-sdl)
##

if (CYGWIN)
  # SDL disabled by default for cygwin
  option (BUILD_SDL2 "Build a version linked against SDL2" OFF)
else ()
  option (BUILD_SDL2 "Build a version linked against SDL2" ON)
endif ()

if (BUILD_SDL2)
  find_package(SDL2_ttf)
  if (SDL2_ttf_FOUND)
    include_directories(${SDL2_ttf_INCLUDE_DIRS})
	include_directories(sdl)
    add_executable(alphacomplex-sdl "sdl/graphicsSDL.cpp")
    target_link_libraries(alphacomplex-sdl alphacomplex-base ${SDL2_ttf_LIBRARIES})
    set_target_properties(alphacomplex-sdl PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_SDL=1")
  else ()
    message("WARNING!! SDL2_ttf not found, will not build SDL executable.")
  endif ()
endif (BUILD_SDL2)


##
#  Build Web version (alphacomplex-web)
##

if (WIN32)
  # Web disabled by default for WIN32 (not cygwin)
  option (BUILD_WEB "Build a web version" OFF)
else ()
  option (BUILD_WEB "Build a web version" ON)
endif ()

if (BUILD_WEB)
  find_package(LibWebSockets)
  if (LIBWEBSOCKETS_FOUND)
    include_directories(${LIBWEBSOCKETS_INCLUDE_DIR})
    add_executable(alphacomplex-web "src/graphics/graphicsWeb.cpp")
    target_link_libraries(alphacomplex-web alphacomplex-base ${LIBWEBSOCKETS_LIBRARIES})
    set_target_properties(alphacomplex-web PROPERTIES COMPILE_DEFINITIONS "GRAPHICS_WEB=1")
  else ()
    message("WARNING!! LibWebSockets not found, will not build web executable.")
  endif ()
endif (BUILD_WEB)
