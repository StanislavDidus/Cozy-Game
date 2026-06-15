# CMake generated Testfile for 
# Source directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools
# Build directory: /home/stasd/Work/GitHub-Projects/Cozy-Game/build/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[spirv-tools_expect_unittests]=] "/home/stasd/Tools/emsdk/node/22.16.0_64bit/bin/node" "/usr/bin/python3.14" "-m" "unittest" "expect_unittest.py")
set_tests_properties([=[spirv-tools_expect_unittests]=] PROPERTIES  WORKING_DIRECTORY "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools" _BACKTRACE_TRIPLES "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools/CMakeLists.txt;15;add_test;/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools/CMakeLists.txt;0;")
add_test([=[spirv-tools_spirv_test_framework_unittests]=] "/home/stasd/Tools/emsdk/node/22.16.0_64bit/bin/node" "/usr/bin/python3.14" "-m" "unittest" "spirv_test_framework_unittest.py")
set_tests_properties([=[spirv-tools_spirv_test_framework_unittests]=] PROPERTIES  WORKING_DIRECTORY "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools" _BACKTRACE_TRIPLES "/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools/CMakeLists.txt;18;add_test;/home/stasd/Work/GitHub-Projects/Cozy-Game/extern/typewriter/externals/SDL_shadercross/external/SPIRV-Tools/test/tools/CMakeLists.txt;0;")
subdirs("opt")
subdirs("objdump")
