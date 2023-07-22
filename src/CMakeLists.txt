cmake_minimum_required(VERSION 3.16)

set(EXEC ${TARGET})
project(${EXEC})

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_executable(${EXEC} ${SRC} ${HEADERS})

add_library(GLAD ${LIB_GLAD_PATH})
add_library(imgui
		${EXTERNAL_DIR}/imgui/imgui.cpp
		${EXTERNAL_DIR}/imgui/imgui_tables.cpp
		${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
		${EXTERNAL_DIR}/imgui/imgui_draw.cpp
		${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
		${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
		)

set(COMMON_LIBS glfw3 imgui GLAD elzip ImGuiFileDialog)
if(UNIX)
	set(OS_LIBS pthread GL dl)
else()
	set(OS_LIBS opengl32)
endif()

target_link_libraries(${EXEC} PUBLIC ${OS_LIBS} ${COMMON_LIBS})

# Copy assets to the binary directory
# file(COPY ${ASSETS_PATH} DESTINATION ${BINARY_OUTPUT_DIR})
add_custom_command(
		TARGET ${TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${BINARY_OUTPUT_DIR}/assets
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_PATH} ${BINARY_OUTPUT_DIR}/assets)