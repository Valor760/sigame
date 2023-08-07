cmake_minimum_required(VERSION 3.16)

set(TARGET sigame)
project(${TARGET})

# Global vars
set(OUTPUT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/$<1:lib>) # $<1:> to remove Debug/Release subdirs
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(ASSETS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(LIB_GLAD_PATH ${EXTERNAL_DIR}/glad/gl.c)

# Defines
set(WINDOWS_DEFINES "SI_PLATFORM_WINDOWS")
set(LINUX_DEFINES   "SI_PLATFORM_LINUX")
set(COMMON_DEFINES  "")

# Set flags according to the OS
# TODO: Add optimization flags to release build
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /Zc:preprocessor")

	if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	endif()
elseif(UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++2a")

	if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	endif()
else()
	message(FATAL_ERROR "Current OS is not supported!")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
if(NOT DEFINED ${CMAKE_BUILD_TYPE})
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Output libs and pdbs to 'lib' dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})

# TODO: Apparently this is due to multi-project generator. Maybe need to be fixed in future
# https://discourse.cmake.org/t/copy-data-files-to-build-dir-and-avoid-debug-release-folder-when-using-msvc/2632
if(MSVC)
	# MSVC has weird way to add additional debug/release directory
	set(BINARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/$<1:bin>) # Change default binary location
else(UNIX AND NOT APPLE)
	set(BINARY_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR}) # Change default binary location
endif()

# GLFW settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Includes
include_directories(${EXTERNAL_DIR}/glfw/include)
include_directories(${EXTERNAL_DIR})
include_directories(${EXTERNAL_DIR}/imgui)
include_directories(${EXTERNAL_DIR}/imgui/backends)
include_directories(${EXTERNAL_DIR}/nlohmann)
include_directories(${EXTERNAL_DIR}/elzip/include)
include_directories(${EXTERNAL_DIR}/imgui-dialog)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# FIXME: For some reasons it fails to link on glfw first time
# https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source
link_directories(${OUTPUT_LIB_DIR})

# CMake modules
add_subdirectory(${EXTERNAL_DIR}/glfw)
add_subdirectory(${EXTERNAL_DIR}/elzip)
add_subdirectory(${EXTERNAL_DIR}/imgui-dialog)
add_subdirectory(src)