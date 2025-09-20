#include "interface.h"

// 获取系统的当前时间，单位微秒(us)
int64_t GetSysTimeMicros()
{
#define EPOCHFILETIME   (116444736000000000UL)
	FILETIME ft;
	LARGE_INTEGER li;
	int64_t tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
	return tt;
}