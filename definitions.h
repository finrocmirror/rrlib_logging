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
// External includes with <>
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "logging/tLoggingDomain.h"
#include "logging/tLoggingDomainRegistry.h"

// macros for internal use
#define RRLIB_LOG_STREAM_CALL(level, domain...) \
  (#domain[0] ? domain::GetDomainForUseInRRLibMacros() : ScopedLoggingDomain::GetDomainForUseInRRLibMacros())->GetMessageStream(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level) \
   
#define RRLIB_LOG_MESSAGE_CALL(level, domain, args...) \
  domain::GetDomainForUseInRRLibMacros()->PrintMessage(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level, args); \
   
/*
#define RRLIB_LOG_MESSAGE_CALL(level, first_arg, args...) \
  if (#first_arg[0] == '\"') { \
    ScopedLoggingDomain::GetDomainForUseInRRLibMacros()->PrintMessage(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level, ## args); \
  } else { \
    typeof(first_arg) object = ""; \
    ScopedLoggingDomain::GetDomainForUseInRRLibMacros()->PrintMessage(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level, object, ## args); \
  } \
*/


#ifdef _RRLIB_LOGGING_LESS_OUTPUT_

/*! Macro to get a stream for messages using operator <<
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 *
 * \returns The stream for message output
 */
#define RRLIB_LOG_STREAM(level, domain...) \
  RRLIB_LOG_STREAM_CALL(level, domain) \
   
/*! Macro for messages using printf syntax
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 * \param args     The format string for printf and the optional arguments to be printed.
 */
#define RRLIB_LOG_MESSAGE(level, domain, args...) \
  if (level  >= rrlib::logging::eLL_HIGH) \
  { \
    RRLIB_LOG_MESSAGE_CALL(level, domain, args) \
  } \
   
#else

/*! Macro to get a stream for messages using operator <<
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 *
 * \returns The stream for message output
 */
#define RRLIB_LOG_STREAM(level, domain...) \
  RRLIB_LOG_STREAM_CALL(level, domain) \
   
/*! Macro for messages using printf syntax
 *
 * \param level    The level of the message
 * \param domain   The domain the message should be processed in
 * \param args     The format string for printf and the optional arguments to be printed.
 */
#define RRLIB_LOG_MESSAGE(level, domain, args...) \
  RRLIB_LOG_MESSAGE_CALL(level, domain, args) \
   
#endif


/*! Macro for creation of a new scoped logging domain
 *
 *  Typical use of logging domains is the creation of a domain for
 *  a specific scope. That means, that the domain is valid between
 *  the enclosing curly brackets. It exists in the domain hierarchy
 *  below the last defined domain of a higher scope.
 *  Using the same name in different scopes at the same level
 *  results in reopening the same domain and combining the log
 *  messages.
 *
 *  The scoped logging domain is the default for following occurences
 *  of RRLIB_LOG_STREAM and can be used in RRLIB_LOG_MESSAGE using
 *  the symbol ScopedLoggingDomain.
 *
 * \param name   The local part of the name in the domain tree.
 */
#define CREATE_SCOPED_LOGGING_DOMAIN(name) \
  typedef ScopedLoggingDomain ParentScopedLoggingDomain; \
  struct ScopedLoggingDomain \
  { \
    static rrlib::logging::tLoggingDomainSharedPointer GetDomain()    \
    { \
      static rrlib::logging::tLoggingDomainSharedPointer instance(rrlib::logging::tLoggingDomainRegistry::GetInstance().GetSubDomain(name, ParentScopedLoggingDomain::GetDomain())); \
      return instance; \
    } \
    static inline rrlib::logging::tLoggingDomainSharedPointer GetDomainForUseInRRLibMacros() \
    { \
      return GetDomain(); \
    } \
  }; \
   
/*! Macro for creation of a new named logging domain
 *
 *  A named logging domain is the same like a scoped logging
 *  domain, but can exist in parallel on the same scope. It then
 *  has a symbolic name that can be specified in RRLIB_LOG_STREAM
 *  or RRLIB_LOG_MESSAGE. Thus, it is possible to defined more
 *  specific domains for one scope for purposes like writing data to
 *  a file or being more selective in message processing.
 *
 *  In the domain hierarchy the named logging domain exists below the
 *  last created ScopedLoggingDomain. That means that first creating
 *  a named domain and then creating the scoped domain places both at
 *  the same level whereas changing the order places the named domain
 *  below the current ScopedLoggingDomain.
 *
 * \param class-name    The symbolic that can be used in following code to use the new domain
 * \param domain-name   The local part of the name in the domain tree.
 */
#define CREATE_NAMED_LOGGING_DOMAIN(class_name, domain_name) \
  struct class_name \
  { \
    static rrlib::logging::tLoggingDomainSharedPointer GetDomain()    \
    { \
      static rrlib::logging::tLoggingDomainSharedPointer instance(rrlib::logging::tLoggingDomainRegistry::GetInstance().GetSubDomain(domain_name, ScopedLoggingDomain::GetDomain())); \
      return instance; \
    } \
    static inline rrlib::logging::tLoggingDomainSharedPointer GetDomainForUseInRRLibMacros() \
    { \
      return GetDomain(); \
    } \
  }; \
   

// The default global scoped logging domain
struct ScopedLoggingDomain
{
  static rrlib::logging::tLoggingDomainSharedPointer GetDomain()
  {
    return rrlib::logging::tLoggingDomainRegistry::GetDefaultDomain();
  }
  static rrlib::logging::tLoggingDomainSharedPointer GetDomainForUseInRRLibMacros()
  {
    return GetDomain();
  }
};

// The default global GetDescription definition
inline const char *GetDescription()
{
  return "<Description not defined>";
}

// To trick the compiler we need a global function declaration
inline rrlib::logging::tLoggingDomainSharedPointer GetDomainForUseInRRLibMacros()
{
  return rrlib::logging::tLoggingDomainSharedPointer();
}

#endif
