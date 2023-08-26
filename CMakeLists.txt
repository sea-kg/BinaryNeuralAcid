cmake_minimum_required(VERSION 3.14)

project(bna C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${bna_SOURCE_DIR})

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/core")

list (APPEND WSJCPP_SOURCES "src/binary_neural_acid.cpp")

list (APPEND WSJCPP_SOURCES "src/main.cpp")


# list (APPEND WSJCPP_SOURCES "src/render/app_state.cpp")
# list (APPEND WSJCPP_SOURCES "src/render/render.cpp")
# list (APPEND WSJCPP_SOURCES "src/render/render_window.cpp")

#### BEGIN_WSJCPP_APPEND
list (APPEND WSJCPP_SOURCES "./src/argument_processor_main.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processor_main.cpp")
#### END_WSJCPP_APPEND


list (APPEND WSJCPP_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

include_directories(${WSJCPP_INCLUDE_DIRS})

# examples
if (UNIX)
    add_subdirectory(examples/sdl_sin)
endif (UNIX)
add_subdirectory(examples/sin)
add_subdirectory(examples/sum)

# enable testing functionality
enable_testing()
add_subdirectory(tests)

add_executable (bna ${WSJCPP_SOURCES})

target_link_libraries(bna ${WSJCPP_LIBRARIES})

install(
    TARGETS
        bna
    RUNTIME DESTINATION
        /usr/bin
)
