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
/*!\file    tLogDomainConfiguration.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-20
 *
 * \brief Contains tLogDomainConfiguration and corresponding enumerations eLogLevel, eLogStream, eLogStreamMask
 *
 * \b tLogDomainConfiguration
 *
 * tLogDomainConfiguration encapsulates the configuration of logging
 * domains in the RRLib logging facility. It therefore stores settings
 * like enabled output fields, max. message level, etc.
 *
 *
 * \b tLogLevel
 *
 * The enumeration that encodes the message levels. These levels are
 * predefined and can be used to give the messages different priorities,
 * as messages with too low level will be suppressed by a configuration
 * setting or when _RRLIB_LOGGING_LESS_OUTPUT_ is defined (e.g. in
 * release mode).
 * They are also used for colored output to stdout or stderr.
 *
 *
 * \b tLogSink
 *
 * The enumeration that encodes the sinks used by a logging domain.
 * Messages can be streamed to stdout, stderr, into one file per domain
 * or into one combined file for all domains that are recursively
 * configured in one subtree of the domain hierarchy.
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_logging_include_guard_
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef _rrlib_logging_tLogDomainConfiguration_h_
#define _rrlib_logging_tLogDomainConfiguration_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <tr1/memory>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
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
//! Enumeration type that contains the available message levels
enum tLogLevel
{
  eLL_USER,             //!< Information for user (including end-users). Is always shown.
  eLL_ERROR,            //!< Error message. Used to inform about _certain_ malfunction of application. Is always shown.
  eLL_WARNING,          //!< Critical warning. Warns about possible application malfunction and invalid (and therefore discarded) user input. (default max level with _RRLIB_LOGGING_LESS_OUTPUT_)
  eLL_DEBUG_WARNING,    //!< Debug info with warning character (e.g. "Parameter x not set - using default y")
  eLL_DEBUG,            //!< Debug info about coarse program flow (default max level without _RRLIB_LOGGING_LESS_OUTPUT_) - information possibly relevant to developers outside of respective domain
  eLL_DEBUG_VERBOSE_1,  //!< Higher detail debug info (not available in release mode) - only relevant to developers in respective domain
  eLL_DEBUG_VERBOSE_2,  //!< Higher detail debug info (not available in release mode) - only relevant to developers in respective domain
  eLL_DEBUG_VERBOSE_3,  //!< Higher detail debug info (not available in release mode) - only relevant to developers in respective domain
  eLL_DIMENSION         //!< Endmarker and dimension of eLogLevel
};


//! Enumeration type that contains the available sinks for message domains
enum tLogSink
{
  eLS_STDOUT,          //!< Messages are printed to stdout
  eLS_STDERR,          //!< Messages are printed to stderr
  eLS_FILE,            //!< Messages are printed to one file per domain
  eLS_COMBINED_FILE,   //!< Messages are collected in one file per recursively configured subtree
  eLS_DIMENSION        //!< Endmarker and dimension of eLogStream
};

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! This class encapsulates the configuration of logging domains
/*! Each logging domain has its own configuration consisting of settings
 *  like enabled output fields, max. message level, etc.
 *  It is a implemented common structure that can be used by instances of
 *  tLogDomain and tLogDomainRegistry. Thus, it should not be
 *  instantiated or used by other classes and is declared totally private
 *  with the mentioned classes as friends.
 *
 */
class tLogDomainConfiguration
{
  friend class tLogDomainRegistry;
  friend class tLogDomain;

  std::string name;
  bool configure_sub_tree;

  bool print_time;
  bool print_name;
  bool print_level;
  bool print_location;
  tLogLevel max_message_level;
  int sink_mask;

  explicit tLogDomainConfiguration(const std::string &name);

  tLogDomainConfiguration(const tLogDomainConfiguration &other);

  tLogDomainConfiguration &operator = (const tLogDomainConfiguration other);

};

//! Shared pointer to instances of tLogDomainConfiguration
typedef std::tr1::shared_ptr<tLogDomainConfiguration> tLogDomainConfigurationSharedPointer;

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
