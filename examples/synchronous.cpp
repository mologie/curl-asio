#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <curl-asio.h>
#include <fstream>

int main(int argc, char* argv[])
{
	// expect two arguments
	if (argc != 3)
	{
		std::cerr << "usage: " << argv[0] << " url file" << std::endl;
		return 1;
	}

	// this example program downloads argv[1] to argv[2]
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
