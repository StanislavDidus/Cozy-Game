cmake_minimum_required(VERSION 3.15...4.2.1)

set(GAME_NAME COZY_GAME)

project(${GAME_NAME} VERSION 1.0
        DESCRIPTION "Cozy game for a cozy game jam"
        LANGUAGES C CXX)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Don't build shared libs" FORCE)

add_subdirectory(extern)

set(SOURCE_FILES
        src/main.cpp
        src/GameLayer.cpp
        src/Systems/InputSystem.cpp
        src/Systems/CollisionSystem.cpp
        src/Systems/PhysicsSystem.cpp
        src/Systems/InteractionSystem.cpp
        src/FoodSpawner.cpp
        src/Systems/ObjectManager.cpp
)
set(HEADER_FILES
        inc/Components.hpp
        inc/GameLayer.hpp
        inc/Enums.hpp
        inc/Systems/InputSystem.hpp
        inc/Systems/CollisionSystem.hpp
        inc/Systems/PhysicsSystem.hpp
        inc/Systems/InteractionSystem.hpp
        inc/StateMachine/State.hpp
        inc/FoodSpawner.hpp
        inc/Layers.hpp
        inc/Systems/ObjectManager.hpp)

set(ALL_FILES ${SOURCE_FILES} ${HEADER_FILES})

add_executable(${GAME_NAME} ${ALL_FILES})

#Copy assets and data folders to the build folder
add_custom_target(copy_assets_game
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(${GAME_NAME} copy_assets_game)

target_include_directories(${GAME_NAME} PUBLIC inc)

target_link_libraries(
       ${GAME_NAME} PUBLIC
        typewriter::typewriter
)

setup_typewriter(${GAME_NAME} "${CMAKE_SOURCE_DIR}/extern/typewriter"})

target_compile_features(${GAME_NAME} PUBLIC cxx_std_23)
