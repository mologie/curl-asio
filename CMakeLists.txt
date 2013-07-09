# curl-asio
# Seamlessly integrate libcurl with Boost.Asio
#
# Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
# See COPYING for license information.

cmake_minimum_required(VERSION 2.6.2 FATAL_ERROR)
project(curl-asio)

option(CURLASIO_STATICLIB "Set to ON to build libcurlasio with static linking. Only recommended when also linking Boost dynamically." ON)
if(CURLASIO_STATICLIB)
	add_definitions(-DCURLASIO_STATIC)
endif()

add_definitions(-D_WIN32_WINNT=0x0501)

find_package(CURL REQUIRED)
find_package(Boost REQUIRED COMPONENTS chrono date_time regex system thread)

add_subdirectory(src)
add_subdirectory(examples)
