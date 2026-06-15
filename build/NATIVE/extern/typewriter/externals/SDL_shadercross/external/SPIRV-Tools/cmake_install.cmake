# Install script for directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/external/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/source/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/tools/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/examples/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/spirv-tools" TYPE FILE FILES
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/include/spirv-tools/libspirv.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/include/spirv-tools/libspirv.hpp"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/include/spirv-tools/optimizer.hpp"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/include/spirv-tools/linker.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/pkgconfig" TYPE FILE FILES
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/SPIRV-Tools.pc"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/SPIRV-Tools-shared.pc"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
