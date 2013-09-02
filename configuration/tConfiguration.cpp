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
/*!\file    rrlib/logging/configuration/tConfiguration.cpp
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
#include <sstream>

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
const int cLOG_SINK_COMBINED_FILE_CHILD_MASK = 1 << eLOG_SINK_DIMENSION;

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tConfiguration constructors
//----------------------------------------------------------------------
tConfiguration::tConfiguration(const tDefaultConfigurationContext &default_context, const tConfiguration *parent, const std::string &name)
  : parent(parent),
    name(name),
    prints_name(parent ? parent->prints_name : default_context.cPRINTS_NAME),
    prints_time(parent ? parent->prints_time : default_context.cPRINTS_TIME),
    prints_level(parent ? parent->prints_level : default_context.cPRINTS_LEVEL),
    prints_location(parent ? parent->prints_location : default_context.cPRINTS_LOCATION),
    max_message_level(parent ? parent->max_message_level : default_context.cMAX_LOG_LEVEL),
    sink_mask(parent ? parent->sink_mask | cLOG_SINK_COMBINED_FILE_CHILD_MASK : default_context.cSINK_MASK),
    stream_buffer_ready(false)
{
  assert(name.length() || !parent);
}

//----------------------------------------------------------------------
// tConfiguration destructor
//----------------------------------------------------------------------
tConfiguration::~tConfiguration()
{
  if (this->file_stream.is_open())
  {
    this->file_stream.close();
  }

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
// tConfiguration SetSinkMask
//----------------------------------------------------------------------
void tConfiguration::SetSinkMask(int sink_mask)
{
  this->sink_mask = sink_mask;
  this->stream_buffer_ready = false;

  sink_mask |= cLOG_SINK_COMBINED_FILE_CHILD_MASK;

  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->SetSinkMask(sink_mask);
  }
}

//----------------------------------------------------------------------
// tConfiguration GetConfigurationByName
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::GetConfigurationByName(const tDefaultConfigurationContext &default_context, const char *domain_name) const
{
  assert(domain_name && *domain_name);

  const char *delimiter = std::strchr(domain_name, '.');

  if (!delimiter)
  {
    return this->LookupChild(default_context, domain_name, std::strlen(domain_name));
  }

  return this->LookupChild(default_context, domain_name, delimiter - domain_name).GetConfigurationByName(default_context, delimiter + 1);
}

//----------------------------------------------------------------------
// tConfiguration GetConfigurationByFilename
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::GetConfigurationByFilename(const tDefaultConfigurationContext &default_context, const char *filename) const
{
  assert(filename && *filename);

  const char *delimiter = std::strchr(filename, '/');

  if (!delimiter)
  {
    return *this;
  }

  return this->LookupChild(default_context, filename, delimiter - filename).GetConfigurationByFilename(default_context, delimiter + 1);
}

//----------------------------------------------------------------------
// tConfiguration LookupChild
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::LookupChild(const tDefaultConfigurationContext &default_context, const char *name, size_t length) const
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
      continue;
    }

    // Shorter names start -> insert new configuration before
    insertion_point = it;
    break;
  }

  // Add child if needed
  if (!configuration)
  {
    configuration = new tConfiguration(default_context, this, std::string(name, length));
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
// tConfiguration PrepareStreamBuffer
//----------------------------------------------------------------------
void tConfiguration::PrepareStreamBuffer() const
{
  this->stream_buffer.Clear();
  if (this->sink_mask & (1 << eLOG_SINK_STDOUT))
  {
    this->stream_buffer.AddStream(std::cout);
  }
  if (this->sink_mask & (1 << eLOG_SINK_STDERR))
  {
    this->stream_buffer.AddStream(std::cerr);
  }
  if (this->sink_mask & (1 << eLOG_SINK_FILE))
  {
    this->stream_buffer.AddStream(this->FileStream());
  }
  if (this->sink_mask & (1 << eLOG_SINK_COMBINED_FILE))
  {
    const tConfiguration *configuration = this;
    while (configuration->parent && configuration->sink_mask & cLOG_SINK_COMBINED_FILE_CHILD_MASK)
    {
      configuration = configuration->parent;
    }
    this->stream_buffer.AddStream(configuration->FileStream());
  }

  this->stream_buffer_ready = true;
}

//----------------------------------------------------------------------
// tConfiguration FileStream
//----------------------------------------------------------------------
std::ofstream &tConfiguration::FileStream() const
{
  if (!this->file_stream.is_open())
  {
    this->OpenFileStream();
  }

  return this->file_stream;
}

//----------------------------------------------------------------------
// tConfiguration OpenFileStream
//----------------------------------------------------------------------
void tConfiguration::OpenFileStream() const
{
  const std::string &file_name_prefix(tDomainRegistry::Instance().LogFilenamePrefix());
  if (file_name_prefix.length() == 0)
  {
    std::stringstream message;
    message << "RRLib Logging >> Prefix for log filenames not set. Can not use eLOG_SINK_FILE or eLOG_SINK_COMBINED_FILE." << std::endl
            << "                 Consider calling e.g. rrlib::logging::SetLogFilenamePrefix(basename(argv[0])) from main." << std::endl;
    throw std::runtime_error(message.str());
  }

  std::string fqdn = this->GetFullQualifiedName();
  std::string file_name(file_name_prefix + (fqdn != "." ? fqdn : "") + ".log");
  this->file_stream.open(file_name.c_str(), std::ios::out | std::ios::trunc);
  if (!this->file_stream.is_open())
  {
    std::stringstream message;
    message << "RRLib Logging >> Could not open file `" << file_name << "'!" << std::endl;
    throw std::runtime_error(message.str());
  }
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
