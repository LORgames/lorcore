#include "lorTime.h"

#include <chrono>
#include <ctime>

//Returns number of seconds since the start of January 1, 1970 @ UTC
uint64_t lorTime_GetCurrentTimestamp()
{
  timespec ts;
  timespec_get(&ts, TIME_UTC);
  return ts.tv_sec; //Hopefully always UNIX...
}
