
#include "../target_dep.h"

extern "C" void g_printLog(const char* sz_log);
extern "C" unsigned long g_getTicks(void);
extern "C" void g_updateScreen(void);

#include <stdio.h>

unsigned long target::system::getTicks(void)
{
	return g_getTicks();
}

void target::system::log(const char* sz_message)
{
	g_printLog(sz_message);
}

void target::updateScreen(void)
{
	g_updateScreen();
}

