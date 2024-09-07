cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

pico_sdk_init()

project(loader C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICOSYSTEM_DIR ${CMAKE_CURRENT_LIST_DIR}/picosystem)
find_package(PICOSYSTEM REQUIRED)

picosystem_executable(
  loader
  main.cpp
)

target_link_libraries(
  loader
  pico_stdlib
)

disable_startup_logo(loader)
no_spritesheet(loader)
