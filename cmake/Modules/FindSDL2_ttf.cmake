set (SDL2_ttf_INCLUDE_DIR "/Library/Frameworks/SDL2_ttf.framework/Headers/")
set (SDL2_ttf_LIBRARIES "-framework SDL2_ttf")

include (FindPackageHandleStandardArgs)

find_package_handle_standard_args (SDL2_ttf DEFAULT_MSG SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIR)
mark_as_advanced (SDL2_ttf_LIBRARIES SDL2_ttf_INCLUDE_DIR)
