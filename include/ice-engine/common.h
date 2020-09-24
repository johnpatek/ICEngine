#ifndef COMMON_H
#define COMMON_H
#ifdef _WIN32
#include <WinSock2.h>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/ioctl.h>
#endif
#include <cstdint>
#include <string>

#ifdef _WIN32
typedef int ssize_t;
#pragma comment(lib, "WinMM.lib")
#endif

#endif