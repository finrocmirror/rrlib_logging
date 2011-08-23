#ifndef _LIBA_H_
#define _LIBA_H_

#include "rrlib/logging/definitions.h"

namespace libA
{
RRLIB_LOG_CREATE_DEFAULT_DOMAIN("libA");

void Test()
{
  RRLIB_LOG_PRINT(rrlib::logging::eLL_DEBUG, "Test in libA");
}
}

#endif
