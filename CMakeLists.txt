cmake_minimum_required(VERSION 3.0)

project(bna C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${bna_SOURCE_DIR})

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")

list (APPEND WSJCPP_SOURCES "src/main.cpp")
list (APPEND WSJCPP_SOURCES "src/bna_types.cpp")
list (APPEND WSJCPP_SOURCES "src/bna.cpp")

#### BEGIN_WSJCPP_APPEND
list (APPEND WSJCPP_SOURCES "./src/argument_processor_main.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processor_main.cpp")
#### END_WSJCPP_APPEND

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (bna ${WSJCPP_SOURCES})

target_link_libraries(bna ${WSJCPP_LIBRARIES})

install(
    TARGETS
        bna
    RUNTIME DESTINATION
        /usr/bin
)


