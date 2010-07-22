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
 * \brief
 *
 * \b
 *
 * A few words for tLogDomainRegistry.cpp
 *
 */
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "tLogDomainRegistry.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
#include <iostream>
#include "xml2_wrapper/tXMLDocument.h"
#endif

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "logging/tLogDomain.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::logging;

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
using namespace rrlib::xml2;
#endif

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
// class tLogDomainRegistry constructors
//----------------------------------------------------------------------
tLogDomainRegistry::tLogDomainRegistry()
{
  this->domain_configurations.push_back(tLogDomainConfigurationSharedPointer(new tLogDomainConfiguration(".")));
  this->domain_configurations.back()->enabled = true;
  this->domains.push_back(std::tr1::shared_ptr<tLogDomain>(new tLogDomain(this->domain_configurations.back())));
}

//----------------------------------------------------------------------
// class tLogDomainRegistry GetInstance
//----------------------------------------------------------------------
tLogDomainRegistry &tLogDomainRegistry::GetInstance()
{
  static tLogDomainRegistry instance;
  return instance;
}

//----------------------------------------------------------------------
// class tLogDomainRegistry GetSubDomain
//----------------------------------------------------------------------
tLogDomainSharedPointer tLogDomainRegistry::GetSubDomain(const std::string &name, tLogDomainSharedPointer parent)
{
  assert(name.length() > 0);
  assert(parent != tLogDomainSharedPointer());
  const std::string full_qualified_domain_name(parent->GetName() + (parent->parent ? "." : "") + name);
  size_t i = this->GetDomainIndexByName(full_qualified_domain_name);
  if (i == this->domains.size())
  {
    tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(full_qualified_domain_name));
    this->domains.push_back(std::tr1::shared_ptr<tLogDomain>(new tLogDomain(configuration, *const_cast<tLogDomain *>(parent.get()))));
    return this->domains.back();
  }
  return this->domains[i];
}

//----------------------------------------------------------------------
// class tLogDomainRegistry GetMaxDomainNameLength
//----------------------------------------------------------------------
size_t tLogDomainRegistry::GetMaxDomainNameLength() const
{
  size_t result = 0;
  for (std::vector<tLogDomainConfigurationSharedPointer>::const_iterator it = this->domain_configurations.begin(); it != this->domain_configurations.end(); ++it)
  {
    result = std::max(result, (*it)->name.length());
  }
  return result;
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainConfiguresSubTree
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainConfiguresSubTree(const std::string &name, bool value)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->configure_sub_tree = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainIsEnabled
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainIsEnabled(const std::string &name, bool value)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->enabled = value;
  this->PropagateDomainConfigurationToChildren(name);
}
//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainPrintsTime
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainPrintsTime(const std::string &name, bool value)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_time = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainPrintsName
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainPrintsName(const std::string &name, bool value)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_name = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainPrintsLevel
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainPrintsLevel(const std::string &name, bool value)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_level = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainPrintsLocation
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainPrintsLocation(const std::string &name, bool value)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_location = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainMaxMessageLevel
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainMaxMessageLevel(const std::string &name, eLogLevel value)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->max_message_level = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainStream
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainStream(const std::string &name, eLogStream stream_1, eLogStream stream_2, eLogStream stream_3, eLogStream stream_4)
{
  int mask = 0;
  mask |= (stream_1 != eLS_DIMENSION) ? (1 << stream_1) : 0;
  mask |= (stream_2 != eLS_DIMENSION) ? (1 << stream_1) : 0;
  mask |= (stream_3 != eLS_DIMENSION) ? (1 << stream_1) : 0;
  mask |= (stream_4 != eLS_DIMENSION) ? (1 << stream_1) : 0;
  this->SetDomainStreamMask(name, mask);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry SetDomainStreamMask
//----------------------------------------------------------------------
void tLogDomainRegistry::SetDomainStreamMask(const std::string &name, int mask)
{
  tLogDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->stream_mask = mask;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLogDomainRegistry GetDomainIndexByName
//----------------------------------------------------------------------
const size_t tLogDomainRegistry::GetDomainIndexByName(const std::string &name) const
{
  for (size_t i = 0; i < this->domains.size(); ++i)
  {
    if (this->domains[i]->GetName() == name)
    {
      return i;
    }
  }
  return this->domains.size();
}

//----------------------------------------------------------------------
// class tLogDomainRegistry GetConfigurationByName
//----------------------------------------------------------------------
tLogDomainConfigurationSharedPointer tLogDomainRegistry::GetConfigurationByName(const std::string &name)
{
  for (std::vector<tLogDomainConfigurationSharedPointer>::iterator it = this->domain_configurations.begin(); it != this->domain_configurations.end(); ++it)
  {
    if ((*it)->name == name)
    {
      return *it;
    }
  }
  this->domain_configurations.push_back(tLogDomainConfigurationSharedPointer(new tLogDomainConfiguration(name)));
  return this->domain_configurations.back();
}

//----------------------------------------------------------------------
// class tLogDomainRegistry PropagateDomainConfigurationToChildren
//----------------------------------------------------------------------
void tLogDomainRegistry::PropagateDomainConfigurationToChildren(const std::string &name)
{
  size_t i = this->GetDomainIndexByName(name);
  if (i != this->domains.size())
  {
    for (std::vector<tLogDomain *>::iterator it = this->domains[i]->children.begin(); it != this->domains[i]->children.end(); ++it)
    {
      (*it)->ConfigureSubTree();
    }
  }
}

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_

//----------------------------------------------------------------------
// class tLogDomainRegistry ConfigureFromFile
//----------------------------------------------------------------------
bool tLogDomainRegistry::ConfigureFromFile(const std::string &file_name)
{
  try
  {
    tXMLDocument document(file_name);
    return this->ConfigureFromXMLNode(document.GetRootNode());
  }
  catch (const tXML2WrapperException &e)
  {
    std::cerr << "RRLib Logging: tLogDomainRegistry::ConfigureFromFile >> " << e.what() << std::endl;
    return false;
  }
}

//----------------------------------------------------------------------
// class tLogDomainRegistry ConfigureFromXMLNode
//----------------------------------------------------------------------
bool tLogDomainRegistry::ConfigureFromXMLNode(const tXMLNode &node)
{
  if (node.GetName() != "rrlib_logging")
  {
    std::cerr << "RRLib Logging: tLogDomainRegistry::ConfigureFromXMLNode >> Unexpected content (Not an rrlib_logging tree)" << std::endl;
    return false;
  }

  try
  {
    for (std::vector<tXMLNode>::const_iterator it = node.GetChildren().begin(); it != node.GetChildren().end(); ++it)
    {
      if (it->GetName() == "domain")
      {
        if (!this->AddConfigurationFromXMLNode(*it))
        {
          return false;
        }
      }
    }
  }
  catch (const tXML2WrapperException &e)
  {
    std::cerr << "RRLib Logging: tLogDomainRegistry::ConfigureFromXMLNode >> " << e.what() << std::endl;
    return false;
  }

  return true;
}

//----------------------------------------------------------------------
// class tLogDomainRegistry AddConfigurationFromXMLNode
//----------------------------------------------------------------------
bool tLogDomainRegistry::AddConfigurationFromXMLNode(const tXMLNode &node, const std::string &parent_name)
{
  static const char *level_names_init[eLL_DIMENSION] = { "user", "error", "warning", "debug_warning", "debug", "debug_verbose_1", "debug_verbose_2", "debug_verbose_3" };
  static const std::vector<std::string> level_names(level_names_init, level_names_init + eLL_DIMENSION);
  // FIXME: with c++0x this can be static const std::vector<std::string> level_names = { "user", "error", "warning", "debug_warning", "debug", "debug_verbose_1", "debug_verbose_2", "debug_verbose_3" };

  static const char *stream_names_init[eLS_DIMENSION] = { "stdout", "stderr", "file", "combined_file" };
  static const std::vector<std::string> stream_names(stream_names_init, stream_names_init + eLS_DIMENSION);
  // FIXME: with c++0x this can be static const std::vector<std::string> stream_names = { "stdout", "stderr", "file", "combined_file" };

  assert(node.GetName() == "domain");

  const std::string prefix((parent_name == "." ? "" : parent_name) + ".");
  const std::string node_name(node.GetStringAttribute("name"));
  const std::string name(prefix + (!parent_name.length() && node_name == "global" ? "" : node_name));

  if (node.HasAttribute("configures_sub_tree"))
  {
    this->SetDomainConfiguresSubTree(name, node.GetBoolAttribute("configures_sub_tree"));
  }

  if (node.HasAttribute("enabled"))
  {
    this->SetDomainIsEnabled(name, node.GetBoolAttribute("enabled"));
  }

  if (node.HasAttribute("print_time"))
  {
    this->SetDomainPrintsTime(name, node.GetBoolAttribute("print_time"));
  }

  if (node.HasAttribute("print_name"))
  {
    this->SetDomainPrintsName(name, node.GetBoolAttribute("print_name"));
  }

  if (node.HasAttribute("print_level"))
  {
    this->SetDomainPrintsLevel(name, node.GetBoolAttribute("print_level"));
  }

  if (node.HasAttribute("print_location"))
  {
    this->SetDomainPrintsLocation(name, node.GetBoolAttribute("print_location"));
  }

  if (node.HasAttribute("max_level"))
  {
    this->SetDomainMaxMessageLevel(name, node.GetEnumAttribute<eLogLevel>("max_level", level_names));
  }

  bool stream_configured = false;
  if (node.HasAttribute("stream"))
  {
    stream_configured = true;
    this->SetDomainStream(name, node.GetEnumAttribute<eLogStream>("stream", stream_names));
  }

  int stream_mask = 0;
  for (std::vector<tXMLNode>::const_iterator it = node.GetChildren().begin(); it != node.GetChildren().end(); ++it)
  {
    if (it->GetName() == "stream")
    {
      if (stream_configured)
      {
        std::cerr << "RRLib Logging: tLogDomainRegistry::AddConfigurationFromXMLNode >> Stream already configured in domain element!" << std::endl;
        return false;
      }
      stream_mask |= 1 << it->GetEnumAttribute<eLogStream>("output", stream_names);
    }
  }
  if (stream_mask != 0)
  {
    this->SetDomainStreamMask(name, stream_mask);
  }

  for (std::vector<tXMLNode>::const_iterator it = node.GetChildren().begin(); it != node.GetChildren().end(); ++it)
  {
    if (it->GetName() == "domain")
    {
      if (!this->AddConfigurationFromXMLNode(*it, name))
      {
        return false;
      }
    }
  }

  return true;
}

#endif
