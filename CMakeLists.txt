cmake_minimum_required(VERSION 3.25)

project(algorithm-visualization C CXX)

add_compile_options(-Wall -Wshadow -Wextra -Wconversion -Wpedantic -Werror)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(
            algo
            src/main.cpp
            src/grid.cpp
            src/utils.cpp
            src/algorithms.cpp)

                    
set_target_properties(algo PROPERTIES CXX_STANDARD 17)

add_library(
       algo_lib
       src/grid.hpp
       src/utils.hpp
       src/algorithms.hpp)

set_target_properties(algo_lib PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(algo ${SDL2_LIBRARIES})
target_link_libraries(algo algo_lib)




