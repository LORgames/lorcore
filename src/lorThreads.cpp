#include "lorThreads.h"

#if LORPLATFORM_WINDOWS
# define WIN32_LEAN_AND_MEAN
# include "Windows.h"
#else
# include <time.h>
#endif

// Basic C++11 implementation
#include <thread>

struct lorThread
{
  std::thread thread;
};

void lorThread_Create(lorThread **ppThread, lorThreadFunction *pFunc, void *pFuncData)
{
  if (ppThread == nullptr || pFunc == nullptr)
    return;

  (*ppThread) = lorAllocType(lorThread, 1);
  new (&((*ppThread)->thread)) std::thread(pFunc, pFuncData);
}

void lorThread_Destroy(lorThread **ppThread)
{
  if (ppThread == nullptr || (*ppThread) == nullptr)
    return;

  (*ppThread)->thread.~thread();
  lorFree(*ppThread);
}

void lorThread_Join(lorThread *pThread)
{
  if (pThread != nullptr)
    pThread->thread.join();
}

// Basic C++11 implementation
#include <mutex>
struct lorMutex
{
  std::mutex mutex;
};

void lorMutex_Create(lorMutex **ppMutex)
{
  if (ppMutex == nullptr)
    return;

  (*ppMutex) = lorAllocType(lorMutex, 1);
  new (&((*ppMutex)->mutex)) std::mutex();
}

void lorMutex_Destroy(lorMutex **ppMutex)
{
  if (ppMutex == nullptr || (*ppMutex) == nullptr)
    return;

  (*ppMutex)->mutex.~mutex();
  lorFree(*ppMutex);
}

void lorMutex_Lock(lorMutex *pMutex)
{
  if (pMutex != nullptr)
    pMutex->mutex.lock();
}

void lorMutex_Unlock(lorMutex *pMutex)
{
  if (pMutex != nullptr)
    pMutex->mutex.unlock();
}

// Basic C++11 implementation
#include <condition_variable>
struct lorConditionVariable
{
  std::condition_variable condition;
};

void lorConditionVariable_Create(lorConditionVariable **ppConditionVariable)
{
  if (ppConditionVariable == nullptr)
    return;

  (*ppConditionVariable) = lorAllocType(lorConditionVariable, 1);
  new (&((*ppConditionVariable)->condition)) std::condition_variable();
}

void lorConditionVariable_Destroy(lorConditionVariable **ppConditionVariable)
{
  if (ppConditionVariable == nullptr || (*ppConditionVariable) == nullptr)
    return;

  (*ppConditionVariable)->condition.~condition_variable();
  lorFree(*ppConditionVariable);
}

void lorConditionVariable_NotifyOne(lorConditionVariable *pConditionVariable)
{
  if (pConditionVariable != nullptr)
    (*pConditionVariable).condition.notify_one();
}

void lorConditionVariable_NotifyAll(lorConditionVariable *pConditionVariable)
{
  if (pConditionVariable != nullptr)
    (*pConditionVariable).condition.notify_all();
}

bool lorConditionVariable_Wait(lorConditionVariable *pConditionVariable, lorMutex *pMutex, int ms /*= lorThread_InfiniteTimeout*/)
{
  std::unique_lock<std::mutex> lock(pMutex->mutex);

  if (pConditionVariable != nullptr)
    return false;

  if (ms == lorThread_InfiniteTimeout)
  {
    (*pConditionVariable).condition.wait(lock);
    return true;
  }
  else
  {
    // wow, best standard
    auto now = std::chrono::system_clock::now();

    return (*pConditionVariable).condition.wait_until(lock, now + std::chrono::milliseconds(ms)) == std::cv_status::no_timeout;
  }
}

struct lorSemaphore
{
  lorMutex *pMutex;
  lorConditionVariable *pCondition;
  int count = 0;
};

void lorSemaphore_Create(lorSemaphore **ppSemaphore, int initialValue /*= 0*/)
{
  if (ppSemaphore == nullptr)
    return;

  (*ppSemaphore) = lorAllocType(lorSemaphore, 1);
  lorMutex_Create(&(*ppSemaphore)->pMutex);
  lorConditionVariable_Create(&(*ppSemaphore)->pCondition);
  (*ppSemaphore)->count = initialValue;
}

void lorSemaphore_Destroy(lorSemaphore **ppSemaphore)
{
  if (ppSemaphore == nullptr || (*ppSemaphore) == nullptr)
    return;

  lorMutex_Destroy(&(*ppSemaphore)->pMutex);
  lorConditionVariable_Destroy(&(*ppSemaphore)->pCondition);
  lorFree(*ppSemaphore);
}

void lorSemaphore_Notify(lorSemaphore *pSemaphore, int count /*= 1*/)
{
  if (pSemaphore == nullptr)
    return;

  std::unique_lock<std::mutex> lock(pSemaphore->pMutex->mutex);
  while (count-- > 0)
  {
    ++(pSemaphore->count);
    lorConditionVariable_NotifyOne(pSemaphore->pCondition);
  }
}

bool lorSemaphore_Wait(lorSemaphore *pSemaphore, int ms /*= lorThread_InfiniteTimeout*/)
{
  if (pSemaphore == nullptr)
    return false;

  std::unique_lock<std::mutex> lock(pSemaphore->pMutex->mutex);
  if (ms == lorThread_InfiniteTimeout)
  {
    while (pSemaphore->count == 0) // handle spurious wake-ups
      pSemaphore->pCondition->condition.wait(lock);

    --(pSemaphore->count);

    return true;
  }
  else
  {
    auto now = std::chrono::system_clock::now();
    bool retVal = pSemaphore->pCondition->condition.wait_until(lock, now + std::chrono::milliseconds(ms)) == std::cv_status::no_timeout;

    if (retVal)
    {
      if (pSemaphore->count > 0)
        --(pSemaphore->count);
      else
        retVal = false;
    }

    return retVal;
  }
}

// Basic C++11 implementation
#include <atomic>

struct lorAtomicInt32
{
  std::atomic<int32_t> atomic;
};

void lorAtomicInt32_Create(lorAtomicInt32 **ppAtomic, int32_t initialValue)
{
  if (ppAtomic == nullptr)
    return;

  (*ppAtomic) = lorAllocType(lorAtomicInt32, 1);
  new (&((*ppAtomic)->atomic)) std::atomic<int32_t>(initialValue);
}

void lorAtomicInt32_Destroy(lorAtomicInt32 **ppAtomic)
{
  if (ppAtomic == nullptr || (*ppAtomic) == nullptr)
    return;

  lorFree(*ppAtomic);
}

void lorAtomicInt32_Set(lorAtomicInt32 *pAtomic, int32_t value)
{
  if (pAtomic == nullptr)
    return;

  pAtomic->atomic.store(value);
}

int32_t lorAtomicInt32_Get(lorAtomicInt32 *pAtomic)
{
  if (pAtomic == nullptr)
    return 0;

  return pAtomic->atomic.load();
}

int32_t lorAtomicInt32_Exchange(lorAtomicInt32 *pAtomic, int32_t value)
{
  if (pAtomic == nullptr)
    return 0;

  return pAtomic->atomic.exchange(value);
}

int32_t lorAtomicInt32_FetchAdd(lorAtomicInt32 *pAtomic, int32_t value)
{
  if (pAtomic == nullptr)
    return 0;

  return pAtomic->atomic.fetch_add(value);
}

int32_t lorAtomicInt32_FetchSub(lorAtomicInt32 *pAtomic, int32_t value)
{
  if (pAtomic == nullptr)
    return 0;

  return pAtomic->atomic.fetch_sub(value);
}

int32_t lorAtomicInt32_FetchAnd(lorAtomicInt32 *pAtomic, int32_t value)
{
  if (pAtomic == nullptr)
    return 0;

  return pAtomic->atomic.fetch_and(value);
}

int32_t lorAtomicInt32_FetchOr(lorAtomicInt32 *pAtomic, int32_t value)
{
  if (pAtomic == nullptr)
    return 0;

  return pAtomic->atomic.fetch_or(value);
}

int32_t lorAtomicInt32_FetchXor(lorAtomicInt32 *pAtomic, int32_t value)
{
  if (pAtomic == nullptr)
    return 0;

  return pAtomic->atomic.fetch_xor(value);
}

void lorSleep(int ms)
{
#if LORPLATFORM_WINDOWS || LORPLATFORM_WINRT || LORPLATFORM_WINPHONE
  Sleep(ms);
#else
  timespec sleepTime;
  sleepTime.tv_sec = (ms / 1000);
  sleepTime.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&sleepTime, NULL);
#endif
}
