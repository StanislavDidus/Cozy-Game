# CMake generated Testfile for 
# Source directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools
# Build directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/build/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[spirv-tools-copyrights]=] "/home/stasd/Tools/emsdk/node/22.16.0_64bit/bin/node" "/usr/bin/python3.14" "utils/check_copyright.py")
set_tests_properties([=[spirv-tools-copyrights]=] PROPERTIES  WORKING_DIRECTORY "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools" _BACKTRACE_TRIPLES "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/CMakeLists.txt;355;add_test;/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/CMakeLists.txt;0;")
subdirs("external")
subdirs("source")
subdirs("tools")
subdirs("test")
subdirs("examples")
