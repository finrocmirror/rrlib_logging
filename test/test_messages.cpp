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
  tLoggingDomainRegistry::GetInstance().SetOutputFileNamePrefix(basename(argv[0]));

  tLoggingDomainRegistry::GetInstance().EnableDomain("global", true);
  tLoggingDomainRegistry::GetInstance().SetDomainPrintsName("global", true);
  tLoggingDomainRegistry::GetInstance().SetDomainPrintsTime("global", true);
  tLoggingDomainRegistry::GetInstance().SetDomainPrintsLevel("global", true);
  tLoggingDomainRegistry::GetInstance().SetDomainPrintsLocation("global", true);
  tLoggingDomainRegistry::GetInstance().SetDomainMinMessageLevel("global", eML_VERBOSE);
//  tLoggingDomainRegistry::GetInstance().SetDomainStreamID("global", eMS_FILE);

//  tLoggingDomainRegistry::GetInstance().EnableDomain(".local");
//  tLoggingDomainRegistry::GetInstance().EnableDomain(".local.class");
//  tLoggingDomainRegistry::GetInstance().EnableDomain(".example", true);
//  tLoggingDomainRegistry::GetInstance().SetDomainMinMessageLevel(".example", eML_VERBOSE);
//  tLoggingDomainRegistry::GetInstance().SetDomainStreamID(".example", eMS_COMBINED_FILE);
//  tLoggingDomainRegistry::GetInstance().EnableDomain("global.libA");
//  tLoggingDomainRegistry::GetInstance().EnableDomain("global.libB");

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
