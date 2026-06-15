#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Audio::Audio" for configuration "Release"
set_property(TARGET Audio::Audio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Audio::Audio PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/Release/libAudio.a"
  )

list(APPEND _cmake_import_check_targets Audio::Audio )
list(APPEND _cmake_import_check_files_for_Audio::Audio "${_IMPORT_PREFIX}/lib/Release/libAudio.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
