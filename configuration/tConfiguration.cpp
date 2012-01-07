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
#include "rrlib/logging/configuration/tDomainRegistry.h"

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
const bool cDEFAULT_PRINTS_NAME = false;                //!< Default prints name setting for reduced output mode
const bool cDEFAULT_PRINTS_TIME = false;                //!< Default prints time setting for reduced output mode
const bool cDEFAULT_PRINTS_LEVEL = false;               //!< Default prints level setting for reduced output mode
const bool cDEFAULT_PRINTS_LOCATION = false;            //!< Default prints location setting for reduced output mode
const tLogLevel cDEFAULT_MAX_LOG_LEVEL = eLL_WARNING;   //!< Default max log level for reduced output mode
const int cDEFAULT_SINK_MASK = 1 << eLS_STDOUT;         //!< Default output stream mask
#else
const bool cDEFAULT_PRINTS_NAME = false;                //!< Default prints name setting for normal output mode
const bool cDEFAULT_PRINTS_TIME = false;                //!< Default prints time setting for normal output mode
const bool cDEFAULT_PRINTS_LEVEL = false;               //!< Default prints level setting for normal output mode
const bool cDEFAULT_PRINTS_LOCATION = true;             //!< Default prints location setting for normal output mode
const tLogLevel cDEFAULT_MAX_LOG_LEVEL = eLL_DEBUG;     //!< Default max log level for normal output mode
const int cDEFAULT_SINK_MASK = 1 << eLS_STDOUT;         //!< Default output stream mask
#endif

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tConfiguration constructors
//----------------------------------------------------------------------
tConfiguration::tConfiguration(const tConfiguration *parent, const std::string &name)
    : parent(parent),
    name(name),
    prints_name(parent ? parent->prints_name : cDEFAULT_PRINTS_NAME),
    prints_time(parent ? parent->prints_time : cDEFAULT_PRINTS_TIME),
    prints_level(parent ? parent->prints_level : cDEFAULT_PRINTS_LEVEL),
    prints_location(parent ? parent->prints_location : cDEFAULT_PRINTS_LOCATION),
    max_message_level(parent ? parent->max_message_level : cDEFAULT_MAX_LOG_LEVEL)//,
//    sink_mask(cDEFAULT_SINK_MASK)
{
  this->stream_buffer.AddStream(std::cout);
}

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
// tConfiguration SetPrintsName
//----------------------------------------------------------------------
void tConfiguration::SetPrintsName(bool value)
{
  std::cout << "SetPrintsName: " << value << " in " << this->GetFullQualifiedName() << std::endl;
  this->prints_name = value;
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->SetPrintsName(value);
  }
}

//----------------------------------------------------------------------
// tConfiguration SetPrintsTime
//----------------------------------------------------------------------
void tConfiguration::SetPrintsTime(bool value)
{
  this->prints_time = value;
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->SetPrintsTime(value);
  }
}

//----------------------------------------------------------------------
// tConfiguration SetPrintsLevel
//----------------------------------------------------------------------
void tConfiguration::SetPrintsLevel(bool value)
{
  this->prints_level = value;
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->SetPrintsLevel(value);
  }
}

//----------------------------------------------------------------------
// tConfiguration SetPrintsLocation
//----------------------------------------------------------------------
void tConfiguration::SetPrintsLocation(bool value)
{
  this->prints_location = value;
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->SetPrintsLocation(value);
  }
}

//----------------------------------------------------------------------
// tConfiguration SetMaxMessageLevel
//----------------------------------------------------------------------
void tConfiguration::SetMaxMessageLevel(tLogLevel level)
{
  this->max_message_level = level;
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->SetMaxMessageLevel(level);
  }
}

//----------------------------------------------------------------------
// tConfiguration GetConfigurationByName
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::GetConfigurationByName(const char *domain_name) const
{
  assert(domain_name && *domain_name);

  const char *delimiter = std::strchr(domain_name, '.');

  if (!delimiter)
  {
    return this->LookupChild(domain_name, std::strlen(domain_name));
  }

  return this->LookupChild(domain_name, delimiter - domain_name).GetConfigurationByName(delimiter + 1);
}

//----------------------------------------------------------------------
// tConfiguration GetConfigurationByFilename
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::GetConfigurationByFilename(const char *filename) const
{
  assert(filename && *filename);

  const char *delimiter = std::strchr(filename, '/');

  if (!delimiter)
  {
    return *this;
  }

  return this->LookupChild(filename, delimiter - filename).GetConfigurationByFilename(delimiter + 1);
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

  // Add child if needed
  if (!configuration)
  {
    configuration = new tConfiguration(this, std::string(name, length));
    const tConfiguration *parent = configuration->parent;
    size_t full_name_length = 0;
    while (parent)
    {
      full_name_length += parent->Name().length() + 1;
      parent = parent->parent;
    }
    tDomainRegistry::Instance().UpdateMaxDomainNameLength(full_name_length + configuration->Name().length());
    this->children.insert(insertion_point, configuration);
  }

  return *configuration;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
