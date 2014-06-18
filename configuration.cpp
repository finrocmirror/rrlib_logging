//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//----------------------------------------------------------------------
/*!\file    rrlib/logging/configuration.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2011-01-05
 *
 */
//----------------------------------------------------------------------
#include "rrlib/logging/configuration.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/configuration/tDomainRegistry.h"
#include "rrlib/logging/sinks/tSink.h"

#include "rrlib/logging/messages.h"

#ifdef _LIB_RRLIB_XML_PRESENT_
#include "rrlib/xml/tDocument.h"
#endif

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
// SetLogFilenamePrefix
//----------------------------------------------------------------------
void SetLogFilenamePrefix(const std::string &filename_prefix)
{
  tDomainRegistry::Instance().SetLogFilenamePrefix(filename_prefix);
}

//----------------------------------------------------------------------
// SetPadPrefixColumns
//----------------------------------------------------------------------
void SetPadPrefixColumns(bool value)
{
  tDomainRegistry::Instance().SetPadPrefixColumns(value);
}

//----------------------------------------------------------------------
// SetPadMultiLineMessages
//----------------------------------------------------------------------
void SetPadMultiLineMessages(bool value)
{
  tDomainRegistry::Instance().SetPadMultiLineMessages(value);
}

//----------------------------------------------------------------------
// SetDomainPrintsName
//----------------------------------------------------------------------
void SetDomainPrintsName(const std::string &domain_name, bool value, const tDefaultConfigurationContext &default_context)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(default_context, NULL, domain_name.c_str())).SetPrintsName(value);
}

//----------------------------------------------------------------------
// SetDomainPrintsTime
//----------------------------------------------------------------------
void SetDomainPrintsTime(const std::string &domain_name, bool value, const tDefaultConfigurationContext &default_context)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(default_context, NULL, domain_name.c_str())).SetPrintsTime(value);
}

//----------------------------------------------------------------------
// SetDomainPrintsLevel
//----------------------------------------------------------------------
void SetDomainPrintsLevel(const std::string &domain_name, bool value, const tDefaultConfigurationContext &default_context)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(default_context, NULL, domain_name.c_str())).SetPrintsLevel(value);
}

//----------------------------------------------------------------------
// SetDomainPrintsLocation
//----------------------------------------------------------------------
void SetDomainPrintsLocation(const std::string &domain_name, bool value, const tDefaultConfigurationContext &default_context)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(default_context, NULL, domain_name.c_str())).SetPrintsLocation(value);
}

//----------------------------------------------------------------------
// SetDomainMaxMessageLevel
//----------------------------------------------------------------------
void SetDomainMaxMessageLevel(const std::string &domain_name, tLogLevel level, const tDefaultConfigurationContext &default_context)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(default_context, NULL, domain_name.c_str())).SetMaxMessageLevel(level);
}

//----------------------------------------------------------------------
// PrintDomainConfigurations
//----------------------------------------------------------------------
void PrintDomainConfigurations()
{
  struct RecursionHandler
  {
    static void Print(const tConfiguration &configuration)
    {
      std::cout << configuration.GetFullQualifiedName() << " (" << &configuration << ")" << std::endl;
      for (auto it = configuration.Children().begin(); it != configuration.Children().end(); ++it)
      {
        Print(**it);
      }
    }
  };

  RecursionHandler::Print(tDomainRegistry::Instance().GetConfiguration(cDEFAULT_CONTEXT, NULL, "."));
}

//----------------------------------------------------------------------
// ConfigureFromFile
//----------------------------------------------------------------------
bool ConfigureFromFile(const std::string &file_name)
{

#ifdef _LIB_RRLIB_XML_PRESENT_

  xml::tDocument document(file_name);
  return ConfigureFromXMLNode(document.RootNode());

#endif

  RRLIB_LOG_PRINT(ERROR, "XML support not available due to missing rrlib_xml.");
  return false;
}

#ifdef _LIB_RRLIB_XML_PRESENT_

namespace
{

//----------------------------------------------------------------------
// AddConfigurationFromXMLNode
//----------------------------------------------------------------------
bool AddConfigurationFromXMLNode(const xml::tNode &node, const std::string &parent_name)
{
  assert(node.Name() == "domain");

  const std::string node_name(node.GetStringAttribute("name"));
  std::string name;
  if (parent_name.length() == 0)
  {
    if (node_name[0] != '.')
    {
      RRLIB_LOG_PRINT(ERROR, "Trying to configure a log domain not below root domain");
      return false;
    }
  }
  else
  {
    name += parent_name;
    if (parent_name != ".")
    {
      name += ".";
    }
  }
  name += node_name;

  tConfiguration &configuration = const_cast<tConfiguration &>(GetConfiguration(NULL, name.c_str()));

  if (node.HasAttribute("prints_name"))
  {
    configuration.SetPrintsName(node.GetBoolAttribute("prints_name"));
  }

  if (node.HasAttribute("prints_time"))
  {
    configuration.SetPrintsTime(node.GetBoolAttribute("prints_time"));
  }

  if (node.HasAttribute("prints_level"))
  {
    configuration.SetPrintsLevel(node.GetBoolAttribute("prints_level"));
  }

  if (node.HasAttribute("prints_location"))
  {
    configuration.SetPrintsLocation(node.GetBoolAttribute("prints_location"));
  }

  if (node.HasAttribute("max_level"))
  {
    configuration.SetMaxMessageLevel(node.GetEnumAttribute<tLogLevel>("max_level"));
  }

  for (xml::tNode::const_iterator it = node.ChildrenBegin(); it != node.ChildrenEnd(); ++it)
  {
    if (it->Name() == "sink")
    {
      configuration.ClearSinks();
      for (xml::tNode::const_iterator sink = it->ChildrenBegin(); sink != sink->ChildrenEnd(); ++sink)
      {
        configuration.AddSink(std::shared_ptr<sinks::tSink>(sinks::tSinkFactory::Instance().Create(sink->Name(), *sink, configuration)));
      }
    }
  }

  for (xml::tNode::const_iterator it = node.ChildrenBegin(); it != node.ChildrenEnd(); ++it)
  {
    if (it->Name() == "domain")
    {
      if (!AddConfigurationFromXMLNode(*it, name))
      {
        return false;
      }
    }
  }

  return true;
}

}

//----------------------------------------------------------------------
// ConfigureFromXMLNode
//----------------------------------------------------------------------
bool ConfigureFromXMLNode(const xml::tNode &node)
{
  if (node.Name() != "rrlib_logging")
  {
    RRLIB_LOG_PRINT(ERROR, "Unexpected content (Not an rrlib_logging tree)");
    return false;
  }

  try
  {
    if (node.HasAttribute("pad_prefix_columns"))
    {
      SetPadPrefixColumns(node.GetBoolAttribute("pad_prefix_columns"));
    }
    if (node.HasAttribute("pad_multi_line_messages"))
    {
      SetPadMultiLineMessages(node.GetBoolAttribute("pad_multi_line_messages"));
    }

    for (xml::tNode::const_iterator it = node.ChildrenBegin(); it != node.ChildrenEnd(); ++it)
    {
      if (it->Name() == "domain")
      {
        if (!AddConfigurationFromXMLNode(*it, ""))
        {
          return false;
        }
      }
    }
  }
  catch (const xml::tException &exception)
  {
    RRLIB_LOG_PRINT(ERROR, exception);
    return false;
  }

  return true;

}

#endif

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
