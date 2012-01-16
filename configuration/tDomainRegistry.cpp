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
/*!\file    tDomainRegistry.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/configuration/tDomainRegistry.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <sstream>

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

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tDomainRegistryImplementation constructors
//----------------------------------------------------------------------
tDomainRegistryImplementation::tDomainRegistryImplementation()
    : global_configuration(new tConfiguration(0, "")),
    max_domain_name_length(0),
    pad_prefix_columns(true),
    pad_multi_line_messages(true)
{
  // Look at the environment variable RRLIB_LOGGING_PATH or a default value and let p point to its beginning
  const char *rrlib_logging_path = std::getenv("RRLIB_LOGGING_PATH");
  if (rrlib_logging_path == NULL)
  {
    rrlib_logging_path = "/usr/include";
  }

  // Fill a list of pointers to the components of our immutable rrlib_logging_path (no zero termination)
  const char *begin_entry = rrlib_logging_path;
  const char *end_entry = std::strchr(begin_entry, ':');
  while (end_entry)
  {
    this->rrlib_logging_path_entries.push_back(std::string(begin_entry, end_entry - begin_entry));
    begin_entry = end_entry + 1;
    end_entry = std::strchr(begin_entry, ':');
  }
  this->rrlib_logging_path_entries.push_back(begin_entry);

  // For efficient best-fit lookup sort entries by length (longest-first)
  std::sort(this->rrlib_logging_path_entries.begin(), this->rrlib_logging_path_entries.end(),
            [](const std::string &a, const std::string &b)
  {
    return a.length() > b.length();
  });
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation destructor
//----------------------------------------------------------------------
tDomainRegistryImplementation::~tDomainRegistryImplementation()
{
  delete this->global_configuration;
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation GetConfiguration
//----------------------------------------------------------------------
const tConfiguration &tDomainRegistryImplementation::GetConfiguration(const char *filename, const char *domain_name)
{
  if (domain_name)
  {
    assert(*domain_name && "domain_name must be zero or not empty!");
    if (domain_name[0] == '.')
    {
      assert(this->global_configuration);
      return (domain_name[1] == 0) ? *this->global_configuration : this->global_configuration->GetConfigurationByName(domain_name + 1);
    }
    return tDomainRegistry::Instance().GetConfigurationByFilename(filename).GetConfigurationByName(domain_name);
  }
  return tDomainRegistry::Instance().GetConfigurationByFilename(filename);
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation SetLogFilenamePrefix
//----------------------------------------------------------------------
void tDomainRegistryImplementation::SetLogFilenamePrefix(const std::string &log_filename_prefix)
{
  assert(log_filename_prefix.length() > 0);
  this->log_filename_prefix = log_filename_prefix;
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation LogFilenamePrefix
//----------------------------------------------------------------------
const std::string &tDomainRegistryImplementation::LogFilenamePrefix() const
{
  if (this->log_filename_prefix.length() == 0)
  {
    throw std::logic_error("tDomainRegistryImplementation::LogFilenamePrefix() called before filename prefix was set. Consider calling rrlib::logging::SetLogFilenamePrefix(basename(argv[0])) from your main function.");
  }
  return this->log_filename_prefix;
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation UpdateMaxDomainNameLength
//----------------------------------------------------------------------
void tDomainRegistryImplementation::UpdateMaxDomainNameLength(size_t added_domain_name_length)
{
  this->max_domain_name_length = std::max(this->max_domain_name_length, added_domain_name_length);
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation GetConfigurationByFilename
//----------------------------------------------------------------------
const tConfiguration &tDomainRegistryImplementation::GetConfigurationByFilename(const char *filename) const
{
  bool found_prefix = false;

  // Iterate over sorted list of RRLIB_LOG_PATH components
  for (auto it = this->rrlib_logging_path_entries.begin(); it != this->rrlib_logging_path_entries.end(); ++it)
  {
    const size_t length = it->length();

    // Skip if prefix length does not fit to path structure
    if (filename[length] != '/')
    {
      continue;
    }

    // Due to the longest-first sorted list the first match terminates our search
    if (std::strncmp(filename, it->c_str(), length) == 0)
    {
      filename = filename + length + 1;
      found_prefix = true;
      break;
    }
  }

  if (!found_prefix && !this->rrlib_logging_path_entries.back().empty())
  {
    std::stringstream message;
    message << "'" << filename << "' is not in RRLIB_LOGGING_PATH";
    throw std::runtime_error(message.str());
  }

  assert(this->global_configuration);
  return this->global_configuration->GetConfigurationByFilename(filename);
}


//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
