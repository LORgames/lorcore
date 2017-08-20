#ifndef lorThreads__h_
#define lorThreads__h_

#include "lorCore.h"

enum
{
  lorThread_InfiniteTimeout = -1,
};

struct lorThread;
typedef int32_t (lorThreadFunction)(void *pData);

void lorThread_Create(lorThread **ppThread, lorThreadFunction *pFunc, void *pFuncData);
void lorThread_Destroy(lorThread **ppThread);
void lorThread_Join(lorThread *pThread);

struct lorMutex;

void lorMutex_Create(lorMutex **ppMutex);
void lorMutex_Destroy(lorMutex **ppMutex);
void lorMutex_Lock(lorMutex *pMutex);
void lorMutex_Unlock(lorMutex *pMutex);

struct lorConditionVariable;

void lorConditionVariable_Create(lorConditionVariable **ppConditionVariable);
void lorConditionVariable_Destroy(lorConditionVariable **ppConditionVariable);
void lorConditionVariable_NotifyOne(lorConditionVariable *pConditionVariable);
void lorConditionVariable_NotifyAll(lorConditionVariable *pConditionVariable);
void lorConditionVariable_Wait(lorConditionVariable *pConditionVariable, lorMutex *pMutex);
// Returns true false if timedout. Note: returns true if awakened spuriously.
bool lorConditionVariable_TimedWait(lorConditionVariable *pConditionVariable, lorMutex *pMutex, int ms = lorThread_InfiniteTimeout);

struct lorSemaphore;

void lorSemaphore_Create(lorSemaphore **ppSemaphore, int initialValue = 0);
void lorSemaphore_Destroy(lorSemaphore **ppSemaphore);
void lorSemaphore_Notify(lorSemaphore *pSemaphore, int count = 1);
void lorSemaphore_Wait(lorSemaphore *pSemaphore);
bool lorSemaphore_TimedWait(lorSemaphore *pSemaphore, int ms = lorThread_InfiniteTimeout);

struct lorAtomicInt32;

void lorAtomicInt32_Create(lorAtomicInt32 **ppAtomic, int32_t initialValue);
void lorAtomicInt32_Destroy(lorAtomicInt32 **ppAtomic);
void lorAtomicInt32_Set(lorAtomicInt32 *pAtomic, int32_t value);
int32_t lorAtomicInt32_Get(lorAtomicInt32 *pAtomic);
// Returns previous value
int32_t lorAtomicInt32_Exchange(lorAtomicInt32 *pAtomic, int32_t value);
int32_t lorAtomicInt32_FetchAdd(lorAtomicInt32 *pAtomic, int32_t value);
int32_t lorAtomicInt32_FetchSub(lorAtomicInt32 *pAtomic, int32_t value);
int32_t lorAtomicInt32_FetchAnd(lorAtomicInt32 *pAtomic, int32_t value);
int32_t lorAtomicInt32_FetchOr(lorAtomicInt32 *pAtomic, int32_t value);
int32_t lorAtomicInt32_FetchXor(lorAtomicInt32 *pAtomic, int32_t value);

void lorSleep(int ms);

#endif // lorThreads__h_
