cmake_minimum_required(VERSION 2.8)

project(imagesquasher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g -Wall -Wextra -Wno-unused-parameter")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2IMAGE_INCLUDE_DIR})

add_executable(imagesquasher main.cpp display.cpp mutator.cpp)
target_link_libraries(imagesquasher ${SDL2_LIBRARY})
target_link_libraries(imagesquasher ${SDL2IMAGE_LIBRARY})
