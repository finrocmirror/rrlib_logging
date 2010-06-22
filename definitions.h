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
 * \brief
 *
 * \b
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
#include "tLoggingDomain.h"
#include "tLoggingDomainRegistry.h"

// macros for internal use
#define RRLIB_LOG_STREAM_CALL(level, args...) \
  (#args[0] ? args::GetDomainForUseInRRLibMacros() : ScopedLoggingDomain::GetDomainForUseInRRLibMacros())->GetMessageStream(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level) \
   
#define RRLIB_LOG_MESSAGE_CALL(level, domain, args...) \
  domain::GetDomainForUseInRRLibMacros()->PrintMessage(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level, args); \
   
/*
    if (#first_arg[0] != '\"') \
    { \
      ScopedLoggingDomain::GetDomainForUseInRRLibMacros()->PrintMessage(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level); \
      printf(first_arg, ##args); \
    } \
    else \
    {\
      first_arg::GetDomainForUseInRRLibMacros()->PrintMessage(GetDescription(), __FUNCTION__, __FILE__, __LINE__, level); \
      printf(args); \
    }\
*/




#ifdef _RRLIB_MESSAGES_LESS_OUTPUT_

// Macro to get a stream for messages using operator <<
#define RRLIB_LOG_STREAM(level, args...) \
  RRLIB_LOG_STREAM_CALL(level, args) \
   
// Macro for messages using printf syntax
#define RRLIB_LOG_MESSAGE(level, domain, args...) \
  if (level  >= ::rrlib::logging::eML_HIGH) \
  { \
    RRLIB_LOG_MESSAGE_CALL(level, domain, args) \
  } \
   
#else

// Macro to get a stream for messages using operator <<
#define RRLIB_LOG_STREAM(level, args...) \
  RRLIB_LOG_STREAM_CALL(level, args) \
   
// Macro for messages using printf syntax
#define RRLIB_LOG_MESSAGE(level, domain, args...) \
  RRLIB_LOG_MESSAGE_CALL(level, domain, args) \
   
#endif


// Macro for creation of a new scoped logging domain
#define CREATE_SCOPED_LOGGING_DOMAIN(name) \
  typedef ScopedLoggingDomain ParentScopedLoggingDomain; \
  struct ScopedLoggingDomain \
  { \
    static ::rrlib::logging::tLoggingDomainSharedPointer GetDomain()    \
    { \
      static ::rrlib::logging::tLoggingDomainSharedPointer instance(::rrlib::logging::tLoggingDomainRegistry::GetInstance().GetSubDomain(name, ParentScopedLoggingDomain::GetDomain())); \
      return instance; \
    } \
    static inline ::rrlib::logging::tLoggingDomainSharedPointer GetDomainForUseInRRLibMacros() \
    { \
      return GetDomain(); \
    } \
  }; \
   
// Macro for creation of a new named logging domain
#define CREATE_NAMED_LOGGING_DOMAIN(class_name, domain_name) \
  struct class_name \
  { \
    static ::rrlib::logging::tLoggingDomainSharedPointer GetDomain()    \
    { \
      static ::rrlib::logging::tLoggingDomainSharedPointer instance(::rrlib::logging::tLoggingDomainRegistry::GetInstance().GetSubDomain(domain_name, ScopedLoggingDomain::GetDomain())); \
      return instance; \
    } \
    static inline ::rrlib::logging::tLoggingDomainSharedPointer GetDomainForUseInRRLibMacros() \
    { \
      return GetDomain(); \
    } \
  }; \
   

// The default global scoped logging domain
struct ScopedLoggingDomain
{
  static ::rrlib::logging::tLoggingDomainSharedPointer GetDomain()
  {
    return ::rrlib::logging::tLoggingDomainRegistry::GetDefaultDomain();
  }
};

// The default global GetDescription definition
inline const char *GetDescription()
{
  return "<Description not defined>";
}

// To trick the compiler we need a global function declaration
inline ::rrlib::logging::tLoggingDomainSharedPointer GetDomainForUseInRRLibMacros()
{
  return ::rrlib::logging::tLoggingDomainSharedPointer();
}

#endif
