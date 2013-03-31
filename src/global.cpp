
#include <boost/lexical_cast.hpp>
#include <curl-asio/native.h>
#include <curl-asio/global.h>

using namespace curl;

global_init_helper global_init_helper_instance;
static bool already_initialized = false;

global_init_helper::global_init_helper()
{
	if (already_initialized)
	{
		throw std::runtime_error("curl has already been initialized");
	}

	already_initialized = true;

	native::CURLcode ec = native::curl_global_init(CURL_GLOBAL_DEFAULT);

	if (ec != native::CURLE_OK)
	{
		throw std::runtime_error("curl_global_init failed with error code " + boost::lexical_cast<std::string>(ec));
	}
}

global_init_helper::~global_init_helper()
{
	if (already_initialized)
	{
		native::curl_global_cleanup();
		already_initialized = false;
	}
}
