cmake_minimum_required(VERSION 3.16)

set(EXEC ${TARGET})
project(${EXEC})

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_executable(${EXEC} ${SRC} ${HEADERS})

add_library(GLAD ${LIB_GLAD_PATH})

if(UNIX)
	set(LIBS pthread GL dl)
else()
	set(LIBS opengl32)
endif()

target_link_libraries(${EXEC} PUBLIC glfw3 GLAD ${LIBS})