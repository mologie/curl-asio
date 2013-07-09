curl-asio
=========

**Warning: Here be dragons. This library is still being developed and has not been extensively tested.**

This library makes use of libcurl's multi interface in order to enable easy integration into Boost.Asio applications.

* **simple interface** - Download and upload anything, synchronously or asynchronously, with just a few lines of code.
* **familiar** - If you have used libcurl in a C application before, you will feel right at home.
* **exceptions** - Libcurl errors throw exceptions. Integrates nicely with Boost.System's error_code class.
* **useful wrappers** - C++ interfaces for libcurl's easy, multi, form, share and string list containers. All setopt calls are wrapped for type safety.
* **source/sink concept** - Works nicely with Boost.Iostreams

Installation
------------
1. If not already done, install cURL and its header files
2. Clone this git repository. There are no tags or packages yet.
3. Run CMake and point it to cURL
4. `make && make install`

Examples
--------
Examples can be found in the wiki on the left and in the examples/ directory. Here is how a simple, blocking file download looks like:

```c++
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <curl-asio.h>
#include <fstream>

int main(int argc, char* argv[])
{
	// this example program downloads argv[1] to argv[2] (arg validation omitted for readability)
	char* url = argv[1];
	char* file_name = argv[2];
	
	// start by creating an io_service object
	boost::asio::io_service io_service;
	
	// construct an instance of curl::easy
	curl::easy downloader(io_service);
	
	// set the object's properties
	downloader.set_url(url);
	downloader.set_sink(boost::make_shared<std::ofstream>(file_name));
	
	// download the file
	boost::system::error_code ec;
	downloader.perform(ec);

	// error handling
	if (!ec)
	{
		std::cerr << "Download succeeded" << std::endl;
	}
	else
	{
		std::cerr << "Download failed: " << ec.message() << std::endl;
	}
	
	return 0;
}
```

License
-------
Curl-asio is licensed under the same MIT/X derivate license used by libcurl.
