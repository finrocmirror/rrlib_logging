#ifndef _LIB_A_H_
#define _LIB_A_H_

#include "rrlib/logging/messages.h"

namespace A
{
void Test()
{
  RRLIB_LOG_PRINT(DEBUG, "Debug message from A::Test()");
}
}

#endif
