# curl-asio
# Seamlessly integrate libcurl with Boost.Asio
#
# Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
# See COPYING for license information.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2 FATAL_ERROR)
PROJECT(curl-asio)

# Adjust the module path
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Build options
OPTION(BUILD_STATIC "Build the static library" ON)
OPTION(BUILD_SHARED "Build the shared library" OFF)
OPTION(BUILD_EXAMPLES "Build the examples" ON)

IF(NOT MSVC)
	OPTION(ENABLE_CPP11 "Enable features and examples which require C++11" ON)
	IF(ENABLE_CPP11)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
		FOREACH(_PROPERTY_NAME CMAKE_EXE_LINKER_FLAGS CMAKE_SHARED_LINKER_FLAGS CMAKE_MODULE_LINKER_FLAGS)
			SET(${_PROPERTY_NAME} "${${_PROPERTY_NAME}} -stdlib=libc++")	
		ENDFOREACH()
	ENDIF()
ENDIF()

IF(NOT BUILD_STATIC AND NOT BUILD_SHARED)
	MESSAGE(FATAL_ERROR "Neither BUILD_STATIC nor BUILD_SHARED is set")
ENDIF()

SET(CURLASIO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Prefer the shared interface over the static for building examples
IF(BUILD_SHARED)
	SET(EXAMPLE_LINK_TARGET curlasio-shared)
ELSE()
	SET(EXAMPLE_LINK_TARGET curlasio-static)
	SET(EXAMPLE_PROJECT_DEFINITIONS -DCURLASIO_STATIC)
ENDIF()

IF(MSVC)
	# Select Windows version
	OPTION(ENABLE_VISTA_FEATURES "Enable Vista features. This disables some workarounds, but sacrifices Windows XP compatibility." OFF)
	IF(ENABLE_VISTA_FEATURES)
		SET(TARGET_WINDOWS_VERSION 0x0600)
	ELSE()
		SET(TARGET_WINDOWS_VERSION 0x0501)
	ENDIF()
	ADD_DEFINITIONS(-D_WIN32_WINNT=${TARGET_WINDOWS_VERSION})
	
	# Disable using __declspec(dllimport) when linking statically
	OPTION(CURL_IS_STATIC "Check if libcurl is a static library" OFF)
	IF(CURL_IS_STATIC)
		ADD_DEFINITIONS(-DCURL_STATICLIB)
	ENDIF()
ENDIF()

# Find dependencies
FIND_PACKAGE(CURLASIO-CURL REQUIRED)
FIND_PACKAGE(Boost REQUIRED COMPONENTS chrono date_time system thread)

# Move along
ADD_SUBDIRECTORY(src)
IF(BUILD_EXAMPLES)
	ADD_SUBDIRECTORY(examples)
ENDIF()

# Install CMake modules for curl-asio and libcurl
OPTION(INSTALL_CMAKE_MODULES "Install CMake module files for libcurl and curl-asio" ON)
IF(INSTALL_CMAKE_MODULES)
	IF(APPLE)
		IF((IS_SYMLINK "${CMAKE_INSTALL_PREFIX}/share/cmake") OR (IS_SYMLINK "${CMAKE_INSTALL_PREFIX}/share/cmake/Modules"))
			MESSAGE(FATAL_ERROR "The destination path `${CMAKE_INSTALL_PREFIX}/share/cmake' is a symlink. "
				"If you are using Homebrew, run the following set of commands prior to installing this library:\n"
				"    brew unlink cmake\n"
				"    mkdir -p '${CMAKE_INSTALL_PREFIX}/share/cmake/Modules'\n"
				"    brew link cmake\n"
				"Without these changes, updating CMake through Homebrew would break the installation. Alternatively, choose a different CMAKE_INSTALL_PREFIX.")
		ENDIF()
	ENDIF()
	INSTALL(DIRECTORY cmake/ DESTINATION share/cmake/Modules)
ENDIF()
