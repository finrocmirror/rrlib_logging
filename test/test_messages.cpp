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
/*!\file    rrlib/logging/test/test_messages.cpp
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
#include <iterator>
#include <iomanip>

extern "C"
{
#include <libgen.h>
#include <unistd.h>
}

#include "rrlib/util/join.h"

#include "rrlib/logging/configuration.h"
#include "rrlib/logging/messages.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/test/libA/libA.h"
#include "rrlib/logging/test/libB/libB.h"

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
    RRLIB_LOG_PRINT_TO(my_domain, DEBUG_WARNING, "this ", "is a ", "local class test");

    RRLIB_LOG_PRINT(WARNING, "foo");
    RRLIB_LOG_PRINT_TO(my_domain, ERROR, "foo2");
    if (true)
    {
      RRLIB_LOG_PRINTF_TO(my_domain, DEBUG, "%s\n", "FOO");
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
    RRLIB_LOG_PRINT_STATIC(USER, "From static method");
  }

  void NonStaticMethod()
  {
    RRLIB_LOG_PRINT(USER, "From non-static method");
  }
};


int main(int argc, char **argv)
{
  rrlib::logging::default_log_description = basename(argv[0]);
  rrlib::logging::SetLogFilenamePrefix(basename(argv[0]));

#ifdef _LIB_RRLIB_XML_PRESENT_
  if (!rrlib::logging::ConfigureFromFile("logging_config.xml"))
  {
    RRLIB_LOG_PRINT(ERROR, "Loading configuration failed!");
    return EXIT_FAILURE;
  }

  rrlib::logging::PrintDomainConfigurations();
#endif

//  rrlib::logging::SetDomainPrintsName(".", true);
//  rrlib::logging::SetDomainPrintsTime(".", true);
//  rrlib::logging::SetDomainPrintsLevel(".", true);
//  rrlib::logging::SetDomainPrintsLocation(".", true);
//  rrlib::logging::SetDomainMaxMessageLevel(".", DEBUG_VERBOSE_3);
//  rrlib::logging::SetDomainSink(".", rrlib::logging::eLOG_SINK_FILE);

  rrlib::logging::SetDomainMaxMessageLevel(".example", rrlib::logging::tLogLevel::DEBUG_VERBOSE_3);
//  rrlib::logging::SetDomainSink(".example", rrlib::logging::eLOG_SINK_STDOUT, rrlib::logging::eLOG_SINK_COMBINED_FILE);

  RRLIB_LOG_PRINT(WARNING, "foo");

  libA::Test();
  libB::Test();

  local::Test::function();

  std::runtime_error exception("runtime_error");
  std::exception &e(exception);
  RRLIB_LOG_PRINT(ERROR, e);

  RRLIB_LOG_PRINT(WARNING, "0x", std::setw(20), std::setfill('0'), std::hex, 324);

  RRLIB_LOG_PRINT(ERROR, "Das hier ist ein mehrzeiliger\nFehler.\n");
  RRLIB_LOG_PRINT(USER, "Und das hier ein mehrzeiliger\nText fuer den lieben Benutzer.");

  const char* texts[] = {"Dies", "ist", "ein", "kleiner", "Text."};
  RRLIB_LOG_PRINT(DEBUG, rrlib::util::Join(texts, texts + 5, " "));

  TestStatic test_static;
  test_static.StaticMethod();
  test_static.NonStaticMethod();

  int *a = 0;
  const int *b = 0;
  RRLIB_LOG_PRINT(DEBUG, "Pointer: ", a);
  RRLIB_LOG_PRINT(DEBUG, "Const-Pointer: ", b);
  RRLIB_LOG_PRINT(DEBUG, "Bool: ", true, false);
  RRLIB_LOG_PRINT(DEBUG, "Function: ", main);
  RRLIB_LOG_PRINT(DEBUG, "Mal noch einzelne Zeichen: ", 'a', '\0', 'b');

  RRLIB_LOG_PRINT_TO(.speech_log, DEBUG, "This is a speech-log message");

  std::cout << "Waiting 10 seconds for optional speech synthesis" << std::endl;
  sleep(10);

  rrlib::logging::PrintDomainConfigurations();
  return EXIT_SUCCESS;
}
