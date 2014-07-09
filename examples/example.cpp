//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//----------------------------------------------------------------------
/*!\file    rrlib/logging/examples/example.cpp
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
#include <stdexcept>
#include <iomanip>

#include <libgen.h>

#include "rrlib/logging/configuration.h"
#include "rrlib/logging/messages.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/examples/lib/A.h"
#include "rrlib/logging/examples/lib/B.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------

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

struct Test
{
  static void function()
  {
    RRLIB_LOG_PRINT_TO(.custom_domain, DEBUG_WARNING, "this ", "is a ", "concatenated debug warning message from local::Test::function() to custom domain");

    RRLIB_LOG_PRINT(WARNING, "Warning message from local::Test::function()");
    RRLIB_LOG_PRINT_TO(.custom_domain, ERROR, "Warning message from local::Test::function() to custom domain");
    if (true)
    {
      RRLIB_LOG_PRINTF_TO(.custom_domain, DEBUG, "%s\n", "Debug message using printf syntax");
    }
  }
};

}

struct TestStatic
{
  const char *GetLogDescription()
  {
    return "TestStatic";
  }

  static void StaticMethod()
  {
    RRLIB_LOG_PRINT_STATIC(DEBUG, "Debug message from static context");
  }

  void NonStaticMethod()
  {
    RRLIB_LOG_PRINT(DEBUG, "Debug message from non-static context");
  }
};


int main(int argc, char **argv)
{
  /*** Setup some needed basic strings ***/
  rrlib::logging::default_log_description = basename(argv[0]);
  rrlib::logging::SetLogFilenamePrefix(basename(argv[0]));

  bool domains_configured = false;

  /*** Add configuring from file if xml support is available ***/
#ifdef _LIB_RRLIB_XML_PRESENT_
  if (argc > 2)
  {
    RRLIB_LOG_PRINTF(ERROR, "Usage: %s [logging_config.xml]", argv[0]);
    return EXIT_FAILURE;
  }
  if (argc == 2)
  {
    try
    {
      domains_configured = rrlib::logging::ConfigureFromFile(argv[1]);
    }
    catch (const rrlib::xml::tException &exception)
    {
      RRLIB_LOG_PRINT(ERROR, "Loading configuration failed: ", exception.what());
      return EXIT_FAILURE;
    }
  }
#else
  if (argc != 1)
  {
    RRLIB_LOG_PRINTF(ERROR, "Usage: %s", argv[0]);
    return EXIT_FAILURE;
  }
#endif

  if (!domains_configured)
  {
    /*** Our custom domain gets a custom configuration if not configured from file ***/
    RRLIB_LOG_PRINT(USER, "Using exemplary configuration for custom domain");
    rrlib::logging::SetDomainPrintsName(".custom_domain", true);
    rrlib::logging::SetDomainPrintsTime(".custom_domain", true);
    rrlib::logging::SetDomainPrintsLevel(".custom_domain", true);
    rrlib::logging::SetDomainPrintsLocation(".custom_domain", true);
    rrlib::logging::SetDomainMaxMessageLevel(".custom_domain", rrlib::logging::tLogLevel::DEBUG_VERBOSE_3);
  }

  /*** Have a look on the configured domains ***/
  RRLIB_LOG_PRINT(USER, "These are the configured log domains:");
  rrlib::logging::PrintDomainConfigurations();

  /*** Generate some messages from different contexts and of different type ***/
  RRLIB_LOG_PRINT(WARNING, "Warning message from main()");

  A::Test();
  B::Test();

  local::Test::function();

  RRLIB_LOG_PRINT(ERROR, std::runtime_error("Forwarding a std::runtime_error directly as error messsage"));

  RRLIB_LOG_PRINT(WARNING, "Warning message using std::iomanip: 0x", std::setw(20), std::setfill('0'), std::hex, 324);

  RRLIB_LOG_PRINT(ERROR, "Multiline error message with\npadding and handling of trailing newline.\n");
  RRLIB_LOG_PRINT(USER, "Multiline user message\nthat should behave accordingly.\n");

  TestStatic test_static;
  test_static.StaticMethod();
  test_static.NonStaticMethod();

  int *a = 0;
  const int *b = 0;
  RRLIB_LOG_PRINT(DEBUG, "Handling of some special data types:\n"
                  "- Pointer:\t\t\t", a, "\n",
                  "- Const-Pointer:\t\t", b, "\n",
                  "- Bool:\t\t\t", true, false, "\n",
                  "- Single characters:\t", 'a', '\0', 'b', "\n");

  /*** In the end, get a list of domains that were configured or used by this program ***/
  RRLIB_LOG_PRINT(USER, "These are the used and configured log domains:");
  rrlib::logging::PrintDomainConfigurations();

  return EXIT_SUCCESS;
}
