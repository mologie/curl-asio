# curl-asio
# Seamlessly integrate libcurl with Boost.Asio
#
# Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
# See COPYING for license information.

set(LIBCURLASIO_NAME libcurlasio)

if(CURLASIO_STATICLIB)
	set(CURLASIO_LIBTYPE STATIC)
else()
	set(CURLASIO_LIBTYPE SHARED)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(LIBCURLASIO_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIBCURLASIO_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")

file(GLOB LIBCURLASIO_SOURCE_FILES "${LIBCURLASIO_SOURCE_DIR}/*.cpp")
file(GLOB LIBCURLASIO_HEADER_FILES "${LIBCURLASIO_HEADER_DIR}/curl-asio/*.h")
LIST(APPEND LIBCURLASIO_HEADER_FILES "${LIBCURLASIO_HEADER_DIR}/curl-asio.h")

add_library(
	${LIBCURLASIO_NAME}
	${CURLASIO_LIBTYPE}
	${LIBCURLASIO_SOURCE_FILES}
	${LIBCURLASIO_HEADER_FILES}
	)

if(NOT CURLASIO_STATICLIB)
	target_link_libraries(
		${LIBCURLASIO_NAME}
		${Boost_LIBRARIES}
		${CURL_LIBRARIES}
		)
	if(WIN32)
		# Copied from cURL's cmake file
		set_target_properties(${LIBCURLASIO_NAME} PROPERTIES IMPORT_SUFFIX "_imp.lib")
	endif()
endif()

# Install shared or static runtime to $PREFIX/lib (and $PREFIX/bin on Windows)
install(TARGETS ${LIBCURLASIO_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

# Install header files to $PREFIX/include
install(DIRECTORY "${LIBCURLASIO_HEADER_DIR}/" DESTINATION include)
