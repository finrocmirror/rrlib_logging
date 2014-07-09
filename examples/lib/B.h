#ifndef _LIB_B_H_
#define _LIB_B_H_

#include "rrlib/logging/messages.h"

namespace B
{
void Test()
{
  RRLIB_LOG_PRINT(USER, "User message from B::Test()");
}
}

#endif
