/**
	curl-asio: wrapper for integrating libcurl with boost.asio applications
	Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
	See COPYING for license information.

	Helper to automatically initialize and cleanup libcurl resources
*/

#pragma once

#include "config.h"
#include <boost/shared_ptr.hpp>

namespace curl
{
	class initialization
	{
	public:
		typedef boost::shared_ptr<initialization> ptr;
		static ptr ensure_initialization();
		~initialization();
	protected:
		initialization();
	};
}
