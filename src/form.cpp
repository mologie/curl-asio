/**
	curl-asio: wrapper for integrating libcurl with boost.asio applications
	Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
	See COPYING for license information.

	C++ wrapper for constructing libcurl forms
*/

#include <boost/asio.hpp>
#include <curl-asio/error_code.h>
#include <curl-asio/form.h>

using namespace curl;

form::form():
	post_(NULL),
	last_(NULL)
{
}

form::~form()
{
	if (post_)
	{
		native::curl_formfree(post_);
		post_ = 0;
	}
}

void form::add_content(const std::string& name, const std::string& content)
{
	boost::system::error_code ec(native::curl_formadd(&post_, &last_,
		native::CURLFORM_COPYNAME, name.c_str(),
		native::CURLFORM_NAMELENGTH, name.length(),
		native::CURLFORM_COPYCONTENTS, content.c_str(),
		native::CURLFORM_CONTENTSLENGTH, content.length(),
		native::CURLFORM_END
		));
	boost::asio::detail::throw_error(ec);
}

void form::add_content(const std::string& name, const std::string& content, const std::string& content_type)
{
	boost::system::error_code ec(native::curl_formadd(&post_, &last_,
		native::CURLFORM_COPYNAME, name.c_str(),
		native::CURLFORM_NAMELENGTH, name.length(),
		native::CURLFORM_COPYCONTENTS, content.c_str(),
		native::CURLFORM_CONTENTSLENGTH, content.length(),
		native::CURLFORM_CONTENTTYPE, content_type.c_str(),
		native::CURLFORM_END
		));
	boost::asio::detail::throw_error(ec);
}

void form::add_file(const std::string& name, const std::string& file_path)
{
	boost::system::error_code ec(native::curl_formadd(&post_, &last_,
		native::CURLFORM_COPYNAME, name.c_str(),
		native::CURLFORM_NAMELENGTH, name.length(),
		native::CURLFORM_FILECONTENT, file_path.c_str(),
		native::CURLFORM_END
		));
	boost::asio::detail::throw_error(ec);
}

void form::add_file(const std::string& name, const std::string& file_path, const std::string& content_type)
{
	boost::system::error_code ec(native::curl_formadd(&post_, &last_,
		native::CURLFORM_COPYNAME, name.c_str(),
		native::CURLFORM_NAMELENGTH, name.length(),
		native::CURLFORM_FILE, file_path.c_str(),
		native::CURLFORM_CONTENTTYPE, content_type.c_str(),
		native::CURLFORM_END
		));
}

void form::add_file_content(const std::string& name, const std::string& file_path)
{
	boost::system::error_code ec(native::curl_formadd(&post_, &last_,
		native::CURLFORM_COPYNAME, name.c_str(),
		native::CURLFORM_NAMELENGTH, name.length(),
		native::CURLFORM_FILE, file_path.c_str(),
		native::CURLFORM_END
		));
	boost::asio::detail::throw_error(ec);
}

void form::add_file_content(const std::string& name, const std::string& file_path, const std::string& content_type)
{
	boost::system::error_code ec(native::curl_formadd(&post_, &last_,
		native::CURLFORM_COPYNAME, name.c_str(),
		native::CURLFORM_NAMELENGTH, name.length(),
		native::CURLFORM_FILECONTENT, file_path.c_str(),
		native::CURLFORM_CONTENTTYPE, content_type.c_str(),
		native::CURLFORM_END
		));
	boost::asio::detail::throw_error(ec);
}
