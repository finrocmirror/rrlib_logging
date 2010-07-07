#ifndef LIBA_H_
#define LIBA_H_

#include "rrlib/logging/definitions.h"

namespace libA
{
CREATE_SCOPED_LOGGING_DOMAIN("libA");

void Test()
{
  RRLIB_LOG_STREAM(rrlib::logging::eLL_VERBOSE) << "Test in libA" << std::endl;
}
}

#endif
