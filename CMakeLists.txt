cmake_minimum_required(VERSION 3.12) 

set(PROJECT_NAME cengine)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(${PROJECT_NAME})
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-g")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB SRC_FILES "src/*.c" "lib/mathc/mathc.c")
file(COPY "shaders" DESTINATION "./")
file(COPY "res" DESTINATION "./")

include_directories("include/")
include_directories("lib/stb/")
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories("lib/darray/")
include_directories("lib/cgltf/")
#include_directories("lib/mathc/")

add_executable(${PROJECT_NAME} ${SRC_FILES})

add_subdirectory(lib/cglm/ EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} m)
target_link_libraries(${PROJECT_NAME} cglm_headers)
