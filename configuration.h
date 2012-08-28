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
/*!\file    configuration.h
 *
 * \author  Tobias Foehst
 *
 * \date    2011-01-05
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__configuration_h__
#define __rrlib__logging__configuration_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__

#include "rrlib/logging/log_levels.h"
#include "rrlib/logging/default_log_description.h"
#include "rrlib/logging/configuration/tDomainRegistry.h"

#undef __rrlib__logging__include_guard__

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
// Function declaration
//----------------------------------------------------------------------

void SetLogFilenamePrefix(const std::string &filename_prefix);

void SetPadPrefixColumns(bool value);

void SetPadMultiLineMessages(bool value);

void SetDomainPrintsName(const std::string &domain_name, bool value);

void SetDomainPrintsTime(const std::string &domain_name, bool value);

void SetDomainPrintsLevel(const std::string &domain_name, bool value);

void SetDomainPrintsLocation(const std::string &domain_name, bool value);

void SetDomainMaxMessageLevel(const std::string &domain_name, tLogLevel level);

void SetDomainSink(const std::string &domain_name, tLogSink sink_1, tLogSink sink_2 = (tLogSink)0, tLogSink sink_3 = (tLogSink)0, tLogSink sink_4 = (tLogSink)0);

void PrintDomainConfigurations();

/*! Read domain configuration from a given XML file
 *
 * The overall configuration of the logging domains tends to be
 * too complicated for a classical command line option interface.
 * Therefore, it is possible to specify the configuration in form
 * of an XML file following the DTD -//RRLIB//logging
 *
 * \param filename   The XML file to be read
 *
 * \returns Whether the configuration could be read and applied or not
 */
bool ConfigureFromFile(const std::string &filename);

#ifdef _LIB_RRLIB_XML_PRESENT_

/*! Read domain configuration from a given XML node
 *
 * Instead of reading and parsing an XML file dedicated to configure
 * logging domains this method can be used after externally parsing
 * a document that contains an rrlib_logging node following the DTD
 * -//RRLIB//logging
 *
 * \param node   The XML node containing the configuration
 *
 * \returns Whether the configuration could be applied or not
 */
bool ConfigureFromXMLNode(const xml::tNode &node);

#endif


//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}



#endif
