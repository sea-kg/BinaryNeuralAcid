cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME example_sdl_sin)

project(${PROJECT_NAME})
set(EXECUTABLE_OUTPUT_PATH ${${PROJECT_NAME}_SOURCE_DIR}/../../)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../../cmake/")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 17)

list (APPEND SDL_SIN1_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src")
list (APPEND SDL_SIN1_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src/render/")
list (APPEND SDL_SIN1_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../../src")
list (APPEND SDL_SIN1_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../../src.wsjcpp/nlohmann_json/")

add_executable(
    ${PROJECT_NAME}
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/render/app_state.cpp"
    "${PROJECT_SOURCE_DIR}/src/render/render.cpp"
    "${PROJECT_SOURCE_DIR}/src/render/render_window.cpp"
    "${PROJECT_SOURCE_DIR}/src/bna_test_sin1.cpp"
    "${PROJECT_SOURCE_DIR}/../../src/binary_neural_acid.cpp"
    "${WSJCPP_SOURCES}"
)

############################
# SDL2
find_package(SDL2 REQUIRED)
if ( SDL2_FOUND )
    list (APPEND SDL_SIN1_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
    list (APPEND SDL_SIN1_LIBRARIES ${SDL2_LIBRARY})
endif( SDL2_FOUND )
############################

############################
# SDL2_ttf
find_package(SDL2TTF REQUIRED)
if ( SDL2_TTF_FOUND )
    list (APPEND SDL_SIN1_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIRS})
    list (APPEND SDL_SIN1_LIBRARIES ${SDL2_TTF_LIBRARIES})
endif( SDL2_TTF_FOUND )
############################

list (APPEND SDL_SIN1_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})


target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${SDL_SIN1_INCLUDE_DIRS}"
)

target_link_libraries(
    ${PROJECT_NAME}
    ${SDL_SIN1_LIBRARIES}
)
