cmake_minimum_required(VERSION 3.0)
set(PROJECT iso8583lib)
project(${PROJECT} CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra -O3")

### variables ###
set(ISOLIB_INCLUDES ${PROJECT_SOURCE_DIR}/includes)

### includes ###
include_directories(${ISOLIB_INCLUDES})

### sources ###
set(SRC_DIR "src")

### outputs ###
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

file(GLOB SOURCE_FILES "${SRC_DIR}/*.cpp")
file(GLOB HEADER_FILES "${ISOLIB_INCLUDES}/*.hpp")

set(SOURCES ${SOURCE_FILES} ${HEADER_FILES})

add_library(${PROJECT} ${SOURCES})

