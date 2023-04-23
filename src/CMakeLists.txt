cmake_minimum_required(VERSION 3.16)

set(EXEC ${TARGET})
project(${EXEC})

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_executable(${EXEC} ${SRC} ${HEADERS})

target_link_libraries(${EXEC} glfw3)