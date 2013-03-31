
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
