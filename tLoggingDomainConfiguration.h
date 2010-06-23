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
/*!\file    tLoggingDomainConfiguration.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-20
 *
 * \brief
 *
 * \b
 *
 * A few words for tLoggingDomainConfiguration.h
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_logging_include_guard_
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef rrlib_logging_tLoggingDomainConfiguration_h_
#define rrlib_logging_tLoggingDomainConfiguration_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <string>
#include <tr1/memory>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace logging
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------
enum eLogLevel
{
  eML_VERBOSE,
  eML_LOW,
  eML_MEDIUM,
  eML_HIGH,
  eML_ALWAYS,
  eML_DIMENSION
};

enum eLogStream
{
  eMS_NULL,
  eMS_STDOUT,
  eMS_STDERR,
  eMS_FILE,
  eMS_COMBINED_FILE,
  eMS_DIMENSION
};

#ifdef _RRLIB_LOGGING_LESS_OUTPUT_
const eLogLevel cDEFAULT_MIN_LOG_LEVEL = eML_HIGH;
const bool cDEFAULT_PRINT_TIME = false;
const bool cDEFAULT_PRINT_NAME = false;
const bool cDEFAULT_PRINT_LEVEL = false;
const bool cDEFAULT_PRINT_LOCATION = false;
#else
const eLogLevel cDEFAULT_MIN_LOG_LEVEL = eML_MEDIUM;
const bool cDEFAULT_PRINT_TIME = false;
const bool cDEFAULT_PRINT_NAME = false;
const bool cDEFAULT_PRINT_LEVEL = false;
const bool cDEFAULT_PRINT_LOCATION = true;
#endif

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! Short description of tLoggingDomainConfiguration
/*! A more detailed description of tLoggingDomainConfiguration, which
 *  Tobias Foehst hasn't done yet!
 *
 */
struct tLoggingDomainConfiguration
{
  std::string name;
  bool configure_sub_tree;

  bool enabled;
  bool print_time;
  bool print_name;
  bool print_level;
  bool print_location;
  eLogLevel min_message_level;
  eLogStream stream_id;

  explicit tLoggingDomainConfiguration(const std::string &name)
      : name(name),
      configure_sub_tree(false),
      enabled(false),
      print_time(cDEFAULT_PRINT_TIME),
      print_name(cDEFAULT_PRINT_NAME),
      print_level(cDEFAULT_PRINT_LEVEL),
      print_location(cDEFAULT_PRINT_LOCATION),
      min_message_level(cDEFAULT_MIN_LOG_LEVEL),
      stream_id(eMS_STDOUT)
  {}

  tLoggingDomainConfiguration(const tLoggingDomainConfiguration &other)
      : configure_sub_tree(other.configure_sub_tree),
      enabled(other.enabled),
      print_time(other.print_time),
      print_name(other.print_name),
      print_level(other.print_level),
      print_location(other.print_location),
      min_message_level(other.min_message_level),
      stream_id(other.stream_id)
  {}

  tLoggingDomainConfiguration &operator = (const tLoggingDomainConfiguration other)
  {
    this->configure_sub_tree = other.configure_sub_tree;
    this->enabled = other.enabled;
    this->print_time = other.print_time;
    this->print_name = other.print_name;
    this->print_level = other.print_level;
    this->print_location = other.print_location;
    this->min_message_level = other.min_message_level;
    this->stream_id = other.stream_id;
    return *this;
  }
};

typedef std::tr1::shared_ptr<tLoggingDomainConfiguration> tLoggingDomainConfigurationSharedPointer;

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
