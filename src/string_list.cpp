
#include <curl-asio/string_list.h>

using namespace curl;

string_list::string_list():
	list_(0)
{
}

string_list::~string_list()
{
	if (list_)
	{
		native::curl_slist_free_all(list_);
		list_ = 0;
	}
}

void string_list::add(const char* str)
{
	native::curl_slist* p = native::curl_slist_append(list_, str);

	if (!p)
	{
		throw std::bad_alloc();
	}

	list_ = p;
}

void string_list::add(const std::string& str)
{
	add(str.c_str());
}
