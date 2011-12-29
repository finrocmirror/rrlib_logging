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
 * \author  Max Reichardt
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/tLogDomain.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/tLogDomainRegistry.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::logging;

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tLogDomain constructors
//----------------------------------------------------------------------
tLogDomain::tLogDomain(tLogDomainConfigurationSharedPointer configuration)
    : parent(0),
    configuration(configuration),
    mutex(GetMutex())
{}

tLogDomain::tLogDomain(tLogDomainConfigurationSharedPointer configuration, tLogDomain &parent)
    : parent(&parent),
    configuration(configuration),
    mutex(GetMutex())
{
  this->parent->children.push_back(this);
  this->ConfigureSubTree();
}

//----------------------------------------------------------------------
// tLogDomain destructor
//----------------------------------------------------------------------
tLogDomain::~tLogDomain()
{
  if (this->file_stream.is_open())
  {
    this->file_stream.close();
  }
}

//----------------------------------------------------------------------
// tLogDomain ConfigureSubTree
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
// tLogDomain SetupOutputStream
//----------------------------------------------------------------------
void tLogDomain::SetupOutputStream(int mask) const
{
  this->stream_buffer.Clear();
  if (mask & (1 << eLS_STDOUT))
  {
    this->stream_buffer.AddStream(std::cout);
  }
  if (mask & (1 << eLS_STDERR))
  {
    this->stream_buffer.AddStream(std::cerr);
  }
  if (mask & (1 << eLS_FILE))
  {
    this->stream_buffer.AddStream(this->OpenFileOutputStream() ? this->file_stream : std::cerr);
  }
  if (mask & (1 << eLS_COMBINED_FILE))
  {
    const tLogDomain *domain = this;
    for (; domain->parent && domain->parent->configuration->configure_sub_tree; domain = domain->parent);
    this->stream_buffer.AddStream(domain->OpenFileOutputStream() ? domain->file_stream : std::cerr);
  }
}

//----------------------------------------------------------------------
// tLogDomain OpenFileOutputStream
//----------------------------------------------------------------------
const bool tLogDomain::OpenFileOutputStream() const
{
  if (this->file_stream.is_open())
  {
    return true;
  }
  const std::string &file_name_prefix(tLogDomainRegistry::GetInstance()->GetOutputFileNamePrefix());
  if (file_name_prefix.length() == 0)
  {
    std::cerr << "RRLib Logging >> Prefix for file names not set. Can not use eMS_FILE." << std::endl
              << "                 Consider calling tLogDomainRegistry::GetInstance()->SetOutputFileNamePrefix(basename(argv[0])) for example." << std::endl;
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
// tLogDomain GetTimeString
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
// tLogDomain GetNameString
//----------------------------------------------------------------------
const std::string tLogDomain::GetNameString() const
{
  char name_string_buffer[128];
  snprintf(name_string_buffer, sizeof(name_string_buffer), "%-*s ", static_cast<int>((tLogDomainRegistry::GetInstance()->GetPadPrefixColumns() ? tLogDomainRegistry::GetInstance()->GetMaxDomainNameLength() : 0)), this->GetName().c_str());
  return name_string_buffer;
}

//----------------------------------------------------------------------
// tLogDomain GetLevelString
//----------------------------------------------------------------------
const std::string tLogDomain::GetLevelString(tLogLevel level) const
{
  const char *level_name = 0;
  switch (level)
  {
  case eLL_ERROR:
    level_name = "[error]";
    break;
  case eLL_WARNING:
    level_name = "[warning]";
    break;
  case eLL_DEBUG_WARNING:
    level_name = "[debug]";
    break;
  case eLL_DEBUG:
    level_name = "[debug]";
    break;
  case eLL_DEBUG_VERBOSE_1:
    level_name = "[verbose]";
    break;
  case eLL_DEBUG_VERBOSE_2:
    level_name = "[verbose]";
    break;
  case eLL_DEBUG_VERBOSE_3:
    level_name = "[verbose]";
    break;
  default:
    level_name = "";
    break;
  }
  char name_string_buffer[128];
  snprintf(name_string_buffer, sizeof(name_string_buffer), "%-*s ", (tLogDomainRegistry::GetInstance()->GetPadPrefixColumns() ? 9 : 0), level_name);
  return name_string_buffer;
}

//----------------------------------------------------------------------
// tLogDomain GetLocationString
//----------------------------------------------------------------------
const std::string tLogDomain::GetLocationString(const char *file, unsigned int line) const
{
  char location_string_buffer[128];
  snprintf(location_string_buffer, sizeof(location_string_buffer), "[%s:%u] ", file, line);
  return location_string_buffer;
}

//----------------------------------------------------------------------
// tLogDomain GetColoredOutputString
//----------------------------------------------------------------------
void tLogDomain::SetupOutputStreamColor(tLogLevel level) const
{
  switch (level)
  {
  case eLL_ERROR:
    this->stream_buffer.SetColor(eLSBE_BOLD, eLSBC_RED);
    break;
  case eLL_WARNING:
    this->stream_buffer.SetColor(eLSBE_BOLD, eLSBC_BLUE);
    break;
  case eLL_DEBUG_WARNING:
    this->stream_buffer.SetColor(eLSBE_DARK, eLSBC_YELLOW);
    break;
  case eLL_DEBUG:
    this->stream_buffer.SetColor(eLSBE_DARK, eLSBC_GREEN);
    break;
  case eLL_DEBUG_VERBOSE_1:
    this->stream_buffer.SetColor(eLSBE_REGULAR, eLSBC_CYAN);
    break;
  case eLL_DEBUG_VERBOSE_2:
    this->stream_buffer.SetColor(eLSBE_REGULAR, eLSBC_CYAN);
    break;
  case eLL_DEBUG_VERBOSE_3:
    this->stream_buffer.SetColor(eLSBE_REGULAR, eLSBC_CYAN);
    break;
  default:
    ;
  }
}
