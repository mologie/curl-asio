# curl-asio
# Seamlessly integrate libcurl with Boost.Asio
#
# Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
# See COPYING for license information.

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})

macro(add_example EXAMPLE_NAME)
	set(EXAMPLE_PROJECT_NAME "${EXAMPLE_NAME}_example")
	set(EXAMPLE_SOURCE_FILES "${EXAMPLE_NAME}.cpp")
	add_executable(${EXAMPLE_PROJECT_NAME} ${EXAMPLE_SOURCE_FILES})
	link_directories(${Boost_LIBRARY_DIR})
	target_link_libraries(${EXAMPLE_PROJECT_NAME} libcurlasio ${Boost_LIBRARIES})
	if(CURLASIO_STATICLIB)
		target_link_libraries(${EXAMPLE_PROJECT_NAME} ${CURL_LIBRARIES})
	endif()
endmacro()

add_example("asynchronous")
add_example("synchronous")
