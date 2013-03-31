
#include <boost/bind.hpp>
#include <curl-asio/easy.h>
#include <curl-asio/error_code.h>
#include <curl-asio/multi.h>

using namespace curl;

multi::multi(boost::asio::io_service& io_service):
	io_service_(io_service),
	timeout_(io_service),
	still_running_(0)
{
	handle_ = native::curl_multi_init();

	if (!handle_)
	{
		throw std::bad_alloc();
	}

	set_socket_function(&multi::socket);
	set_socket_data(this);

	set_timer_function(&multi::timer);
	set_timer_data(this);
}

multi::~multi()
{
	while (!easy_handles_.empty())
	{
		easy_map_type::iterator it = easy_handles_.begin();
		easy* easy_handle = it->first;
		handler_type handler = it->second;
		handler(boost::system::error_code(boost::asio::error::operation_aborted));
		remove(easy_handle);
	}

	if (handle_)
	{
		native::curl_multi_cleanup(handle_);
		handle_ = 0;
	}
}

void multi::add(easy* easy_handle, handler_type handler)
{
	easy_handles_.insert(easy_map_type::value_type(easy_handle, handler));
	native::curl_multi_add_handle(handle_, easy_handle->native_handle());
}

void multi::remove(easy* easy_handle)
{
	easy_map_type::iterator it = easy_handles_.find(easy_handle);

	if (it != easy_handles_.end())
	{
		easy_handles_.erase(it);
		native::curl_multi_remove_handle(handle_, easy_handle->native_handle());
	}
}

void multi::set_socket_function(socket_function_t socket_function)
{
	boost::system::error_code ec(native::curl_multi_setopt(handle_, native::CURLMOPT_SOCKETFUNCTION, socket_function));
	boost::asio::detail::throw_error(ec);
}

void multi::set_socket_data(void* socket_data)
{
	boost::system::error_code ec(native::curl_multi_setopt(handle_, native::CURLMOPT_SOCKETDATA, socket_data));
	boost::asio::detail::throw_error(ec);
}

void multi::set_timer_function(timer_function_t timer_function)
{
	boost::system::error_code ec(native::curl_multi_setopt(handle_, native::CURLMOPT_TIMERFUNCTION, timer_function));
	boost::asio::detail::throw_error(ec);
}

void multi::set_timer_data(void* timer_data)
{
	boost::system::error_code ec(native::curl_multi_setopt(handle_, native::CURLMOPT_TIMERDATA, timer_data));
	boost::asio::detail::throw_error(ec);
}

void multi::monitor_socket(socket_type* sd, int action)
{
	if (action & CURL_POLL_IN)
	{
		sd->async_read_some(boost::asio::null_buffers(), boost::bind(&multi::handle_socket_read, this, boost::asio::placeholders::error, sd));
	}

	if (action & CURL_POLL_OUT)
	{
		sd->async_write_some(boost::asio::null_buffers(), boost::bind(&multi::handle_socket_write, this, boost::asio::placeholders::error, sd));
	}
}

void multi::process_messages()
{
	native::CURLMsg* msg;
	int msgs_left;

	while ((msg = native::curl_multi_info_read(handle_, &msgs_left)))
	{
		if (msg->msg == native::CURLMSG_DONE)
		{
			easy* easy_handle = easy::from_native(msg->easy_handle);
			handler_type handler = get_handler(easy_handle);
			boost::system::error_code ec;

			if (msg->data.result != native::CURLE_OK)
			{
				ec = boost::system::error_code(msg->data.result);
			}

			handler(ec);
			remove(easy_handle);
		}
	}
}

bool multi::still_running()
{
	return (still_running_ > 0);
}

multi::handler_type multi::get_handler(easy* easy_handle)
{
	easy_map_type::iterator it = easy_handles_.find(easy_handle);

	if (it != easy_handles_.end())
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("easy handle not registered");
	}
}

void multi::handle_socket_read(const boost::system::error_code& err, socket_type* sd)
{
	if (!err)
	{
		boost::system::error_code ec(native::curl_multi_socket_action(handle_, sd->native_handle(), CURL_CSELECT_IN, &still_running_));
		boost::asio::detail::throw_error(ec);
		process_messages();

		if (still_running())
		{
			sd->async_write_some(boost::asio::null_buffers(), boost::bind(&multi::handle_socket_write, this, boost::asio::placeholders::error, sd));
		}
		else
		{
			timeout_.cancel();
		}
	}
	else if (err != boost::asio::error::operation_aborted)
	{
		boost::system::error_code ec(native::curl_multi_socket_action(handle_, sd->native_handle(), CURL_CSELECT_ERR, &still_running_));
		boost::asio::detail::throw_error(ec);
		process_messages();
	}
}

void multi::handle_socket_write(const boost::system::error_code& err, socket_type* sd)
{
	if (!err)
	{
		boost::system::error_code ec(native::curl_multi_socket_action(handle_, sd->native_handle(), CURL_CSELECT_OUT, &still_running_));
		boost::asio::detail::throw_error(ec);
		process_messages();

		if (still_running())
		{
			sd->async_read_some(boost::asio::null_buffers(), boost::bind(&multi::handle_socket_read, this, boost::asio::placeholders::error, sd));
		}
		else
		{
			timeout_.cancel();
		}
	}
	else if (err != boost::asio::error::operation_aborted)
	{
		boost::system::error_code ec(native::curl_multi_socket_action(handle_, sd->native_handle(), CURL_CSELECT_ERR, &still_running_));
		boost::asio::detail::throw_error(ec);
		process_messages();
	}
}

void multi::handle_timeout(const boost::system::error_code& err)
{
	if (!err)
	{
		boost::system::error_code ec(native::curl_multi_socket_action(handle_, CURL_SOCKET_TIMEOUT, 0, &still_running_));
		boost::asio::detail::throw_error(ec);
		process_messages();
	}
}

int multi::socket(native::CURL* native_easy, native::curl_socket_t s, int what, void* userp, void* socketp)
{
	multi* self = static_cast<multi*>(userp);

	if (what == CURL_POLL_REMOVE)
	{
		// stop listening for events
		socket_type* sd = static_cast<socket_type*>(socketp);
		//sd->cancel();
	}
	else
	{
		if (socketp)
		{
			// change direction
			socket_type* sd = static_cast<socket_type*>(socketp);
			//sd->cancel();
			self->monitor_socket(sd, what);
		}
		else if (native_easy)
		{
			// start listening for events
			easy* easy_handle = easy::from_native(native_easy);
			socket_type* sd = easy_handle->get_socket_from_native(s);

			if (sd)
			{
				self->monitor_socket(sd, what);
			}
		}
	}

	return 0;
}

int multi::timer(native::CURLM* native_multi, long timeout_ms, void* userp)
{
	multi* self = static_cast<multi*>(userp);

	if (timeout_ms > 0)
	{
		self->timeout_.expires_from_now(boost::posix_time::millisec(timeout_ms));
		self->timeout_.async_wait(boost::bind(&multi::handle_timeout, self, boost::asio::placeholders::error));
	}
	else
	{
		self->timeout_.cancel();
		self->handle_timeout(boost::system::error_code());
	}

	return 0;
}
