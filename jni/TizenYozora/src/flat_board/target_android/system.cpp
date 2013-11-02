
#include "../target_dep.h"

namespace
{
	unsigned long s_tick = 0;
}

unsigned long target::system::getTicks(void)
{
	//??
	return ++s_tick;
}
