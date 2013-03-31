
#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <curl-asio/native.h>

namespace curl
{
	class string_list:
		public boost::enable_shared_from_this<string_list>,
		public boost::noncopyable
	{
	public:
		string_list();
		~string_list();

		inline native::curl_slist* native_handle() { return list_; }

		void add(const char* str);
		void add(const std::string& str);
		
	private:
		native::curl_slist* list_;
	};
}
