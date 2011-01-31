#ifndef _LIBB_H_
#define _LIBB_H_

#include "rrlib/logging/definitions.h"

namespace libB
{
RRLIB_LOG_CREATE_DEFAULT_DOMAIN("libB");

void Test()
{
  RRLIB_LOG_STREAM(rrlib::logging::eLL_USER) << "Test in libB";
}
}

#endif
