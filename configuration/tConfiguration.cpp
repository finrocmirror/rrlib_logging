//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
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
/*!\file    tConfiguration.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/configuration/tConfiguration.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstring>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
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

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------
#ifdef _RRLIB_LOGGING_LESS_OUTPUT_
const bool cDEFAULT_PRINT_TIME = false;                 //!< Default print time setting for reduced output mode
const bool cDEFAULT_PRINT_NAME = false;                 //!< Default print name setting for reduced output mode
const bool cDEFAULT_PRINT_LEVEL = false;                //!< Default print level setting for reduced output mode
const bool cDEFAULT_PRINT_LOCATION = false;             //!< Default print location setting for reduced output mode
const tLogLevel cDEFAULT_MAX_LOG_LEVEL = eLL_WARNING;   //!< Default max log level for reduced output mode
const int cDEFAULT_SINK_MASK = 1 << eLS_STDOUT;         //!< Default output stream mask
#else
const bool cDEFAULT_PRINT_TIME = false;               //!< Default print time setting for normal output mode
const bool cDEFAULT_PRINT_NAME = false;               //!< Default print name setting for normal output mode
const bool cDEFAULT_PRINT_LEVEL = false;              //!< Default print level setting for normal output mode
const bool cDEFAULT_PRINT_LOCATION = true;            //!< Default print location setting for normal output mode
const tLogLevel cDEFAULT_MAX_LOG_LEVEL = eLL_DEBUG;   //!< Default max log level for normal output mode
const int cDEFAULT_SINK_MASK = 1 << eLS_STDOUT;       //!< Default output stream mask
#endif

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tConfiguration constructors
//----------------------------------------------------------------------
tConfiguration::tConfiguration(const tConfiguration *parent, const std::string &name)
    : parent(parent),
    name(name)//,
//    print_time(cDEFAULT_PRINT_TIME),
//    print_name(cDEFAULT_PRINT_NAME),
//    print_level(cDEFAULT_PRINT_LEVEL),
//    print_location(cDEFAULT_PRINT_LOCATION),
//    max_message_level(cDEFAULT_MAX_LOG_LEVEL),
//    sink_mask(cDEFAULT_SINK_MASK)
{}

//tConfiguration::tConfiguration(const tConfiguration &other)
//    : print_time(other.print_time),
//    print_name(other.print_name),
//    print_level(other.print_level),
//    print_location(other.print_location),
//    max_message_level(other.max_message_level),
//    sink_mask(other.sink_mask)
//{}
//
////----------------------------------------------------------------------
//// tConfiguration operator =
////----------------------------------------------------------------------
//tConfiguration &tConfiguration::operator = (const tConfiguration other)
//{
//  this->print_time = other.print_time;
//  this->print_name = other.print_name;
//  this->print_level = other.print_level;
//  this->print_location = other.print_location;
//  this->max_message_level = other.max_message_level;
//  this->sink_mask = other.sink_mask;
//  return *this;
//}

//----------------------------------------------------------------------
// tConfiguration destructor
//----------------------------------------------------------------------
tConfiguration::~tConfiguration()
{
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    delete *it;
  }
}

//----------------------------------------------------------------------
// tConfiguration GetConfigurationByName
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::GetConfigurationByName(const char *domain_name) const
{
  assert(domain_name);

  if (!*domain_name)
  {
    return *this;
  }

  const char *p = std::strchr(domain_name, '.');
  if (p)
  {
    return this->LookupChild(domain_name, p - domain_name).GetConfigurationByName(p + 1);
  }

  return this->LookupChild(domain_name, std::strlen(domain_name));
}

//----------------------------------------------------------------------
// tConfiguration GetConfigurationByFilename
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::GetConfigurationByFilename(const char *filename) const
{
  assert(filename);

  // Return directly if filename does not contain any folders (leaf)
  const char *p = std::strchr(filename, '/');
  if (!p)
  {
    return *this;
  }

  return this->LookupChild(filename, p - filename).GetConfigurationByFilename(p + 1);
}

const tConfiguration &tConfiguration::LookupChild(const char *name, size_t length) const
{
  tConfiguration *configuration = 0;
  auto insertion_point = this->children.end();

  // Iterate over sorted list of children (must be sorted longest name first to work)
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    const size_t current_length = (*it)->Name().length();

    // Skip too long names
    if (current_length > length)
    {
      continue;
    }

    // Compare matching names
    if (current_length == length)
    {
      if (std::strncmp(name, (*it)->Name().c_str(), length) == 0)
      {
        configuration = *it;
        break;
      }
    }

    // Shorter names start -> insert new configuration before
    insertion_point = it;
    break;
  }

  if (!configuration)
  {
    configuration = new tConfiguration(this, std::string(name, length));
    this->children.insert(insertion_point, configuration);
  }

  return *configuration;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
