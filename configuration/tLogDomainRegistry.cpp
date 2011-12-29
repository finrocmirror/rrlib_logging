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
/*!\file    tLogDomainRegistry.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/configuration/tLogDomainRegistry.h"

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
// tLogDomainRegistryImplementation constructors
//----------------------------------------------------------------------
tLogDomainRegistryImplementation::tLogDomainRegistryImplementation()
    : global_configuration(new tConfiguration(0, ""))
//    : max_domain_name_length(0),
//    pad_prefix_columns(true),
//    pad_multi_line_messages(true)
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


//  this->domain_configurations.push_back(tConfigurationSharedPointer(new tConfiguration(".")));
//  this->domains.push_back(std::shared_ptr<tLogDomain>(new tLogDomain(this->domain_configurations.back())));
}

//----------------------------------------------------------------------
// tLogDomainRegistryImplementation destructor
//----------------------------------------------------------------------
tLogDomainRegistryImplementation::~tLogDomainRegistryImplementation()
{
  delete this->global_configuration;
}

//----------------------------------------------------------------------
// tLogDomainRegistryImplementation GetConfigurationByFilename
//----------------------------------------------------------------------
const tConfiguration &tLogDomainRegistryImplementation::GetConfigurationByFilename(const char *filename) const
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

////----------------------------------------------------------------------
//// tLogDomainRegistry GetSubDomain
////----------------------------------------------------------------------
//tLogDomainSharedPointer tLogDomainRegistry::GetSubDomain(const std::string &name, tLogDomainSharedPointer parent)
//{
//  assert(name.length() > 0);
//  assert(parent != tLogDomainSharedPointer());
//  const std::string full_qualified_domain_name(parent->GetName() + (parent->parent ? "." : "") + name);
//  size_t i = this->GetDomainIndexByName(full_qualified_domain_name);
//  if (i == this->domains.size())
//  {
//    tConfigurationSharedPointer configuration(this->GetConfigurationByName(full_qualified_domain_name));
//    this->domains.push_back(std::shared_ptr<tLogDomain>(new tLogDomain(configuration, *const_cast<tLogDomain *>(parent.get()))));
//    return this->domains.back();
//  }
//  return this->domains[i];
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainConfiguresSubTree
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainConfiguresSubTree(const std::string &name, bool value)
//{
//  tConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
//  configuration->configure_sub_tree = value;
//  this->PropagateDomainConfigurationToChildren(name);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainPrintsTime
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainPrintsTime(const std::string &name, bool value)
//{
//  tConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
//  configuration->print_time = value;
//  this->PropagateDomainConfigurationToChildren(name);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainPrintsName
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainPrintsName(const std::string &name, bool value)
//{
//  tConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
//  configuration->print_name = value;
//  this->PropagateDomainConfigurationToChildren(name);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainPrintsLevel
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainPrintsLevel(const std::string &name, bool value)
//{
//  tConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
//  configuration->print_level = value;
//  this->PropagateDomainConfigurationToChildren(name);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainPrintsLocation
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainPrintsLocation(const std::string &name, bool value)
//{
//  tConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
//  configuration->print_location = value;
//  this->PropagateDomainConfigurationToChildren(name);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainMaxMessageLevel
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainMaxMessageLevel(const std::string &name, tLogLevel value)
//{
//  tLogLevel effective_value = std::max(value, eLL_ERROR);
//
//#ifdef _RRLIB_LOGGING_LESS_OUTPUT_
//  effective_value = std::min(value, eLL_DEBUG);
//#endif
//
//  tConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
//  configuration->max_message_level = effective_value;
//  this->PropagateDomainConfigurationToChildren(name);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainSink
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainSink(const std::string &name, tLogSink sink_1, tLogSink sink_2, tLogSink sink_3, tLogSink sink_4)
//{
//  int mask = 0;
//  mask |= (sink_1 != eLS_DIMENSION) ? (1 << sink_1) : 0;
//  mask |= (sink_2 != eLS_DIMENSION) ? (1 << sink_2) : 0;
//  mask |= (sink_3 != eLS_DIMENSION) ? (1 << sink_3) : 0;
//  mask |= (sink_4 != eLS_DIMENSION) ? (1 << sink_4) : 0;
//  this->SetDomainSinkMask(name, mask);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry SetDomainSinkMask
////----------------------------------------------------------------------
//void tLogDomainRegistry::SetDomainSinkMask(const std::string &name, int mask)
//{
//  tConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
//  configuration->sink_mask = mask;
//  this->PropagateDomainConfigurationToChildren(name);
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry GetDomainIndexByName
////----------------------------------------------------------------------
//const size_t tLogDomainRegistry::GetDomainIndexByName(const std::string &name) const
//{
//  for (size_t i = 0; i < this->domains.size(); ++i)
//  {
//    if (this->domains[i]->GetName() == name)
//    {
//      return i;
//    }
//  }
//  return this->domains.size();
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry GetConfigurationByName
////----------------------------------------------------------------------
//tConfigurationSharedPointer tLogDomainRegistry::GetConfigurationByName(const std::string &name)
//{
//  for (std::vector<tConfigurationSharedPointer>::iterator it = this->domain_configurations.begin(); it != this->domain_configurations.end(); ++it)
//  {
//    if ((*it)->name == name)
//    {
//      return *it;
//    }
//  }
//  this->domain_configurations.push_back(tConfigurationSharedPointer(new tConfiguration(name)));
//  this->max_domain_name_length = std::max(this->max_domain_name_length, name.length());
//  return this->domain_configurations.back();
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry PropagateDomainConfigurationToChildren
////----------------------------------------------------------------------
//void tLogDomainRegistry::PropagateDomainConfigurationToChildren(const std::string &name)
//{
//  size_t i = this->GetDomainIndexByName(name);
//  if (i != this->domains.size())
//  {
//    for (std::vector<tLogDomain *>::iterator it = this->domains[i]->children.begin(); it != this->domains[i]->children.end(); ++it)
//    {
//      (*it)->ConfigureSubTree();
//    }
//  }
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry ConfigureFromFile
////----------------------------------------------------------------------
//bool tLogDomainRegistry::ConfigureFromFile(const std::string &file_name)
//{
//#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
//  try
//  {
//    tXMLDocument document(file_name);
//    return this->ConfigureFromXMLNode(document.GetRootNode());
//  }
//  catch (const tXML2WrapperException &e)
//  {
//    std::cerr << "RRLib Logging: tLogDomainRegistry::ConfigureFromFile >> " << e.what() << std::endl;
//    return false;
//  }
//#else
//  std::cerr << "RRLib Logging: tLogDomainRegistry::ConfigureFromFile >> XML support not available due to missing rrlib_mca2_wrapper." << std::endl;
//  return false;
//#endif
//}
//
//#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
//
////----------------------------------------------------------------------
//// tLogDomainRegistry ConfigureFromXMLNode
////----------------------------------------------------------------------
//bool tLogDomainRegistry::ConfigureFromXMLNode(const tXMLNode &node)
//{
//  if (node.GetName() != "rrlib_logging")
//  {
//    std::cerr << "RRLib Logging: tLogDomainRegistry::ConfigureFromXMLNode >> Unexpected content (Not an rrlib_logging tree)" << std::endl;
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
//    std::cerr << "RRLib Logging: tLogDomainRegistry::ConfigureFromXMLNode >> " << e.what() << std::endl;
//    return false;
//  }
//
//  return true;
//}
//
////----------------------------------------------------------------------
//// tLogDomainRegistry AddConfigurationFromXMLNode
////----------------------------------------------------------------------
//bool tLogDomainRegistry::AddConfigurationFromXMLNode(const tXMLNode &node, const std::string &parent_name)
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
//        std::cerr << "RRLib Logging: tLogDomainRegistry::AddConfigurationFromXMLNode >> Sink already configured in domain element!" << std::endl;
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
