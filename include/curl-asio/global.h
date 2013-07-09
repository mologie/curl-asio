/**
	curl-asio: wrapper for integrating libcurl with boost.asio applications
	Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
	See COPYING for license information.

	Helper to automatically initialize and cleanup libcurl resources
*/

#pragma once

namespace curl
{
	class global_init_helper
	{
	public:
		global_init_helper();
		~global_init_helper();
	};

	extern global_init_helper global_init_helper_instance;
}
