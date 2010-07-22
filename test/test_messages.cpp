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
#include <stdexcept>

extern "C"
{
#include <libgen.h>
}

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "logging/test/libA/libA.h"
#include "logging/test/libB/libB.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include "logging/test/debugging.h"

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
    RRLIB_LOG_STREAM(eLL_ERROR, my_domain) << "local class test";
  }
};

}


int main(int argc, char **argv)
{
  if (!tLogDomainRegistry::GetInstance().ConfigureFromFile("logging_config.xml"))
  {
    std::cout << "Loading configuration failed!";
    return EXIT_FAILURE;
  }

  tLogDomainRegistry::GetInstance().SetOutputFileNamePrefix(basename(argv[0]));


//  std::cout << tLogDomainRegistry::GetInstance() << std::endl;

//  tLogDomainRegistry::GetInstance().EnableDomain("global", true);
//  tLogDomainRegistry::GetInstance().SetDomainPrintsName("global", true);
//  tLogDomainRegistry::GetInstance().SetDomainPrintsTime("global", true);
//  tLogDomainRegistry::GetInstance().SetDomainPrintsLevel("global", true);
//  tLogDomainRegistry::GetInstance().SetDomainPrintsLocation("global", true);
//  tLogDomainRegistry::GetInstance().SetDomainMinMessageLevel("global", eLL_VERBOSE);
//  tLogDomainRegistry::GetInstance().SetDomainStreamID("global", eLS_FILE);

//  tLogDomainRegistry::GetInstance().SetDomainStreamMask("global", eLSM_STDOUT | eLSM_FILE | eLSM_COMBINED_FILE);

//  tLogDomainRegistry::GetInstance().EnableDomain(".local");
//  tLogDomainRegistry::GetInstance().EnableDomain(".local.class");
//  tLogDomainRegistry::GetInstance().EnableDomain(".example", true);
//  tLogDomainRegistry::GetInstance().SetDomainMinMessageLevel(".example", eLL_VERBOSE);
//  tLogDomainRegistry::GetInstance().SetDomainStreamID(".example", eLS_COMBINED_FILE);
//  tLogDomainRegistry::GetInstance().EnableDomain("global.libA");
//  tLogDomainRegistry::GetInstance().EnableDomain("global.libB");

  CREATE_SCOPED_LOGGING_DOMAIN("main");

  CREATE_NAMED_LOGGING_DOMAIN(my_domain, "main_named");

  RRLIB_LOG_STREAM(eLL_WARNING) << "foo";
  RRLIB_LOG_STREAM(eLL_ERROR, my_domain) << "foo2";
  RRLIB_LOG_MESSAGE(eLL_DEBUG, my_domain, "%s", "FOO");

  libA::Test();
  libB::Test();

  local::Test::function();

  DEBUGMSG("blablabla Debug");
  INFOMSG("blablabla Info");
  WARNINGMSG("blablabla Warning");
  ERRORMSG("blablabla Error");
  USERMSG("blablabla User");

  RRLIB_LOG_STREAM(eLL_ERROR) << std::runtime_error("runtime_error");

  return EXIT_SUCCESS;
}
