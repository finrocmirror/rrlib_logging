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
// External includes with <>
//----------------------------------------------------------------------
#include <algorithm>
#include <iterator>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "tLoggingDomain.h"

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
const char *cDEFAULT_DOMAIN_NAME = "global";

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

tLoggingDomainRegistry::tLoggingDomainRegistry()
{
  this->domain_configurations.push_back(tLoggingDomainConfigurationSharedPointer(new tLoggingDomainConfiguration(cDEFAULT_DOMAIN_NAME)));
  this->domain_configurations.back()->enabled = true;
  this->domains.push_back(std::tr1::shared_ptr<tLoggingDomain>(new tLoggingDomain(this->domain_configurations.back())));
}

tLoggingDomainRegistry &tLoggingDomainRegistry::GetInstance()
{
  static tLoggingDomainRegistry instance;
  return instance;
}

tLoggingDomainSharedPointer tLoggingDomainRegistry::GetSubDomain(const std::string &name, tLoggingDomainSharedPointer parent)
{
  assert(name.length() > 0);
  const std::string full_qualified_domain_name(parent->GetName() + "." + name);
  size_t i = this->GetDomainIndexByName(full_qualified_domain_name);
  if (i == this->domains.size())
  {
    tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(full_qualified_domain_name));
    this->domains.push_back(std::tr1::shared_ptr<tLoggingDomain>(new tLoggingDomain(configuration, *const_cast<tLoggingDomain *>(parent.get()))));
    return this->domains.back();
  }
  return this->domains[i];
}

//void tMessageDomainRegistry::ListDomains() const
//{
//  std::cout << "Registered message domains:" << std::endl;
//  for (std::vector<std::tr1::shared_ptr<tMessageDomain> >::const_iterator it = this->domains.begin(); it != this->domains.end(); ++it)
//  {
//    std::cout << "  " << (*it)->GetName() << std::endl;
//  }
//  std::cout << std::endl;
//}

size_t tLoggingDomainRegistry::GetMaxDomainNameLength() const
{
  size_t result = 0;
  for (std::vector<tLoggingDomainConfigurationSharedPointer>::const_iterator it = this->domain_configurations.begin(); it != this->domain_configurations.end(); ++it)
  {
    result = std::max(result, (*it)->name.length());
  }
  return result;
}

void tLoggingDomainRegistry::SetDomainConfiguresSubTree(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->configure_sub_tree = value;
  this->PropagateDomainConfigurationToChildren(name);
}
void tLoggingDomainRegistry::SetDomainIsEnabled(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->enabled = value;
  this->PropagateDomainConfigurationToChildren(name);
}
void tLoggingDomainRegistry::SetDomainPrintsTime(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_time = value;
  this->PropagateDomainConfigurationToChildren(name);
}
void tLoggingDomainRegistry::SetDomainPrintsName(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_name = value;
  this->PropagateDomainConfigurationToChildren(name);
}
void tLoggingDomainRegistry::SetDomainPrintsLevel(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_level = value;
  this->PropagateDomainConfigurationToChildren(name);
}
void tLoggingDomainRegistry::SetDomainPrintsLocation(const std::string &name, bool value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->print_location = value;
  this->PropagateDomainConfigurationToChildren(name);
}
void tLoggingDomainRegistry::SetDomainMinMessageLevel(const std::string &name, eLogLevel value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->min_message_level = value;
  this->PropagateDomainConfigurationToChildren(name);
}
void tLoggingDomainRegistry::SetDomainStreamID(const std::string &name, eLogStream value)
{
  tLoggingDomainConfigurationSharedPointer configuration(this->GetConfigurationByName(name));
  configuration->stream_id = value;
  this->PropagateDomainConfigurationToChildren(name);
}




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
