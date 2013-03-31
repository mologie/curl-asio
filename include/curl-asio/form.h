
#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include "native.h"

namespace curl
{
	class form:
		public boost::enable_shared_from_this<form>,
		public boost::noncopyable
	{
	public:
		form();
		~form();

		inline native::curl_httppost* native_handle() { return post_; };

		void add_content(const std::string& name, const std::string& content);
		void add_content(const std::string& name, const std::string& content, const std::string& content_type);
		void add_file(const std::string& name, const std::string& file_path);
		void add_file(const std::string& name, const std::string& file_path, const std::string& content_type);
		void add_file_content(const std::string& name, const std::string& file_path);
		void add_file_content(const std::string& name, const std::string& file_path, const std::string& content_type);

	private:
		native::curl_httppost* post_;
		native::curl_httppost* last_;
	};
}
