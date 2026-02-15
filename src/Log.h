#pragma once
#include <stdio.h>

#ifndef __FUNCTION__
#define __FUNCTION__ "<unknown>"
#endif

#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif

#if defined CMAKE_BUILD_TYPE == Debug
#define LOG_FILE stdout
#define DBG_LOG(fmt, ...) _DBG_LOG("[%s@%s:%d] " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define _DBG_LOG(fmt, ...) fprintf(LOG_FILE, fmt, __VA_ARGS__); fflush(LOG_FILE)
#else
#define DBG_LOG(fmt, ...)
#endif