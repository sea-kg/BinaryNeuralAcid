cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME example_sum)

project(${PROJECT_NAME})
set(EXECUTABLE_OUTPUT_PATH ${${PROJECT_NAME}_SOURCE_DIR}/../../)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../cmake/")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 17)

list (APPEND EXAMPLE_SUM_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src")
list (APPEND EXAMPLE_SUM_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../../src")

add_executable(
    ${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/../../src/binary_neural_acid.cpp"
    "${WSJCPP_SOURCES}"
)


target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${EXAMPLE_SUM_INCLUDE_DIRS}"
)
