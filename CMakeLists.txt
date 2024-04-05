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
    GIT_TAG 88a755e00f2da8a2d600479085a4bc92b8d5d30f
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(gamemenu)

set(SOURCE_FILES 
"src/main.cpp"
"src/ui/MainMenu.cpp"
"src/ui/MainMenu.h"
"src/core/engine.h"
"src/core/food.h"
"src/core/food.cpp"
"src/core/snake.h"
"src/core/snake.cpp"
"src/core/game.h"
"src/core/game.cpp"
)

if(WIN32)
    add_executable(snake WIN32 ${SOURCE_FILES})
else()
    add_executable(snake ${SOURCE_FILES})
endif()

target_link_libraries(snake PRIVATE sfml-graphics gamemenu)
target_compile_features(snake PRIVATE cxx_std_17)
target_include_directories(snake PRIVATE src)

add_custom_target(copy_menu_font
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/sansation.ttf ${CMAKE_BINARY_DIR}
)
add_dependencies(snake copy_menu_font)

if(WIN32)
    add_custom_command(
        TARGET snake
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS snake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)