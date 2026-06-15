# CMake generated Testfile for 
# Source directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/source/lint
# Build directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/source/lint
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[spirv-tools-symbol-exports-SPIRV-Tools-lint]=] "/usr/bin/python3.14" "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/utils/check_symbol_exports.py" "/home/stasd/Work/GitHub-Projects/Cozy-Game/build/NATIVE/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/source/lint/libSPIRV-Tools-lint.a")
set_tests_properties([=[spirv-tools-symbol-exports-SPIRV-Tools-lint]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/CMakeLists.txt;249;add_test;/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/source/lint/CMakeLists.txt;46;spvtools_check_symbol_exports;/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/source/lint/CMakeLists.txt;0;")
