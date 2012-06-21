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
/*!\file    configuration.cpp
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
void SetDomainPrintsName(const std::string &domain_name, bool value)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(NULL, domain_name.c_str())).SetPrintsName(value);
}

//----------------------------------------------------------------------
// SetDomainPrintsTime
//----------------------------------------------------------------------
void SetDomainPrintsTime(const std::string &domain_name, bool value)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(NULL, domain_name.c_str())).SetPrintsTime(value);
}

//----------------------------------------------------------------------
// SetDomainPrintsLevel
//----------------------------------------------------------------------
void SetDomainPrintsLevel(const std::string &domain_name, bool value)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(NULL, domain_name.c_str())).SetPrintsLevel(value);
}

//----------------------------------------------------------------------
// SetDomainPrintsLocation
//----------------------------------------------------------------------
void SetDomainPrintsLocation(const std::string &domain_name, bool value)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(NULL, domain_name.c_str())).SetPrintsLocation(value);
}

//----------------------------------------------------------------------
// SetDomainMaxMessageLevel
//----------------------------------------------------------------------
void SetDomainMaxMessageLevel(const std::string &domain_name, tLogLevel level)
{
  const_cast<tConfiguration &>(tDomainRegistry::Instance().GetConfiguration(NULL, domain_name.c_str())).SetMaxMessageLevel(level);
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

  RecursionHandler::Print(tDomainRegistry::Instance().GetConfiguration(NULL, "."));
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

  RRLIB_LOG_PRINT(eLL_ERROR, "XML support not available due to missing rrlib_mca2_wrapper.");
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
  static const std::vector<std::string> level_names = { "error", "warning", "debug_warning", "debug", "debug_verbose_1", "debug_verbose_2", "debug_verbose_3" };
  static const std::vector<std::string> sink_names = { "stdout", "stderr", "file", "combined_file" };

  assert(node.Name() == "domain");

  const std::string node_name(node.GetStringAttribute("name"));
  std::string name;
  if (parent_name.length() == 0)
  {
    if (node_name[0] != '.')
    {
      RRLIB_LOG_PRINT(eLL_ERROR, "Trying to configure a log domain not below root domain");
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
    configuration.SetMaxMessageLevel(static_cast<tLogLevel>(eLL_ERROR + node.GetEnumAttribute<tLogLevel>("max_level", level_names)));
  }

//  bool sinks_configured = false;
//  if (node.HasAttribute("sink"))
//  {
//    sinks_configured = true;
//    this->SetDomainSink(name, node.GetEnumAttribute<tLogSink>("sink", sink_names));
//  }

//  int sink_mask = 0;
//  for (xml::tNode::const_iterator it = node.ChildrenBegin(); it != node.ChildrenEnd(); ++it)
//  {
//    if (it->Name() == "sink")
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
    RRLIB_LOG_PRINT(eLL_ERROR, "Unexpected content (Not an rrlib_logging tree)");
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
    RRLIB_LOG_PRINT(eLL_ERROR, exception);
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
