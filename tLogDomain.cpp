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
/*!\file    tLogDomain.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief
 *
 * \b
 *
 * A few words for tLogDomain.cpp
 *
 */
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "tLogDomain.h"

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "logging/tLogDomainRegistry.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

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

//----------------------------------------------------------------------
// class tLogDomain constructors
//----------------------------------------------------------------------
tLogDomain::tLogDomain(tLogDomainConfigurationSharedPointer configuration)
    : parent(0),
    configuration(configuration),
    stream(&this->stream_buffer)
{}

tLogDomain::tLogDomain(tLogDomainConfigurationSharedPointer configuration, tLogDomain &parent)
    : parent(&parent),
    configuration(configuration),
    stream(&this->stream_buffer)
{
  this->parent->children.push_back(this);
  this->ConfigureSubTree();
}

//----------------------------------------------------------------------
// class tLogDomain destructor
//----------------------------------------------------------------------
tLogDomain::~tLogDomain()
{
  if (this->file_stream.is_open())
  {
    this->file_stream.close();
  }
}

//----------------------------------------------------------------------
// class tLogDomain ConfigureSubTree
//----------------------------------------------------------------------
void tLogDomain::ConfigureSubTree()
{
  if (this->parent && this->parent->configuration->configure_sub_tree)
  {
    *this->configuration = *this->parent->configuration;
    for (std::vector<tLogDomain *>::iterator it = this->children.begin(); it != this->children.end(); ++it)
    {
      (*it)->ConfigureSubTree();
    }
  }
}

//----------------------------------------------------------------------
// class tLogDomain SetupOutputStream
//----------------------------------------------------------------------
void tLogDomain::SetupOutputStream(eLogStreamMask mask) const
{
  this->stream_buffer.Clear();
  if (mask & static_cast<int>(eLSM_STDOUT))
  {
    this->stream_buffer.AddStream(std::cout);
  }
  if (mask & eLSM_STDERR)
  {
    this->stream_buffer.AddStream(std::cerr);
  }
  if (mask & eLSM_FILE)
  {
    this->stream_buffer.AddStream(this->OpenFileOutputStream() ? this->file_stream : std::cerr);
  }
  if (mask & eLSM_COMBINED_FILE)
  {
    const tLogDomain *domain = this;
    for (; domain->parent && domain->parent->configuration->configure_sub_tree; domain = domain->parent);
    this->stream_buffer.AddStream(domain->OpenFileOutputStream() ? domain->file_stream : std::cerr);
  }
}

//----------------------------------------------------------------------
// class tLogDomain OpenFileOutputStream
//----------------------------------------------------------------------
const bool tLogDomain::OpenFileOutputStream() const
{
  if (this->file_stream.is_open())
  {
    return true;
  }
  const std::string &file_name_prefix(tLogDomainRegistry::GetInstance().GetOutputFileNamePrefix());
  if (file_name_prefix.length() == 0)
  {
    std::cerr << "RRLib Logging >> Prefix for file names not set. Can not use eMS_FILE." << std::endl
              << "                 Consider calling tMessageDomainRegistry::GetInstance().SetOutputFileNamePrefix(basename(argv[0])) for example." << std::endl;
    return false;
  }
  std::string file_name(file_name_prefix + this->GetName() + ".log");
  this->file_stream.open(file_name.c_str(), std::ios::out | std::ios::trunc);
  if (!this->file_stream.is_open())
  {
    std::cerr << "RRLib Logging >> Could not open file `" << file_name << "'!" << std::endl;
    return false;
  }
  return true;
}

//----------------------------------------------------------------------
// class tLogDomain GetTimeString
//----------------------------------------------------------------------
const std::string tLogDomain::GetTimeString() const
{
  char time_string_buffer[32];
  time_t t = time(0);
  struct tm *temp = localtime(&t);
  strftime(time_string_buffer, sizeof(time_string_buffer), "[ %T ] ", temp);
  return time_string_buffer;
}

//----------------------------------------------------------------------
// class tLogDomain GetNameString
//----------------------------------------------------------------------
const std::string tLogDomain::GetNameString() const
{
  char name_string_buffer[128];
  snprintf(name_string_buffer, sizeof(name_string_buffer), "%-*s ", tLogDomainRegistry::GetInstance().GetMaxDomainNameLength(), this->GetName().c_str());
  return name_string_buffer;
}

//----------------------------------------------------------------------
// class tLogDomain GetLevelString
//----------------------------------------------------------------------
const std::string tLogDomain::GetLevelString(eLogLevel level) const
{
  switch (level)
  {
  case eLL_ERROR:
    return "[error]   ";
  case eLL_WARNING:
    return "[warning] ";
  case eLL_DEBUG_WARNING:
    return "[debug]   ";
  case eLL_DEBUG:
    return "[debug]   ";
  default:
    return "          ";
  }
}

//----------------------------------------------------------------------
// class tLogDomain GetLocationString
//----------------------------------------------------------------------
const std::string tLogDomain::GetLocationString(const char *file, unsigned int line) const
{
  char location_string_buffer[128];
  snprintf(location_string_buffer, sizeof(location_string_buffer), "[%s:%u] ", file, line);
  return location_string_buffer;
}

//----------------------------------------------------------------------
// class tLogDomain GetColoredOutputString
//----------------------------------------------------------------------
const std::string tLogDomain::GetControlStringForColoredOutput(eLogLevel level) const
{
  switch (level)
  {
  case eLL_ERROR:
    return "\033[;1;31m";
  case eLL_WARNING:
    return "\033[;1;34m";
  case eLL_DEBUG_WARNING:
    return "\033[;2;33m";
  case eLL_DEBUG:
    return "\033[;2;32m";
  default:
    return "\033[;0m";
  }
}
