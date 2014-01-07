# - Try to find SDL2_ttf
# Once done this will define
#  SDL2_ttf_FOUND - System has SDL2_ttf
#  SDL2_ttf_INCLUDE_DIRS - The SDL2_ttf include directories
#  SDL2_ttf_LIBRARIES - The libraries needed to use SDL2_ttf

include(FindSDL2)

if (SDL2_FOUND)
  if (APPLE)
    find_package(PkgConfig)

    pkg_search_module (PC_SDL2_ttf sdl2_ttf SDL2_ttf)

    find_path (SDL2_ttf_INCLUDE_DIR SDL_ttf.h
      PATH_SUFFIXES include/SDL2 include
      PATHS
      ${PC_SDL2_ttf_INCLUDEDIR}
      ~/Library/Frameworks
      /Library/Frameworks
      )

    find_library (SDL2_ttf_LIBRARY SDL2_ttf
      PATH_SUFFIXES lib64 lib
      PATHS
      ${PC_SDL2_ttf_LIBDIR}
      ~/Library/Frameworks
      /Library/Frameworks
      )

    include(FindPackageHandleStandardArgs)

    find_package_handle_standard_args (SDL2_ttf DEFAULT_MSG SDL2_ttf_LIBRARY SDL2_ttf_INCLUDE_DIR)
    set (SDL2_ttf_INCLUDE_DIRS ${SDL2_ttf_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})
    set (SDL2_ttf_LIBRARIES ${SDL2_ttf_LIBRARY} ${SDL2_LIBRARIES})
    mark_as_advanced (SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIRS)

    set (SDL2_ttf_FOUND ${SDL2_TTF_FOUND})

  else ()

    include(FindPkgConfig)
    pkg_search_module (SDL2_ttf sdl2_ttf SDL2_ttf)

    include(FindPackageHandleStandardArgs)

    find_package_handle_standard_args (SDL2_ttf DEFAULT_MSG SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIRS)
    mark_as_advanced (SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIRS)
  endif ()
endif ()
