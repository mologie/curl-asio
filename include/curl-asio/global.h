/*
	curl-asio: wrapper for integrating libcurl with boost.asio applications
	Copyright (c) 2013 Oliver Kuckertz <oliver.kuckertz@mologie.de>
	See COPYING for license information.

	Initialize libcurl when loading this library
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
