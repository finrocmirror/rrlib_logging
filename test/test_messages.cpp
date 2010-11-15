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
 */
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
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
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

namespace local
{

RRLIB_LOG_CREATE_DEFAULT_DOMAIN("local");

struct Test
{
  RRLIB_LOG_CREATE_NAMED_DOMAIN(my_domain, "class");

  static void function()
  {
    RRLIB_LOG_STREAM(eLL_DEBUG_WARNING, my_domain, "this ", "is a ") << "local class test";

    RRLIB_LOG_STREAM(eLL_WARNING) << "foo" << std::endl;
    RRLIB_LOG_STREAM(eLL_ERROR, my_domain) << "foo2" << std::endl;
    if (true)
    {
      RRLIB_LOG_MESSAGE(eLL_DEBUG, my_domain, "%s\n", "FOO");
    }
  }
};

}


int main(int argc, char **argv)
{
  default_log_description = basename(argv[0]);

#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
  if (!tLogDomainRegistry::GetInstance()->ConfigureFromFile("logging_config.xml"))
  {
    std::cout << "Loading configuration failed!";
    return EXIT_FAILURE;
  }
#endif

  tLogDomainRegistry::GetInstance()->SetOutputFileNamePrefix(basename(argv[0]));


//  std::cout << tLogDomainRegistry::GetInstance() << std::endl;

//  tLogDomainRegistry::GetInstance()->SetDomainConfiguresSubTree("global", true);
//  tLogDomainRegistry::GetInstance()->SetDomainPrintsName("global", true);
//  tLogDomainRegistry::GetInstance()->SetDomainPrintsTime("global", true);
//  tLogDomainRegistry::GetInstance()->SetDomainPrintsLevel("global", true);
//  tLogDomainRegistry::GetInstance()->SetDomainPrintsLocation("global", true);
//  tLogDomainRegistry::GetInstance()->SetDomainMinMessageLevel("global", eLL_VERBOSE);
//  tLogDomainRegistry::GetInstance()->SetDomainStreamID("global", eLS_FILE);

//  tLogDomainRegistry::GetInstance()->SetDomainStreamMask("global", eLSM_STDOUT | eLSM_FILE | eLSM_COMBINED_FILE);

//  tLogDomainRegistry::GetInstance()->SetDomainConfiguresSubTree(".example", true);
//  tLogDomainRegistry::GetInstance()->SetDomainMinMessageLevel(".example", eLL_VERBOSE);
//  tLogDomainRegistry::GetInstance()->SetDomainStreamID(".example", eLS_COMBINED_FILE);


  RRLIB_LOG_STREAM(eLL_WARNING) << "foo" << std::endl;

  libA::Test();
  libB::Test();

  local::Test::function();

  DEBUGMSG("blablabla Debug");
  INFOMSG("blablabla Info");
  WARNINGMSG("blablabla Warning");
  ERRORMSG("blablabla Error");
  USERMSG("blablabla User");

  RRLIB_LOG_STREAM(eLL_ERROR) << std::runtime_error("runtime_error");

  RRLIB_LOG_STREAM(eLL_WARNING) << std::hex << 324 << std::endl;

  RRLIB_LOG_STREAM(eLL_ERROR) << "Das hier ist ein mehrzeiliger\nFehler.";
  RRLIB_LOG_STREAM(eLL_USER) << "Und das hier ein mehrzeiliger\nText fuer den lieben Benutzer.";

  return EXIT_SUCCESS;
}
