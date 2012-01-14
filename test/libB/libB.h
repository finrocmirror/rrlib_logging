#ifndef _LIBB_H_
#define _LIBB_H_

#include "rrlib/logging/messages.h"

namespace libB
{
void Test()
{
  RRLIB_LOG_PRINT(rrlib::logging::eLL_USER, "Test in libB");
}
}

#endif
