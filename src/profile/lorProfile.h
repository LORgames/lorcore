#ifndef LOR_PROFILE
#define LOR_PROFILE

#include "lorMath.h"

struct lorUserProfile
{
  uint32_t UserID;  //The unique user ID.
  char *pUsername;  //UTF-8 Username
};

#endif // LOR_PROFILE