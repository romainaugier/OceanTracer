# Global

cmake_minimum_required(VERSION 3.15)

# Vcpkg
if(DEFINED CMAKE_TOOLCHAIN_FILE)
	if(EXISTS "${CMAKE_TOOLCHAIN_FILE}")
		message(STATUS "Vcpkg has been found")
	else()
		message(FATAL_ERROR "Vcpkg can't be found, CMake will exit")
	endif()
else()
	if(EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
		message(STATUS "Vcpkg has been found")	
		set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "CMAKE Toolchain" FORCE)
	else()
		message(FATAL_ERROR "Vcpkg can't be found, CMake will exit")
	endif()
endif()

# Vcpkg triplet
if(WIN32)
	if("${CMAKE_SIZEOF_VOID_P}" EQUAL 4)
		message(FATAL_ERROR "VCPKG : windows:x86 triplet is not supported, CMake will exit")
	elseif("${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
		set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "VCPKG Triplet" FORCE)
	endif()
elseif(UNIX)
	if("${CMAKE_SIZEOF_VOID_P}" EQUAL 4)
		message(FATAL_ERROR "VCPKG : linux:x86 triplet is not supported, CMake will exit.")
	elseif("${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
		set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "VCPKG Triplet" FORCE)
	endif()
endif()

# Project
project(OceanTracer)

# CPP Standard
set(CMAKE_CXX_STANDARD 17)

# Link with pthreads on linux (required for tbb)
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(MSVC_COMPILER_FLAGS "/fp:fast /Ox /Os /Oy /GT /GL /Oi /arch:AVX2 /Zi /Gm- /Zc:inline /Qpar")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "${MSVC_COMPILER_FLAGS}")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options(-fopenmp -O3 -mavx2 -mf16c -fpermissive -mfma)
	add_link_options(-lstdc++fs)
else()
	message(FATAL_ERROR "Your compiler is not supported yet, CMake will exit.")
endif()

# TBB
find_package(TBB CONFIG REQUIRED)

# Glew
find_package(glew REQUIRED)

# ImGui
set(IMGUI_INCLUDES 
    dependencies/imgui
    dependencies/glfw/include
    )
include_directories(${IMGUI_INCLUDES})

add_subdirectory(dependencies)

# Src
include_directories(src)
add_subdirectory(src)