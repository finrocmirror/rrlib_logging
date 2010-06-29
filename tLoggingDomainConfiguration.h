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
  eLL_VERBOSE,
  eLL_LOW,
  eLL_MEDIUM,
  eLL_HIGH,
  eLL_ALWAYS,
  eLL_DIMENSION
};

enum eLogStream
{
//  eLS_NULL,
  eLS_STDOUT,
  eLS_STDERR,
  eLS_FILE,
  eLS_COMBINED_FILE,
  eLS_DIMENSION
};

enum eLogStreamMask
{
//  eLSM_NULL = 1 << eLS_NULL,
  eLSM_STDOUT = 1 << eLS_STDOUT,
  eLSM_STDERR = 1 << eLS_STDERR,
  eLSM_FILE = 1 << eLS_FILE,
  eLSM_COMBINED_FILE = 1 << eLS_COMBINED_FILE,
  eLSM_DIMENSION = 1 << eLS_DIMENSION
};

inline const eLogStreamMask operator & (eLogStreamMask a, eLogStreamMask b)
{
  return static_cast<eLogStreamMask>(static_cast<int>(a) & static_cast<int>(b));
}
inline const eLogStreamMask operator | (eLogStreamMask a, eLogStreamMask b)
{
  return static_cast<eLogStreamMask>(static_cast<int>(a) | static_cast<int>(b));
}
inline const eLogStreamMask operator ~(eLogStreamMask a)
{
  return static_cast<eLogStreamMask>(~static_cast<int>(a));
}
inline eLogStreamMask &operator |= (eLogStreamMask &a, const eLogStreamMask &b)
{
  a = a | b;
  return a;
}

#ifdef _RRLIB_LOGGING_LESS_OUTPUT_
const eLogLevel cDEFAULT_MIN_LOG_LEVEL = eLL_HIGH;
const bool cDEFAULT_PRINT_TIME = false;
const bool cDEFAULT_PRINT_NAME = false;
const bool cDEFAULT_PRINT_LEVEL = false;
const bool cDEFAULT_PRINT_LOCATION = false;
#else
const eLogLevel cDEFAULT_MIN_LOG_LEVEL = eLL_MEDIUM;
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
class tLoggingDomainConfiguration
{
  friend class tLoggingDomainRegistry;
  friend class tLoggingDomain;

  std::string name;
  bool configure_sub_tree;

  bool enabled;
  bool print_time;
  bool print_name;
  bool print_level;
  bool print_location;
  eLogLevel min_message_level;
  eLogStreamMask stream_mask;

  explicit tLoggingDomainConfiguration(const std::string &name)
      : name(name),
      configure_sub_tree(false),
      enabled(false),
      print_time(cDEFAULT_PRINT_TIME),
      print_name(cDEFAULT_PRINT_NAME),
      print_level(cDEFAULT_PRINT_LEVEL),
      print_location(cDEFAULT_PRINT_LOCATION),
      min_message_level(cDEFAULT_MIN_LOG_LEVEL),
      stream_mask(eLSM_STDOUT)
  {}

  tLoggingDomainConfiguration(const tLoggingDomainConfiguration &other)
      : configure_sub_tree(other.configure_sub_tree),
      enabled(other.enabled),
      print_time(other.print_time),
      print_name(other.print_name),
      print_level(other.print_level),
      print_location(other.print_location),
      min_message_level(other.min_message_level),
      stream_mask(other.stream_mask)
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
    this->stream_mask = other.stream_mask;
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
