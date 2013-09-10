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
#include <iostream>

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
    sinks(parent ? parent->sinks : std::vector<std::shared_ptr<sinks::tSink>>()),
    stream_buffer_ready(false)
{
  assert(name.length() || !parent);
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
  this->ClearSinks();
  if (sink_mask | eLOG_SINK_STDOUT)
  {
    xml::tNode node;
    node.SetAttribute("id", "stdout");
    this->AddSink(std::shared_ptr<sinks::tSink>(sinks::tSinkFactory::Instance().Create("stream", node, *this)));
  }
  if (sink_mask | eLOG_SINK_STDERR)
  {
    xml::tNode node;
    node.SetAttribute("id", "stderr");
    this->AddSink(std::shared_ptr<sinks::tSink>(sinks::tSinkFactory::Instance().Create("stream", node, *this)));
  }
  if (sink_mask | eLOG_SINK_FILE)
  {
    std::cerr << "INFO: The meaning of this sink changed to be the same as combined file. There will be one file for the whole subtree starting at " << this->GetFullQualifiedName() << std::endl;
    xml::tNode node;
    this->AddSink(std::shared_ptr<sinks::tSink>(sinks::tSinkFactory::Instance().Create("file", node, *this)));
  }
  if (sink_mask | eLOG_SINK_COMBINED_FILE)
  {
    xml::tNode node;
    this->AddSink(std::shared_ptr<sinks::tSink>(sinks::tSinkFactory::Instance().Create("file", node, *this)));
  }
}

//----------------------------------------------------------------------
// tConfiguration ClearSinks
//----------------------------------------------------------------------
void tConfiguration::ClearSinks()
{
  this->sinks.clear();
  this->stream_buffer_ready = false;

  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->ClearSinks();
  }
}

//----------------------------------------------------------------------
// tConfiguration AddSink
//----------------------------------------------------------------------
void tConfiguration::AddSink(std::shared_ptr<sinks::tSink> sink)
{
  this->sinks.push_back(sink);
  this->stream_buffer_ready = false;

  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    (*it)->AddSink(sink);
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
    return this->GetChild(default_context, domain_name, std::strlen(domain_name));
  }

  return this->GetChild(default_context, domain_name, delimiter - domain_name).GetConfigurationByName(default_context, delimiter + 1);
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

  return this->GetChild(default_context, filename, delimiter - filename).GetConfigurationByFilename(default_context, delimiter + 1);
}

//----------------------------------------------------------------------
// tConfiguration GetChild
//----------------------------------------------------------------------
const tConfiguration &tConfiguration::GetChild(const tDefaultConfigurationContext &default_context, const char *name, size_t length) const
{
  tConfiguration *configuration = this->FindChild(name, length);
  if (!configuration)
  {
    std::lock_guard<std::mutex> lock(this->children_mutex);
    configuration = this->FindChild(name, length);

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
      this->children.insert(this->FindInsertionPoint(length), configuration);
    }
  }

  return *configuration;
}

//----------------------------------------------------------------------
// tConfiguration FindChild
//----------------------------------------------------------------------
tConfiguration *tConfiguration::FindChild(const char *name, size_t length) const
{
  // Iterate over sorted list of children (must be sorted longest name first to work)
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    const size_t current_length = (*it)->Name().length();

    // Skip too long names
    if (current_length > length)
    {
      continue;
    }

    // Compare names of matching length
    if (current_length == length)
    {
      if (std::strncmp(name, (*it)->Name().c_str(), length) == 0)
      {
        return *it;
      }
      continue;
    }

    // Shorter names start -> can not be found anymore
    break;
  }
  return NULL;
}

//----------------------------------------------------------------------
// tConfiguration FindInsertionPoint
//----------------------------------------------------------------------
std::list<tConfiguration *>::iterator tConfiguration::FindInsertionPoint(size_t length) const
{
  // Iterate over sorted list of children (must be sorted longest name first to work)
  for (auto it = this->children.begin(); it != this->children.end(); ++it)
  {
    const size_t current_length = (*it)->Name().length();

    // Shorter names start -> insert new configuration before
    if (current_length < length)
    {
      return it;
    }
  }
  return this->children.end();
}

//----------------------------------------------------------------------
// tConfiguration PrepareStreamBuffer
//----------------------------------------------------------------------
void tConfiguration::PrepareStreamBuffer() const
{
  this->stream_buffer.Clear();
  for (auto sink = this->sinks.begin(); sink != this->sinks.end(); ++sink)
  {
    this->stream_buffer.AddSink((*sink)->GetStreamBuffer());
  }

  this->stream_buffer_ready = true;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
