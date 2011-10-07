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
/*!\file    tLogDomainConfiguration.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/tLogDomainConfiguration.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
#include <iostream>
#include "rrlib/xml2_wrapper/tXMLDocument.h"
#endif

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/tLogDomain.h"

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
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------
#ifdef _RRLIB_LOGGING_LESS_OUTPUT_
const bool cDEFAULT_PRINT_TIME = false;                 //!< Default print time setting for reduced output mode
const bool cDEFAULT_PRINT_NAME = false;                 //!< Default print name setting for reduced output mode
const bool cDEFAULT_PRINT_LEVEL = false;                //!< Default print level setting for reduced output mode
const bool cDEFAULT_PRINT_LOCATION = false;             //!< Default print location setting for reduced output mode
const tLogLevel cDEFAULT_MAX_LOG_LEVEL = eLL_WARNING;   //!< Default max log level for reduced output mode
const int cDEFAULT_SINK_MASK = 1 << eLS_STDOUT;         //!< Default output stream mask
#else
const bool cDEFAULT_PRINT_TIME = false;               //!< Default print time setting for normal output mode
const bool cDEFAULT_PRINT_NAME = false;               //!< Default print name setting for normal output mode
const bool cDEFAULT_PRINT_LEVEL = false;              //!< Default print level setting for normal output mode
const bool cDEFAULT_PRINT_LOCATION = true;            //!< Default print location setting for normal output mode
const tLogLevel cDEFAULT_MAX_LOG_LEVEL = eLL_DEBUG;   //!< Default max log level for normal output mode
const int cDEFAULT_SINK_MASK = 1 << eLS_STDOUT;       //!< Default output stream mask
#endif

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tLogDomainConfiguration constructors
//----------------------------------------------------------------------
tLogDomainConfiguration::tLogDomainConfiguration(const std::string &name)
    : name(name),
    configure_sub_tree(false),
    print_time(cDEFAULT_PRINT_TIME),
    print_name(cDEFAULT_PRINT_NAME),
    print_level(cDEFAULT_PRINT_LEVEL),
    print_location(cDEFAULT_PRINT_LOCATION),
    max_message_level(cDEFAULT_MAX_LOG_LEVEL),
    sink_mask(cDEFAULT_SINK_MASK)
{}

tLogDomainConfiguration::tLogDomainConfiguration(const tLogDomainConfiguration &other)
    : configure_sub_tree(other.configure_sub_tree),
    print_time(other.print_time),
    print_name(other.print_name),
    print_level(other.print_level),
    print_location(other.print_location),
    max_message_level(other.max_message_level),
    sink_mask(other.sink_mask)
{}

//----------------------------------------------------------------------
// tLogDomainConfiguration operator =
//----------------------------------------------------------------------
tLogDomainConfiguration &tLogDomainConfiguration::operator = (const tLogDomainConfiguration other)
{
  this->configure_sub_tree = other.configure_sub_tree;
  this->print_time = other.print_time;
  this->print_name = other.print_name;
  this->print_level = other.print_level;
  this->print_location = other.print_location;
  this->max_message_level = other.max_message_level;
  this->sink_mask = other.sink_mask;
  return *this;
}
