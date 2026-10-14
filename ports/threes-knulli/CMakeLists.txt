cmake_minimum_required(VERSION 3.24)
project(threes_knulli VERSION 0.4.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# KNULLI supplies a patched SDL2 driver for H700. Use that shared library.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(_system_sdl_default ON)
else()
    set(_system_sdl_default OFF)
endif()
option(THREES_SYSTEM_SDL2 "Use the target system's shared SDL2" ${_system_sdl_default})
if(THREES_SYSTEM_SDL2)
    find_package(SDL2 2.0.14 REQUIRED CONFIG)
else()
    include(FetchContent)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SDL2
        URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.12/SDL2-2.30.12.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(SDL2)
endif()

include(cmake/EmbedResource.cmake)
embed_resource(font_inter_bold assets/fonts/Inter-Bold.ttf)
embed_resource(font_jua_regular assets/fonts/Jua-Regular.ttf)
add_executable(triples WIN32
    src/main.cpp src/app.cpp
    src/game/board.cpp src/game/deck.cpp src/game/move.cpp src/game/rng.cpp
    src/game/score.cpp src/game/game_state.cpp src/game/serialize.cpp
    src/render/renderer.cpp src/render/tile_draw.cpp src/render/text.cpp
    src/render/animation.cpp src/render/effects.cpp src/render/skin.cpp
    src/input/input.cpp src/input/drag.cpp
    src/audio/synth.cpp src/audio/mixer_sdl2.cpp src/audio/sfx.cpp
    ${EMBEDDED_RESOURCE_SOURCES})
target_include_directories(triples PRIVATE src third_party)
target_compile_definitions(triples PRIVATE SDL_MAIN_HANDLED)
if(THREES_SYSTEM_SDL2)
    target_link_libraries(triples PRIVATE SDL2::SDL2)
else()
    target_link_libraries(triples PRIVATE SDL2::SDL2-static)
endif()
if(WIN32)
    target_sources(triples PRIVATE src/platform/persistence_win.cpp)
    target_link_libraries(triples PRIVATE shell32)
else()
    target_sources(triples PRIVATE src/platform/persistence_posix.cpp)
endif()
if(MINGW)
    target_link_options(triples PRIVATE -static -static-libgcc -static-libstdc++)
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/assets/skin")
    add_custom_command(TARGET triples POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/skin $<TARGET_FILE_DIR:triples>/assets/skin)
endif()
include(CTest)
if(BUILD_TESTING AND NOT CMAKE_CROSSCOMPILING)
    add_subdirectory(tests)
endif()
install(TARGETS triples RUNTIME DESTINATION .)
if(EXISTS "${CMAKE_SOURCE_DIR}/assets/skin")
    install(DIRECTORY assets/skin DESTINATION assets)
endif()
