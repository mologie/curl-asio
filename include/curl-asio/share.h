
#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <curl-asio/native.h>

namespace curl
{
	class share:
		public boost::enable_shared_from_this<share>,
		public boost::noncopyable
	{
	public:
		share();
		~share();

		inline native::CURLSH* native_handle() { return handle_; }
		void set_share_cookies(bool enabled);
		void set_share_dns(bool enabled);
		void set_share_ssl_session(bool enabled);

		typedef void (*lock_function_t)(native::CURL* handle, native::curl_lock_data data, native::curl_lock_access access, void* userptr);
		void set_lock_function(lock_function_t lock_function);

		typedef void (*unlock_function_t)(native::CURL* handle, native::curl_lock_data data, void* userptr);
		void set_unlock_function(unlock_function_t unlock_function);

		void set_user_data(void* user_data);

	private:
		static void lock(native::CURL* handle, native::curl_lock_data data, native::curl_lock_access access, void* userptr);
		static void unlock(native::CURL* handle, native::curl_lock_data data, void* userptr);

		native::CURLSH* handle_;
		boost::mutex mutex_;
	};
}
