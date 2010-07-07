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
/*!\file    tLoggingDomain.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief
 *
 * \b
 *
 * A few words for tLoggingDomain.cpp
 *
 */
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "tLoggingDomain.h"

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "tLoggingDomainRegistry.h"

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
// class tLoggingDomain constructors
//----------------------------------------------------------------------
tLoggingDomain::tLoggingDomain(tLoggingDomainConfigurationSharedPointer configuration)
    : parent(0),
    configuration(configuration),
    stream(&this->stream_buffer)
{}

tLoggingDomain::tLoggingDomain(tLoggingDomainConfigurationSharedPointer configuration, tLoggingDomain &parent)
    : parent(&parent),
    configuration(configuration),
    stream(&this->stream_buffer)
{
  this->parent->children.push_back(this);
  this->ConfigureSubTree();
}

//----------------------------------------------------------------------
// class tLoggingDomain destructor
//----------------------------------------------------------------------
tLoggingDomain::~tLoggingDomain()
{
  if (this->file_stream.is_open())
  {
    this->file_stream.close();
  }
}

//----------------------------------------------------------------------
// class tLoggingDomain ConfigureSubTree
//----------------------------------------------------------------------
void tLoggingDomain::ConfigureSubTree()
{
  if (this->parent && this->parent->configuration->configure_sub_tree)
  {
    *this->configuration = *this->parent->configuration;
    for (std::vector<tLoggingDomain *>::iterator it = this->children.begin(); it != this->children.end(); ++it)
    {
      (*it)->ConfigureSubTree();
    }
  }
}

//----------------------------------------------------------------------
// class tLoggingDomain SetupOutputStream
//----------------------------------------------------------------------
void tLoggingDomain::SetupOutputStream(eLogStreamMask mask) const
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
    const tLoggingDomain *domain = this;
    for (; domain->parent && domain->parent->configuration->configure_sub_tree; domain = domain->parent);
    this->stream_buffer.AddStream(domain->OpenFileOutputStream() ? domain->file_stream : std::cerr);
  }
}

//----------------------------------------------------------------------
// class tLoggingDomain OpenFileOutputStream
//----------------------------------------------------------------------
const bool tLoggingDomain::OpenFileOutputStream() const
{
  if (this->file_stream.is_open())
  {
    return true;
  }
  const std::string &file_name_prefix(tLoggingDomainRegistry::GetInstance().GetOutputFileNamePrefix());
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
// class tLoggingDomain GetTimeString
//----------------------------------------------------------------------
const std::string tLoggingDomain::GetTimeString() const
{
  char time_string_buffer[32];
  time_t t = time(0);
  struct tm *temp = localtime(&t);
  strftime(time_string_buffer, sizeof(time_string_buffer), "[ %T ] ", temp);
  return time_string_buffer;
}

//----------------------------------------------------------------------
// class tLoggingDomain GetNameString
//----------------------------------------------------------------------
const std::string tLoggingDomain::GetNameString() const
{
  char name_string_buffer[128];
  snprintf(name_string_buffer, sizeof(name_string_buffer), "%-*s ", tLoggingDomainRegistry::GetInstance().GetMaxDomainNameLength(), this->GetName().c_str());
  return name_string_buffer;
}

//----------------------------------------------------------------------
// class tLoggingDomain GetLevelString
//----------------------------------------------------------------------
const std::string tLoggingDomain::GetLevelString(eLogLevel level) const
{
  switch (level)
  {
  case eLL_VERBOSE:
    return "[verbose] ";
  case eLL_LOW:
    return "[low]     ";
  case eLL_MEDIUM:
    return "[medium]  ";
  case eLL_HIGH:
    return "[high]    ";
  default:
    return "          ";
  }
}

//----------------------------------------------------------------------
// class tLoggingDomain GetLocationString
//----------------------------------------------------------------------
const std::string tLoggingDomain::GetLocationString(const char *file, unsigned int line) const
{
  char location_string_buffer[128];
  snprintf(location_string_buffer, sizeof(location_string_buffer), "[%s:%u] ", file, line);
  return location_string_buffer;
}

//----------------------------------------------------------------------
// class tLoggingDomain GetColoredOutputString
//----------------------------------------------------------------------
const std::string tLoggingDomain::GetControlStringForColoredOutput(eLogLevel level) const
{
  switch (level)
  {
  case eLL_VERBOSE:
    return "\033[;2;32m";
  case eLL_LOW:
    return "\033[;2;33m";
  case eLL_MEDIUM:
    return "\033[;1;34m";
  case eLL_HIGH:
    return "\033[;1;31m";
  default:
    return "\033[;0m";
  }
}
