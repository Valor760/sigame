cmake_minimum_required(VERSION 3.16)

set(TARGET sigame)
project(${TARGET})

# Global vars
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(OUTPUT_LIB_DIR $<1:${LIB_DIR}>) # $<1:> to remove Debug/Release subdirs
set(CMAKE_BUILD_TYPE "")
set(BINARY_OUTPUT_DIR $<1:${CMAKE_CURRENT_SOURCE_DIR}/build/bin>)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

# Check for library type [STATIC | SHARED]
# if(NOT DEFINED LIB_TYPE)
# 	message("LIB_TYPE variable is not defined. Setting STATIC by default")
# 	# If library type is not defined, then set static by default
# 	set(LIB_TYPE STATIC)
# else()
# 	string(TOUPPER ${LIB_TYPE} LIB_TYPE) # Convert to upper case
# 	if(NOT (${LIB_TYPE} MATCHES STATIC OR ${LIB_TYPE} MATCHES SHARED) )
# 		message("\'${LIB_TYPE}\' is not currently supported")
# 	endif()
# endif()

# Set flags according to the OS
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /Zc:preprocessor")
elseif(UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++2a")
else()
	message(FATAL_ERROR "Current OS is not supported!")
endif()

# Change default binary location
# TODO: probably change it to different path?
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})

# Output libs and pdbs to 'lib' dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})


# GLFW settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(OpenGL REQUIRED)

# Includes
include_directories(${EXTERNAL_DIR}/glfw/include)
include_directories(${EXTERNAL_DIR}/imgui)
include_directories(${EXTERNAL_DIR}/imgui/backends)

# FIXME: For some reasons it fails to link on glfw first time
# https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source
link_directories(${LIB_DIR})

# CMake modules
add_subdirectory(${EXTERNAL_DIR}/glfw)
add_subdirectory(src)