#include "timer.hpp"
#include <Windows.h>

c_timer::c_timer()
{
	this->time = GetTickCount64();
}

void c_timer::reset()
{
	this->time = GetTickCount64();
}

aids::qword c_timer::get_time()
{
	return this->time;
}

bool c_timer::has_passed(long ms)
{
	return (GetTickCount64() - this->get_time() >= ms);
}
