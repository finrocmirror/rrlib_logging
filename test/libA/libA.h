#ifndef _LIBA_H_
#define _LIBA_H_

#include "rrlib/logging/messages.h"

namespace libA
{
void Test()
{
  RRLIB_LOG_PRINT(rrlib::logging::eLL_DEBUG, "Test in libA");
}
}

#endif
