# - Try to find SDL2_ttf
# Once done this will define
#  SDL2_ttf_FOUND - System has SDL2_ttf
#  SDL2_ttf_INCLUDE_DIRS - The SDL2_ttf include directories
#  SDL2_ttf_LIBRARIES - The libraries needed to use SDL2_ttf

include(FindSDL2)

if (SDL2_FOUND)
  # if (APPLE)
  #   set (SDL2_ttf_INCLUDE_DIR "/Library/Frameworks/SDL2_ttf.framework/Headers/" ${SDL2_INCLUDE_DIRS})
  #   set (SDL2_ttf_LIBRARIES "-framework SDL2_ttf" ${SDL2_LIBRARIES})

  #   include (FindPackageHandleStandardArgs)

  #   find_package_handle_standard_args (SDL2_ttf DEFAULT_MSG SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIR)
  #   mark_as_advanced (SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIR)
  include(FindPkgConfig)
  pkg_search_module (SDL2_ttf sdl2_ttf SDL2_ttf)

  include(FindPackageHandleStandardArgs)

  find_package_handle_standard_args (SDL2_ttf DEFAULT_MSG SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIRS)
  mark_as_advanced (SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIRS)
endif ()