#ifndef LIBA_H_
#define LIBA_H_

#include "logging/definitions.h"

namespace libA
{
RRLIB_LOG_CREATE_DEFAULT_DOMAIN("libA");

void Test()
{
  RRLIB_LOG_STREAM(rrlib::logging::eLL_DEBUG) << "Test in libA";
}
}

#endif
