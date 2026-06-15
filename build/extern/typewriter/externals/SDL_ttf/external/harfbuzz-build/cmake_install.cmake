# Install script for directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/stasd/Tools/emsdk/upstream/emscripten/cache/sysroot")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/harfbuzz" TYPE FILE FILES
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-aat-layout.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-aat.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-blob.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-buffer.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-common.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-cplusplus.hh"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-deprecated.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-draw.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-face.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-font.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-map.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-color.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-deprecated.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-font.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-layout.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-math.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-meta.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-metrics.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-name.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-shape.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot-var.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ot.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-paint.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-set.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-shape-plan.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-shape.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-style.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-unicode.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-version.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb.h"
    "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_ttf/external/harfbuzz/src/hb-ft.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/stasd/Work/GitHub-Projects/Cozy-Game/build/extern/typewriter/externals/SDL_ttf/external/harfbuzz-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
