//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) AG Robotersysteme TU Kaiserslautern
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//----------------------------------------------------------------------
/*!\file    test_messages.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief
 *
 * \b
 *
 * A few words for test_messages.cpp
 *
 */
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <cstdlib>
#include <iostream>

extern "C"
{
#include <libgen.h>
}

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/test/libA/libA.h"
#include "rrlib/logging/test/libB/libB.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include "rrlib/logging/test/debugging.h"

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::logging;

//----------------------------------------------------------------------
// Forward declarations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

namespace local
{
CREATE_SCOPED_LOGGING_DOMAIN("local");

struct Test
{
  CREATE_NAMED_LOGGING_DOMAIN(my_domain, "class");

  static void function()
  {
    RRLIB_LOG_STREAM(eML_HIGH, my_domain) << "local class test" << std::endl;
  }
};

}


int main(int argc, char **argv)
{
  rrlib::logging::tLoggingDomainRegistry::GetInstance().SetOutputFileNamePrefix(basename(argv[0]));

  rrlib::logging::tLoggingDomainRegistry::GetInstance().EnableDomain("global", true);

  rrlib::logging::tLoggingDomainRegistry::GetInstance().SetDomainPrintsName("global", true);
  rrlib::logging::tLoggingDomainRegistry::GetInstance().SetDomainPrintsTime("global", true);
  rrlib::logging::tLoggingDomainRegistry::GetInstance().SetDomainPrintsLevel("global", true);
  rrlib::logging::tLoggingDomainRegistry::GetInstance().SetDomainPrintsLocation("", true);
  rrlib::logging::tLoggingDomainRegistry::GetInstance().SetDomainMinMessageLevel("global", rrlib::logging::eML_VERBOSE);
//  rrlib::logging::tLoggingDomainRegistry::GetInstance().SetDomainStreamID("global", rrlib::logging::eMS_FILE);

//  rrlib::logging::tLoggingDomainRegistry::GetInstance().EnableDomain("global.local");
//  rrlib::logging::tLoggingDomainRegistry::GetInstance().EnableDomain("global.local.class");
//  rrlib::logging::tLoggingDomainRegistry::GetInstance().EnableDomain("global.example", true);
//  rrlib::logging::tLoggingDomainRegistry::GetInstance().EnableDomain("global.libA");
//  rrlib::logging::tLoggingDomainRegistry::GetInstance().EnableDomain("global.libB");

  CREATE_SCOPED_LOGGING_DOMAIN("example");

  CREATE_NAMED_LOGGING_DOMAIN(my_domain, "example_2");

  RRLIB_LOG_STREAM(eML_MEDIUM) << "foo" << std::endl;
  RRLIB_LOG_STREAM(eML_HIGH, my_domain) << "foo2" << std::endl;
  RRLIB_LOG_MESSAGE(eML_LOW, ScopedLoggingDomain, "%s\n", "FOO");

  libA::Test();
  libB::Test();

  local::Test::function();

  DEBUGMSG("blablabla Debug\n");
  INFOMSG("blablabla Info\n");
  WARNINGMSG("blablabla Warning\n");
  ERRORMSG("blablabla Error\n");
  USERMSG("blablabla User\n");

  return EXIT_SUCCESS;
}
