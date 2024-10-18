#pragma once

#include <queue>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define THREADCALL WINAPI
#define STRCASECMP _stricmp

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"

#if defined(_DEBUG) || defined(DEBUG)
#	include <cassert>
#	include "DebugUtils.h"

#	define ASSERT assert
#	define TRACE FormattedDebugString

//	Enables tracing to stdout. 
//	Preferable with the VS debugger (is thrown in the "output" window), but
//	it can be spammy otherwise (especially if you don't need it enabled).
#	define USE_SQL_TRACE

//	Ensure both typically used debug defines behave as intended
#	ifndef DEBUG
#		define DEBUG
#	endif

#	ifndef _DEBUG
#		define 0
#	endif

#else
#	define ASSERT 
#	define TRACE 
#endif

// Ignore the warning "nonstandard extension used: enum '<enum name>' used in qualified name"
// Sometimes it's necessary to avoid collisions, but aside from that, specifying the enumeration helps make code intent clearer.
#pragma warning(disable: 4482)

#define STR(str) #str
#define STRINGIFY(str) STR(str)

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

class SRWLock
{
	//#ifdef DEBUG
	unsigned tid;
	//#endif // !DEBUG


public:
	SRWLock()
	{
		//#ifdef DEBUG
		tid = 0xffffffff;
		//#endif
		InitializeSRWLock(&m_lock);
	}
	void w_lock()
	{
		//#ifdef DEBUG
		unsigned me = GetCurrentThreadId();
		if (tid != 0xffffffff)
		{
			if (tid == me)
			{
				printf("#Error: Recursive calls is not suitable for SRWLock, owner %d\n", me);
				ASSERT(0);
			}
		}
		//#endif


		AcquireSRWLockExclusive(&m_lock);
		//#ifdef DEBUG
		tid = me;
		//#endif
	}
	void w_unlock()
	{
		//#ifdef DEBUG
		tid = 0xffffffff;
		//#endif
		ReleaseSRWLockExclusive(&m_lock);
	}

	void r_lock()
	{
		//#ifdef DEBUG
		unsigned me = GetCurrentThreadId();
		if (tid != 0xffffffff)
		{
			if (tid == me)
			{

				printf("#Error: Recursive calls is not suitable for SRWLock, (RLOCK) %d\n", me);
				ASSERT(0);
			}
		}
		//#endif
		AcquireSRWLockShared(&m_lock);

		//#ifdef DEBUG
		tid = me;
		//#endif	
	}
	void r_unlock()
	{
		//#ifdef DEBUG
		tid = 0xffffffff;
		//#endif

		ReleaseSRWLockShared(&m_lock);

	}



public:
	SRWLOCK m_lock;

};
class ReadGuard
{
public:

	ReadGuard(SRWLock& mutex) : target(mutex) { target.r_lock(); }
	~ReadGuard() { target.r_unlock(); }

protected:
	SRWLock& target;
};

class WriteGuard
{
public:

	WriteGuard(SRWLock& mutex) : target(mutex) { target.w_lock(); }
	~WriteGuard() { target.w_unlock(); }

protected:
	SRWLock& target;
};

class Guard
{
public:
	Guard(std::recursive_mutex& mutex) : target(mutex) { target.lock(); }
	Guard(std::recursive_mutex* mutex) : target(*mutex) { target.lock(); }
	~Guard() { target.unlock(); }

protected:
	std::recursive_mutex& target;
};

#define sleep(ms) Sleep(ms)

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// define compiler-specific types
#include "types.h"

#include <random>
#include <memory>
#include <map>
#include <list>
#include <vector>

#include "tstring.h"
#include "globals.h"
#include "Atomic.h"
#include "Thread.h"
#include "Network.h"
#include "TimeThread.h"
#include "BugTrapHandler.h"

#ifdef _MSC_VER
#if( _SECURE_SCL != 0 )
#pragma message( "Warning: _SECURE_SCL != 0. You _will_ get either slowness or runtime errors." )
#endif

#if( _HAS_ITERATOR_DEBUGGING != 0 )
#pragma message( "Warning: _HAS_ITERATOR_DEBUGGING != 0. You _will_ get either slowness or runtime errors." )
#endif
#endif