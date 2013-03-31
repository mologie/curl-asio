
#pragma once

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <curl-asio/native.h>
#include <map>

namespace curl
{
	class easy;

	class multi:
		public boost::noncopyable
	{
	public:
		typedef boost::function<void(boost::system::error_code& err)> handler_type;
		typedef std::map<easy*, handler_type> easy_map_type;
		typedef boost::asio::ip::tcp::socket socket_type;

		multi(boost::asio::io_service& io_service);
		~multi();

		inline boost::asio::io_service& get_io_service() { return io_service_; }
		inline native::CURLM* native_handle() { return handle_; }

		void add(easy* easy_handle, handler_type handler);
		void remove(easy* easy_handle);

		typedef int (*socket_function_t)(native::CURL* native_easy, native::curl_socket_t s, int what, void* userp, void* socketp);
		void set_socket_function(socket_function_t socket_function);
		void set_socket_data(void* socket_data);

		typedef int (*timer_function_t)(native::CURLM* native_multi, long timeout_ms, void* userp);
		void set_timer_function(timer_function_t timer_function);
		void set_timer_data(void* timer_data);

	private:
		void monitor_socket(socket_type* sd, int action);
		void process_messages();
		bool still_running();
		handler_type get_handler(easy* easy_handle);

		void handle_socket_read(const boost::system::error_code& err, socket_type* sd);
		void handle_socket_write(const boost::system::error_code& err, socket_type* sd);
		void handle_timeout(const boost::system::error_code& err);

		static int socket(native::CURL* native_easy, native::curl_socket_t s, int what, void* userp, void* socketp);
		static int timer(native::CURLM* native_multi, long timeout_ms, void* userp);

		boost::asio::io_service& io_service_;
		native::CURLM* handle_;
		easy_map_type easy_handles_;
		boost::asio::deadline_timer timeout_;
		int still_running_;
	};
}
