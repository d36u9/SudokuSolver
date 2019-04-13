#####################################################################
##          Copyright Yibo Zhu 2019
## Distributed under the MIT License
##    (See https://opensource.org/licenses/MIT)
#####################################################################

cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

project(Sudoku VERSION 0.0.1 LANGUAGES CXX)


set(BUILD_TESTS ON CACHE BOOL "Build tests.")
#set(BUILD_BECHMARK ON CACHE BOOL "Build benchmark.")
#C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#set up paths
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

set(LIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/sudoku")

include_directories(sudoku)


add_library(libsudoku INTERFACE)

target_include_directories(libsudoku INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sudoku>
    $<INSTALL_INTERFACE:${LIBRARY_OUTPUT_PATH}/include/sudoku>)

set(LibSudokuHeader ${PROJECT_SOURCE_DIR}/sudoku/SudokuSolver.h ${PROJECT_SOURCE_DIR}/sudoku/SimpleSudoku.h)


install(FILES "sudoku/*.h"
    DESTINATION "${LIBRARY_OUTPUT_PATH}/include/sudoku")

add_executable(SampleTest ${PROJECT_SOURCE_DIR}/test/SampleTest.cpp 
    ${LibSudokuHeader})

add_executable(benchmark ${PROJECT_SOURCE_DIR}/test/benchmark.cpp 
    ${LibSudokuHeader})