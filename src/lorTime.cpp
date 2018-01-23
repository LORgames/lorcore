#include "lorTime.h"

#include <chrono>

//Returns number of seconds since the start of January 1, 1970 @ UTC
uint64_t lorTime_GetCurrentTimestamp()
{
  return std::chrono::system_clock::now().time_since_epoch().count() / std::chrono::system_clock::period::den;
}
