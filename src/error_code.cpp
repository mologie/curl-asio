
#include <curl-asio/error_code.h>

using namespace curl;

class curl_easy_error_category : public boost::system::error_category
{
public:
	curl_easy_error_category() { }
	const char* name() const BOOST_NOEXCEPT;
	std::string message(int ev) const;
};

const char* curl_easy_error_category::name() const BOOST_NOEXCEPT
{
	return "curl-easy";
}

std::string curl_easy_error_category::message(int ev) const
{
	return native::curl_easy_strerror(static_cast<native::CURLcode>(ev));
}

class curl_multi_error_category : public boost::system::error_category
{
public:
	curl_multi_error_category() { }
	const char* name() const BOOST_NOEXCEPT;
	std::string message(int ev) const;
};

const char* curl_multi_error_category::name() const BOOST_NOEXCEPT
{
	return "curl-multi";
}

std::string curl_multi_error_category::message(int ev) const
{
	// error descriptions have been taken from libcurl's documentation

	switch (static_cast<errc::multi::multi_error_codes>(ev))
	{
	case errc::multi::success:
		return "Things are fine.";

	case errc::multi::bad_handle:
		return "The passed-in handle is not a valid CURLM handle.";

	case errc::multi::bad_easy_handle:
		return "An easy handle was not good/valid. It could mean that it isn't an easy handle at all, or possibly that the handle already is in used by this or another multi handle.";

	case errc::multi::out_of_memory:
		return "Out of memory (you are doomed).";

	case errc::multi::intenral_error:
		return "This can only be returned if libcurl bugs. Please report it to us!";

	case errc::multi::bad_socket:
		return "The passed-in socket is not a valid one that libcurl already knows about.";

	case errc::multi::unknown_option:
		return "curl_multi_setopt() with unsupported option";

	default:
		return "no error description (unknown CURLMcode)";
	}
}

class curl_share_error_category : public boost::system::error_category
{
public:
	curl_share_error_category() { }
	const char* name() const BOOST_NOEXCEPT;
	std::string message(int ev) const;
};

const char* curl_share_error_category::name() const BOOST_NOEXCEPT
{
	return "curl-share";
}

std::string curl_share_error_category::message(int ev) const
{
	// error descriptions have been taken from libcurl's documentation

	switch (static_cast<errc::share::share_error_codes>(ev))
	{
	case errc::share::success:
		return "All fine. Proceed as usual.";

	case errc::share::bad_option:
		return "An invalid option was passed to the function.";

	case errc::share::in_use:
		return "The share object is currently in use.";

	case errc::share::invalid:
		return "An invalid share object was passed to the function.";

	case errc::share::nomem:
		return "Not enough memory was available.";

	default:
		return "no error description (unknown CURLSHcode)";
	}
}

class curl_form_error_category : public boost::system::error_category
{
public:
	curl_form_error_category() { }
	const char* name() const BOOST_NOEXCEPT;
	std::string message(int ev) const;
};

const char* curl_form_error_category::name() const BOOST_NOEXCEPT
{
	return "curl-form";
}

std::string curl_form_error_category::message(int ev) const
{
	switch (static_cast<errc::form::form_error_codes>(ev))
	{
	case errc::form::success:
		return "no error";

	case errc::form::memory:
		return "memory allocation error";

	case errc::form::option_twice:
		return "one option is given twice";

	case errc::form::null:
		return "a null pointer was given for a char";

	case errc::form::unknown_option:
		return "an unknown option was used";

	case errc::form::incomplete:
		return "some FormInfo is not complete (or error)";

	case errc::form::illegal_array:
		return "an illegal option is used in an array";

	case errc::form::disabled:
		return "form support was disabled";

	default:
		return "no error description (unknown CURLFORMcode)";
	}
}

const boost::system::error_category& errc::get_easy_category() BOOST_NOEXCEPT
{
	static const curl_easy_error_category curl_easy_category_const;
	return curl_easy_category_const;
}

const boost::system::error_category& errc::get_multi_category() BOOST_NOEXCEPT
{
	static const curl_multi_error_category curl_multi_category_const;
	return curl_multi_category_const;
}

const boost::system::error_category& errc::get_share_category() BOOST_NOEXCEPT
{
	static const curl_share_error_category curl_share_category_const;
	return curl_share_category_const;
}

const boost::system::error_category& errc::get_form_category() BOOST_NOEXCEPT
{
	static const curl_form_error_category curl_form_category_const;
	return curl_form_category_const;
}
