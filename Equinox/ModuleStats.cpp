#include "ModuleStats.h"

bool ModuleStats::Init()
{
	_total_complex_time.Start();
	_total_simple_time.Start();

	return true;
}
