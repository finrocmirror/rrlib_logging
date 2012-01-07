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

#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
#include "rrlib/xml2_wrapper/tXMLDocument.h"
#endif

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
    message << "'" << filename << "' is not in RRLIB_LOG_PATH";
    throw std::runtime_error(message.str());
  }

  assert(this->global_configuration);
  return this->global_configuration->GetConfigurationByFilename(filename);
}

////----------------------------------------------------------------------
//// tDomainRegistry ConfigureFromFile
////----------------------------------------------------------------------
//bool tDomainRegistry::ConfigureFromFile(const std::string &file_name)
//{
//#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
//  try
//  {
//    tXMLDocument document(file_name);
//    return this->ConfigureFromXMLNode(document.GetRootNode());
//  }
//  catch (const tXML2WrapperException &e)
//  {
//    std::cerr << "RRLib Logging: tDomainRegistry::ConfigureFromFile >> " << e.what() << std::endl;
//    return false;
//  }
//#else
//  std::cerr << "RRLib Logging: tDomainRegistry::ConfigureFromFile >> XML support not available due to missing rrlib_mca2_wrapper." << std::endl;
//  return false;
//#endif
//}



//#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
//
////----------------------------------------------------------------------
//// tDomainRegistry ConfigureFromXMLNode
////----------------------------------------------------------------------
//bool tDomainRegistry::ConfigureFromXMLNode(const tXMLNode &node)
//{
//  if (node.GetName() != "rrlib_logging")
//  {
//    std::cerr << "RRLib Logging: tDomainRegistry::ConfigureFromXMLNode >> Unexpected content (Not an rrlib_logging tree)" << std::endl;
//    return false;
//  }
//
//  try
//  {
//    if (node.HasAttribute("pad_prefix_columns"))
//    {
//      this->pad_prefix_columns = node.GetBoolAttribute("pad_prefix_columns");
//    }
//    if (node.HasAttribute("pad_multi_line_messages"))
//    {
//      this->pad_multi_line_messages = node.GetBoolAttribute("pad_multi_line_messages");
//    }
//
//    for (tXMLNode::const_iterator it = node.GetChildrenBegin(); it != node.GetChildrenEnd(); ++it)
//    {
//      if (it->GetName() == "domain")
//      {
//        if (!this->AddConfigurationFromXMLNode(*it))
//        {
//          return false;
//        }
//      }
//    }
//  }
//  catch (const tXML2WrapperException &e)
//  {
//    std::cerr << "RRLib Logging: tDomainRegistry::ConfigureFromXMLNode >> " << e.what() << std::endl;
//    return false;
//  }
//
//  return true;
//}
//
////----------------------------------------------------------------------
//// tDomainRegistry AddConfigurationFromXMLNode
////----------------------------------------------------------------------
//bool tDomainRegistry::AddConfigurationFromXMLNode(const tXMLNode &node, const std::string &parent_name)
//{
//  static const char *level_names_init[eLL_DIMENSION - eLL_ERROR] = { "error", "warning", "debug_warning", "debug", "debug_verbose_1", "debug_verbose_2", "debug_verbose_3" };
//  static const std::vector<std::string> level_names(level_names_init, level_names_init + eLL_DIMENSION - eLL_ERROR);
//  // FIXME: with c++0x this can be static const std::vector<std::string> level_names = { "error", "warning", "debug_warning", "debug", "debug_verbose_1", "debug_verbose_2", "debug_verbose_3" };
//
//  static const char *sink_names_init[eLS_DIMENSION] = { "stdout", "stderr", "file", "combined_file" };
//  static const std::vector<std::string> sink_names(sink_names_init, sink_names_init + eLS_DIMENSION);
//  // FIXME: with c++0x this can be static const std::vector<std::string> stream_names = { "stdout", "stderr", "file", "combined_file" };
//
//  assert(node.GetName() == "domain");
//
//  const std::string prefix((parent_name == "." ? "" : parent_name) + ".");
//  const std::string node_name(node.GetStringAttribute("name"));
//  const std::string name(prefix + (!parent_name.length() && node_name == "global" ? "" : node_name));
//
//  if (node.HasAttribute("configures_sub_tree"))
//  {
//    this->SetDomainConfiguresSubTree(name, node.GetBoolAttribute("configures_sub_tree"));
//  }
//
//  if (node.HasAttribute("print_time"))
//  {
//    this->SetDomainPrintsTime(name, node.GetBoolAttribute("print_time"));
//  }
//
//  if (node.HasAttribute("print_name"))
//  {
//    this->SetDomainPrintsName(name, node.GetBoolAttribute("print_name"));
//  }
//
//  if (node.HasAttribute("print_level"))
//  {
//    this->SetDomainPrintsLevel(name, node.GetBoolAttribute("print_level"));
//  }
//
//  if (node.HasAttribute("print_location"))
//  {
//    this->SetDomainPrintsLocation(name, node.GetBoolAttribute("print_location"));
//  }
//
//  if (node.HasAttribute("max_level"))
//  {
//    this->SetDomainMaxMessageLevel(name, static_cast<tLogLevel>(eLL_ERROR + node.GetEnumAttribute<tLogLevel>("max_level", level_names)));
//  }
//
//  bool sinks_configured = false;
//  if (node.HasAttribute("sink"))
//  {
//    sinks_configured = true;
//    this->SetDomainSink(name, node.GetEnumAttribute<tLogSink>("sink", sink_names));
//  }
//
//  int sink_mask = 0;
//  for (tXMLNode::const_iterator it = node.GetChildrenBegin(); it != node.GetChildrenEnd(); ++it)
//  {
//    if (it->GetName() == "sink")
//    {
//      if (sinks_configured)
//      {
//        std::cerr << "RRLib Logging: tDomainRegistry::AddConfigurationFromXMLNode >> Sink already configured in domain element!" << std::endl;
//        return false;
//      }
//      sink_mask |= 1 << it->GetEnumAttribute<tLogSink>("output", sink_names);
//    }
//  }
//  if (sink_mask != 0)
//  {
//    this->SetDomainSinkMask(name, sink_mask);
//  }
//
//  for (tXMLNode::const_iterator it = node.GetChildrenBegin(); it != node.GetChildrenEnd(); ++it)
//  {
//    if (it->GetName() == "domain")
//    {
//      if (!this->AddConfigurationFromXMLNode(*it, name))
//      {
//        return false;
//      }
//    }
//  }
//
//  return true;
//}
//
//#endif

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
