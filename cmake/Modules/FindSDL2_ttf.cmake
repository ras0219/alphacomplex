# - Try to find SDL2_ttf
# Once done this will define
#  SDL2_ttf_FOUND - System has SDL2_ttf
#  SDL2_ttf_INCLUDE_DIRS - The SDL2_ttf include directories
#  SDL2_ttf_LIBRARIES - The libraries needed to use SDL2_ttf

if (APPLE)
  set (SDL2_ttf_INCLUDE_DIR "/Library/Frameworks/SDL2_ttf.framework/Headers/")
  set (SDL2_ttf_LIBRARIES "-framework SDL2_ttf")

  include (FindPackageHandleStandardArgs)

  find_package_handle_standard_args (SDL2_ttf DEFAULT_MSG SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIR)
  mark_as_advanced (SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIR)
elseif (UNIX)
  include(FindPkgConfig)
  pkg_search_module (SDL2_ttf sdl2_ttf SDL2_ttf)

  include(FindPackageHandleStandardArgs)

  find_package_handle_standard_args (SDL2_ttf DEFAULT_MSG SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIRS)
  mark_as_advanced (SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIRS)
endif ()
