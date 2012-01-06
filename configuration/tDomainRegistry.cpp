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
#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
#include <iostream>
#include "rrlib/xml2_wrapper/tXMLDocument.h"
#endif

#include <algorithm>
#include <cstring>
#include <sstream>

#include <iostream>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
using namespace rrlib::xml2;
#endif

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
  // Create a string literal from the value of RRLIB_LOG_PATH and let p point to its beginning
#define __RRLIB_LOG_QUOTE_MACRO__(x) #x
#define __RRLIB_LOG_MAKE_STRING__(macro) __RRLIB_LOG_QUOTE_MACRO__(macro)
  const char *p = __RRLIB_LOG_MAKE_STRING__(RRLIB_LOG_PATH);
#undef __RRLIB_LOG_MAKE_STRING__
#undef __RRLIB_LOG_QUOTE_MACRO__

  // Fill a list of pointers to the components of our immutable RRLIB_LOG_PATH literal (no zero termination)
  this->prefix_pointers.push_back(p);
  while (*p)
  {
    if (*p == ':')
    {
      this->prefix_pointers.push_back(p + 1);
    }
    ++p;
  }
  this->prefix_pointers.push_back(p + 1);

  // From the list of pointer calculate the component sizes and a lookup table for sorted access (longest-first)
  for (size_t i = 0; i < this->prefix_pointers.size() - 1; ++i)
  {
    this->prefix_lengths.push_back(this->prefix_pointers[i + 1] - this->prefix_pointers[i] - 1);
    this->prefix_indices_sorted_by_length.push_back(i);
  }
  std::sort(this->prefix_indices_sorted_by_length.begin(), this->prefix_indices_sorted_by_length.end(),
            [this](size_t a, size_t b)
  {
    return this->prefix_lengths[a] > this->prefix_lengths[b];
  });

//  for (auto it = this->prefix_indices_sorted_by_length.begin(); it != this->prefix_indices_sorted_by_length.end(); ++it)
//  {
//    std::cout << *it << " -> " << this->prefix_lengths[*it] << std::endl;
//  }
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation destructor
//----------------------------------------------------------------------
tDomainRegistryImplementation::~tDomainRegistryImplementation()
{
  delete this->global_configuration;
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation GetConfigurationByFilename
//----------------------------------------------------------------------
const tConfiguration &tDomainRegistryImplementation::GetConfigurationByFilename(const char *filename) const
{
  bool found_prefix = false;

  // Iterate over sorted list of RRLIB_LOG_PATH components
  for (auto it = this->prefix_indices_sorted_by_length.begin(); it != this->prefix_indices_sorted_by_length.end(); ++it)
  {
    const size_t length = this->prefix_lengths[*it];

    // Skip if prefix length does not fit to path structure
    if (filename[length] != '/')
    {
      continue;
    }

    // Due to the longest-first sorted list the first match terminates our search
    if (length > 0 && std::strncmp(filename, this->prefix_pointers[*it], length) == 0)
    {
      filename = filename + length + 1;
      found_prefix = true;
      break;
    }
  }

  if (!found_prefix && this->prefix_lengths[this->prefix_indices_sorted_by_length.back()] > 0)
  {
    std::stringstream message;
    message << "'" << filename << "' is not in RRLIB_LOG_PATH";
    throw std::runtime_error(message.str());
  }

  assert(this->global_configuration);
  return this->global_configuration->GetConfigurationByFilename(filename);
}

//----------------------------------------------------------------------
// tDomainRegistryImplementation UpdateMaxDomainNameLength
//----------------------------------------------------------------------
void tDomainRegistryImplementation::UpdateMaxDomainNameLength(size_t added_domain_name_length)
{
  this->max_domain_name_length = std::max(this->max_domain_name_length, added_domain_name_length);
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
//
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
