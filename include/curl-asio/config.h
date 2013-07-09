
#pragma once

#if !defined CURLASIO_API
#	if defined _MSC_VER
#		if defined CURLASIO_STATIC
#			define CURLASIO_API
#		elif defined libcurlasio_EXPORTS
#			define CURLASIO_API __declspec(dllexport)
#		else
#			define CURLASIO_API __declspec(dllimport)
#		endif
#	else
#		define CURLASIO_API
#	endif
#endif
