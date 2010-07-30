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
/*!\file    definitions.h
 *
 * \author  Tobias Foehst
 * \author  Max Reichardt
 *
 * \date    2010-06-16
 *
 * \brief Contains macro definitions for using the RRLib logging framework
 *
 * \b Creating Domains
 *
 * A few words for definitions.h
 *
 * \b Log Messages
 *
 * A few words for definitions.h
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_logging_definitions_h_
#define _rrlib_logging_definitions_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "logging/tLogDomain.h"
#include "logging/tLogDomainRegistry.h"

// macros for internal use

#define RRLIB_LOG_GET_DOMAIN_I(domain...) \
  rrlib::logging::GetLogDomain(default_log, ## domain) \
   
#define RRLIB_LOG_GET_DOMAIN(domain, args...) \
  RRLIB_LOG_GET_DOMAIN_I(domain)

#define RRLIB_LOG_STREAM_CALL(level, args...) \
  ((level) <= RRLIB_LOG_GET_DOMAIN(args)->GetMaxMessageLevel() ? RRLIB_LOG_GET_DOMAIN(args)->GetMessageStream(GetLogDescription(), __FUNCTION__, __FILE__, __LINE__, level).Evaluate(args) : RRLIB_LOG_GET_DOMAIN(args)->GetMessageStream(GetLogDescription(), __FUNCTION__, __FILE__, __LINE__, level)) \
   
#define RRLIB_LOG_MESSAGE_CALL(level, args...) \
  if ((level) <= RRLIB_LOG_GET_DOMAIN(args)->GetMaxMessageLevel()) \
  { \
    RRLIB_LOG_GET_DOMAIN(args)->PrintMessage(GetLogDescription(), __FUNCTION__, __FILE__, __LINE__, level, args); \
  } \
   
#ifdef _RRLIB_LOGGING_LESS_OUTPUT_

/*! Macro to get a stream for messages using operator <<
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 *
 * \returns The stream for message output
 */
#define RRLIB_LOG_STREAM(level, args...) \
  RRLIB_LOG_STREAM_CALL(level, args) \
   
/*! Macro for messages using printf syntax
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 * \param args     The format string for printf and the optional arguments to be printed.
 */
#define RRLIB_LOG_MESSAGE(level, args...) \
  if ((level) <= rrlib::logging::eLL_DEBUG) \
  { \
    RRLIB_LOG_MESSAGE_CALL(level, args) \
  } \
   
#else

/*! Macro to get a stream for messages using operator <<
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 *
 * \returns The stream for message output
 */
#define RRLIB_LOG_STREAM(level, args...) \
  RRLIB_LOG_STREAM_CALL(level, args) \
   
/*! Macro for messages using printf syntax
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 * \param args     The format string for printf and the optional arguments to be printed.
 */
#define RRLIB_LOG_MESSAGE(level, args...) \
  RRLIB_LOG_MESSAGE_CALL(level, args) \
   
#endif


/*! Macro for creation of a default domain for the current scope
 *
 *  Typical use of logging domains is the creation of a domain for
 *  a specific scope. That means, that the domain is valid between
 *  the enclosing curly brackets. It exists in the domain hierarchy
 *  below the last defined domain of a higher scope.
 *  Using the same name in different scopes at the same level
 *  results in reopening the same domain and combining the log
 *  messages.
 *
 * \param domain_name   The local part of the name in the domain tree.
 */
#define RRLIB_LOG_CREATE_DEFAULT_DOMAIN(domain_name) \
  struct default_log_struct \
  { \
    static rrlib::logging::tLogDomainSharedPointer GetDomain() \
    { \
      static rrlib::logging::tLogDomainSharedPointer instance(rrlib::logging::tLogDomainRegistry::GetInstance().GetSubDomain(domain_name, default_log())); \
      return instance; \
    } \
  }; \
  inline static rrlib::logging::tLogDomainSharedPointer default_log() \
  { \
    return default_log_struct::GetDomain(); \
  } \
   
/*! Macro for creation of a new named domain for the current scope
 *
 *  A named logging domain can exists in parallel to the deault
 *  domain in the same scope. It then has a symbolic name that can
 *  be specified in RRLIB_LOG_STREAM or RRLIB_LOG_MESSAGE. Thus, it
 *  is possible to define more specific domains for one scope for
 *  purposes like writing data to a file or being more selective in
 *  message processing.
 *
 *  In the domain hierarchy the named logging domain exists below the
 *  last created default domain. That means that first creating a
 *  named domain and then creating the scoped domain places both at
 *  the same level whereas changing the order places the named domain
 *  below the current default_log.
 *
 * \param symbolic_name   The name that can be used in following code to use the new domain
 * \param domain_name     The local part of the name in the domain tree.
 */
#define RRLIB_LOG_CREATE_NAMED_DOMAIN(symbolic_name, domain_name) \
  struct symbolic_name ## _struct \
  { \
    static rrlib::logging::tLogDomainSharedPointer GetDomain() \
    { \
      static rrlib::logging::tLogDomainSharedPointer instance(rrlib::logging::tLogDomainRegistry::GetInstance().GetSubDomain(domain_name, default_log())); \
      return instance; \
    } \
  }; \
  inline static rrlib::logging::tLogDomainSharedPointer symbolic_name() \
  { \
    return symbolic_name ## _struct::GetDomain(); \
  } \
   

// The default global scoped logging domain
inline rrlib::logging::tLogDomainSharedPointer default_log()
{
  return rrlib::logging::tLogDomainRegistry::GetDefaultDomain();
}

// The default global GetLogDescription definition
inline const char *GetLogDescription()
{
  return "<Description not defined>";
}

namespace rrlib
{
namespace logging
{

inline rrlib::logging::tLogDomainSharedPointer GetLogDomain(rrlib::logging::tLogDomainSharedPointer(&default_domain)(), ...)
{
  return default_domain();
}
inline rrlib::logging::tLogDomainSharedPointer GetLogDomain(rrlib::logging::tLogDomainSharedPointer(&default_domain)(), rrlib::logging::tLogDomainSharedPointer(&named_domain)())
{
  return named_domain();
}

}
}

#endif
