
#ifndef _VIT_LOCK_H_LEE_
#define _VIT_LOCK_H_LEE_

#include "VitPlatformConfig.h"

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
#include <WinSock2.h>
#endif

typedef struct SVitLock
{
	SVitLock()
	{
		incLock = new pthread_mutex_t();
	}
	
#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
	CRITICAL_SECTION  incLock;
#else
	pthread_mutex_t*  incLock;
#endif
	
}SVitLock;

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
//--create lock--
#define VitCreateLock(lock){ InitializeCriticalSection (&lock.incLock); }
//--destroylock--
#define VitDestroyLock (lock){ DeleteCriticalSection (&lock.incLock); }
//--locking--
#define VitLock(lock){ EnterCriticalSection (&lock.incLock); }
//--unlock--
#define VitUnLock(lock){ LeaveCriticalSection (&lock.incLock); }
#else
//--create lock--
#define VitCreateLock(lock){ pthread_mutex_init (lock.incLock,NULL); }
//--destroylock--
#define VitDestroyLock (lock){ pthread_mutex_destroy (lock.incLock); }
//--locking--
#define VitLock(lock){ pthread_mutex_lock (lock.incLock); }
//--unlock--
#define VitUnLock(lock){ pthread_mutex_unlock (lock.incLock); }
#endif

#endif	// _VIT_LOCK_H_LEE_
