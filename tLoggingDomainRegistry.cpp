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
/*!\file    tLoggingDomainRegistry.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief
 *
 * \b
 *
 * A few words for tLoggingDomainRegistry.cpp
 *
 */
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "tLoggingDomainRegistry.h"

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
#include "logging/tLoggingDomain.h"

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
// class tLoggingDomainRegistry constructors
//----------------------------------------------------------------------
tLoggingDomainRegistry::tLoggingDomainRegistry()
{
  this->domain_configurations.push_back(tLoggingDomainConfigurationSharedPointer(new tLoggingDomainConfiguration(".")));
  this->domain_configurations.back()->enabled = true;
  this->domains.push_back(std::tr1::shared_ptr<tLoggingDomain>(new tLoggingDomain(this->domain_configurations.back())));
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry GetInstance
//----------------------------------------------------------------------
tLoggingDomainRegistry &tLoggingDomainRegistry::GetInstance()
{
  static tLoggingDomainRegistry instance;
  return instance;
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry GetSubDomain
//----------------------------------------------------------------------
tLoggingDomainSharedPointer tLoggingDomainRegistry::GetSubDomain(const std::string &name, tLoggingDomainSharedPointer parent)
{
  assert(name.length() > 0);
  const std::string full_qualified_domain_name(parent->GetName() + (parent->parent ? "." : "") + name);
  size_t i = this->GetDomainIndexByName(full_qualified_domain_name);
  if (i == this->domains.size())
  {
    tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(full_qualified_domain_name));
    this->domains.push_back(std::tr1::shared_ptr<tLoggingDomain>(new tLoggingDomain(configuration, *const_cast<tLoggingDomain *>(parent.get()))));
    return this->domains.back();
  }
  return this->domains[i];
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry GetMaxDomainNameLength
//----------------------------------------------------------------------
size_t tLoggingDomainRegistry::GetMaxDomainNameLength() const
{
  size_t result = 0;
  for (std::vector<tLoggingDomainConfigurationSharedPointer>::const_iterator it = this->domain_configurations.begin(); it != this->domain_configurations.end(); ++it)
  {
    result = std::max(result, (*it)->name.length());
  }
  return result;
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainConfiguresSubTree
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainConfiguresSubTree(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->configure_sub_tree = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainIsEnabled
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainIsEnabled(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->enabled = value;
  this->PropagateDomainConfigurationToChildren(name);
}
//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainPrintsTime
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainPrintsTime(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_time = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainPrintsName
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainPrintsName(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_name = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainPrintsLevel
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainPrintsLevel(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_level = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainPrintsLocation
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainPrintsLocation(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_location = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainMinMessageLevel
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainMinMessageLevel(const std::string &name, eLogLevel value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->min_message_level = value;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry SetDomainStreamMask
//----------------------------------------------------------------------
void tLoggingDomainRegistry::SetDomainStreamMask(const std::string &name, eLogStreamMask mask)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->stream_mask = mask;
  this->PropagateDomainConfigurationToChildren(name);
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry GetDomainIndexByName
//----------------------------------------------------------------------
const size_t tLoggingDomainRegistry::GetDomainIndexByName(const std::string &name) const
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
// class tLoggingDomainRegistry GetConfigurationByName
//----------------------------------------------------------------------
tLoggingDomainConfigurationSharedPointer tLoggingDomainRegistry::GetConfigurationByName(const std::string &name)
{
  for (std::vector<tLoggingDomainConfigurationSharedPointer>::iterator it = this->domain_configurations.begin(); it != this->domain_configurations.end(); ++it)
  {
    if ((*it)->name == name)
    {
      return *it;
    }
  }
  this->domain_configurations.push_back(tLoggingDomainConfigurationSharedPointer(new tLoggingDomainConfiguration(name)));
  return this->domain_configurations.back();
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry PropagateDomainConfigurationToChildren
//----------------------------------------------------------------------
void tLoggingDomainRegistry::PropagateDomainConfigurationToChildren(const std::string &name)
{
  size_t i = this->GetDomainIndexByName(name);
  if (i != this->domains.size())
  {
    for (std::vector<tLoggingDomain *>::iterator it = this->domains[i]->children.begin(); it != this->domains[i]->children.end(); ++it)
    {
      (*it)->ConfigureSubTree();
    }
  }
}

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_

//----------------------------------------------------------------------
// class tLoggingDomainRegistry ConfigureFromFile
//----------------------------------------------------------------------
bool tLoggingDomainRegistry::ConfigureFromFile(const std::string &file_name)
{
  try
  {
    tXMLDocument document(file_name);
    return this->ConfigureFromXMLNode(document.GetRootNode());
  }
  catch (const tXML2WrapperException &e)
  {
    std::cerr << "RRLib Logging: tLoggingDomainRegistry::ConfigureFromFile >> " << e.what() << std::endl;
    return false;
  }
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry ConfigureFromXMLNode
//----------------------------------------------------------------------
bool tLoggingDomainRegistry::ConfigureFromXMLNode(const tXMLNode &node)
{
  if (node.GetName() != "rrlib_logging")
  {
    std::cerr << "RRLib Logging: tLoggingDomainRegistry::ConfigureFromXMLNode >> Unexpected content (Not an rrlib_logging tree)" << std::endl;
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
    std::cerr << "RRLib Logging: tLoggingDomainRegistry::ConfigureFromXMLNode >> " << e.what() << std::endl;
    return false;
  }

  return true;
}

//----------------------------------------------------------------------
// class tLoggingDomainRegistry AddConfigurationFromXMLNode
//----------------------------------------------------------------------
bool tLoggingDomainRegistry::AddConfigurationFromXMLNode(const tXMLNode &node, const std::string &parent_name)
{
  static const char *level_names_init[eLL_DIMENSION] = { "verbose", "low", "medium", "high", "always" };
  static const std::vector<std::string> level_names(level_names_init, level_names_init + eLL_DIMENSION);
  // FIXME: with c++0x this can be static const std::vector<std::string> level_names = { "verbose", "low", "medium", "high", "always" };

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

  if (node.HasAttribute("min_level"))
  {
    this->SetDomainMinMessageLevel(name, node.GetEnumAttribute<eLogLevel>("min_level", level_names));
  }

  bool stream_configured = false;
  if (node.HasAttribute("stream"))
  {
    stream_configured = true;
    this->SetDomainStreamMask(name, static_cast<eLogStreamMask>(1 << node.GetEnumAttribute<eLogStream >("stream", stream_names)));
  }

  eLogStreamMask stream_mask = static_cast<eLogStreamMask>(0);
  for (std::vector<tXMLNode>::const_iterator it = node.GetChildren().begin(); it != node.GetChildren().end(); ++it)
  {
    if (it->GetName() == "stream")
    {
      if (stream_configured)
      {
        std::cerr << "RRLib Logging: tLoggingDomainRegistry::AddConfigurationFromXMLNode >> Stream already configured in domain element!" << std::endl;
        return false;
      }
      stream_mask |= static_cast<eLogStreamMask>(1 << it->GetEnumAttribute<eLogStream>("output", stream_names));
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
