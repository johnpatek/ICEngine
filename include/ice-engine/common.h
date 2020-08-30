#ifndef COMMON_H
#define COMMON_H
#ifndef _WIN32
#include <sys/ioctl.h>
#endif
#include <cstdint>

#ifdef _WIN32
#pragma comment(lib, "WinMM.lib")
#endif

#endif