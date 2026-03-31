include(FetchContent)

cmake_minimum_required(VERSION 3.16)
project(sfml-snake LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 20)

# Due to bug in sfml's build system. It doesn't affect anything much so leaving it off
set(FLAGS "-Wno-unused-command-line-argument") 

set(CMAKE_C_FLAGS ${FLAGS})
set(CMAKE_CXX_FLAGS ${FLAGS})

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 11b73743c42cf7ecd7c596ba83fdbf1150ffa96c
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(gamemenu
    GIT_REPOSITORY https://github.com/ParadoxZero/GameMenu-cpp.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(gamemenu)

set(GAME_CORE_FILES
    "src/ui/MainMenu.cpp"
    "src/ui/MainMenu.h"
    "src/core/engine.h"
    "src/core/food.h"
    "src/core/food.cpp"
    "src/core/snake.h"
    "src/core/snake.cpp"
    "src/core/game.h"
    "src/core/game.cpp"
    "src/ui/TipWindow.cpp"
    "src/ui/TipWindow.h"
    "src/core/game_ai_action.cpp"
    "src/core/game_ai_action.h"
)

# trainer doesn't need MainMenu (avoids game_menu.h double-include issue)
set(TRAIN_CORE_FILES
    "src/core/engine.h"
    "src/core/food.h"
    "src/core/food.cpp"
    "src/core/snake.h"
    "src/core/snake.cpp"
    "src/core/game.h"
    "src/core/game.cpp"
    "src/ui/TipWindow.cpp"
    "src/ui/TipWindow.h"
    "src/core/game_ai_action.cpp"
    "src/core/game_ai_action.h"
)

# ── Main game ──────────────────────────────────────────────────────────────────
if(WIN32)
    add_executable(snake WIN32 "src/main.cpp" ${GAME_CORE_FILES})
else()
    add_executable(snake "src/main.cpp" ${GAME_CORE_FILES})
endif()

target_link_libraries(snake PRIVATE sfml-graphics gamemenu)
target_compile_features(snake PRIVATE cxx_std_17)
target_include_directories(snake PRIVATE src)

# ── AI trainer (headless training loop) ───────────────────────────────────────
add_executable(snake_train "src/ai/train_main.cpp" ${TRAIN_CORE_FILES})
target_link_libraries(snake_train PRIVATE sfml-graphics gamemenu)
target_compile_features(snake_train PRIVATE cxx_std_17)
target_include_directories(snake_train PRIVATE src)

add_custom_target(copy_menu_font
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/sansation.ttf ${CMAKE_BINARY_DIR}
)
add_dependencies(snake copy_menu_font)
add_dependencies(snake_train copy_menu_font)

if(WIN32)
    add_custom_command(
        TARGET snake
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS snake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)