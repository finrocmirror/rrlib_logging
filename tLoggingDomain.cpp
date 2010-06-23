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
    null_stream("/dev/null")
{
  assert(this->null_stream.is_open());
}

tLoggingDomain::tLoggingDomain(tLoggingDomainConfigurationSharedPointer configuration, tLoggingDomain &parent)
    : parent(&parent),
    configuration(configuration),
    null_stream("/dev/null")
{
  assert(this->null_stream.is_open());
  this->parent->children.push_back(this);
  this->ConfigureSubTree();
}

//----------------------------------------------------------------------
// class tLoggingDomain destructor
//----------------------------------------------------------------------
tLoggingDomain::~tLoggingDomain()
{
  this->null_stream.close();

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
// class tLoggingDomain GetOutputStream
//----------------------------------------------------------------------
std::ostream &tLoggingDomain::GetOutputStream() const
{
  switch (this->configuration->stream_id)
  {
  case eMS_NULL:
    return this->null_stream;
  case eMS_STDOUT:
    return std::cout;
  case eMS_STDERR:
    return std::cerr;
  case eMS_FILE:
    return this->OpenFileOutputStream() ? this->file_stream : std::cerr;
  case eMS_COMBINED_FILE:
    if (this->parent && this->parent->configuration->configure_sub_tree)
    {
      return this->parent->GetOutputStream();
    }
    return this->OpenFileOutputStream() ? this->file_stream : std::cerr;
  default:
    std::cerr << "RRlib Messages: Stream ID " << this->configuration->stream_id << " not supported. Using eMS_STDERR instead." << std::endl;
    return std::cerr;
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
    std::cerr << "RRLib Messages >> Prefix for file names not set. Can not use eMS_FILE." << std::endl
              << "                  Consider calling tMessageDomainRegistry::GetInstance().SetOutputFileNamePrefix(basename(argv[0])) for example." << std::endl;
    return false;
  }
  std::string file_name(file_name_prefix + "." + this->GetName() + ".log");
  this->file_stream.open(file_name.c_str(), std::ios::out | std::ios::trunc);
  if (!this->file_stream.is_open())
  {
    std::cerr << "RRLib Messages >> Could not open file `" << file_name << "'!" << std::endl;
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
  case eML_VERBOSE:
    return "[verbose] ";
  case eML_LOW:
    return "[low]     ";
  case eML_MEDIUM:
    return "[medium]  ";
  case eML_HIGH:
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
const std::string tLoggingDomain::GetColoredOutputString(eLogLevel level) const
{
  switch (level)
  {
  case eML_VERBOSE:
    return "\033[;2;32m";
  case eML_LOW:
    return "\033[;2;33m";
  case eML_MEDIUM:
    return "\033[;1;34m";
  case eML_HIGH:
    return "\033[;1;31m";
  default:
    return "\033[;0m";
  }
}
