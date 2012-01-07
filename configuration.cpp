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
// End of namespace declaration
//----------------------------------------------------------------------
}
}
