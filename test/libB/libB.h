#ifndef LIBB_H_
#define LIBB_H_

#include "logging/definitions.h"

namespace libB
{
CREATE_SCOPED_LOGGING_DOMAIN("libB");

void Test()
{
  RRLIB_LOG_STREAM(rrlib::logging::eLL_ALWAYS) << "Test in libB" << std::endl;
}
}

#endif
