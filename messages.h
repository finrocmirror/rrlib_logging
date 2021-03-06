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
/*!\file    rrlib/logging/messages.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief   Contains macro definitions for using the RRLib logging framework
 *
 * \b Log Messages
 *
 * A few words for messages.h
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__messages_h__
#define __rrlib__logging__messages_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__

#include "rrlib/logging/log_levels.h"
#include "rrlib/logging/default_log_description.h"
#include "rrlib/logging/messages/implementation.h"

#undef __rrlib__logging__include_guard__

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
// The macro interface and default GetLogDescription function must live
// within the global namespace

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Macros for internal use
//----------------------------------------------------------------------
#define __RRLIB_LOG_PRINT__(domain, level, args...) \
  do \
  { \
    if ((level) <= domain.MaxMessageLevel()) \
    { \
      rrlib::logging::Print(domain, GetLogDescription(), __FUNCTION__, __FILE__, __LINE__, level, args); \
    } \
  } while(0) \
     
#define __RRLIB_LOG_PRINT_STATIC__(domain, level, args...) \
  do \
  { \
    if ((level) <= domain.MaxMessageLevel()) \
    { \
      rrlib::logging::Print(domain, "<static>", __FUNCTION__, __FILE__, __LINE__, level, args); \
    } \
  } while(0) \
     
#define __RRLIB_LOG_PRINTF__(domain, level, args...) \
  do \
  { \
    if ((level) <= domain.MaxMessageLevel()) \
    { \
      rrlib::logging::PrintFormatted(domain, GetLogDescription(), __FUNCTION__, __FILE__, __LINE__, level, args); \
    } \
  } while(0) \
     
#define __RRLIB_LOG_PRINTF_STATIC__(domain, level, args...) \
  do \
  { \
    if ((level) <= domain.MaxMessageLevel()) \
    { \
      rrlib::logging::PrintFormatted(domain, "<static>", __FUNCTION__, __FILE__, __LINE__, level, args); \
    } \
  } while(0) \
     
#define __EXPAND_LEVEL__(level) rrlib::logging::tLogLevel::level

//----------------------------------------------------------------------
// The macro interface to the logging library
//----------------------------------------------------------------------
#ifdef RRLIB_LOGGING_LESS_OUTPUT

/*! Macro to print messages using stream semantics
 *
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT(level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINT__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
/*! Macro to print messages to explicitly specified domain using stream semantics
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT_TO(domain, level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINT__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
/*! Macro to print messages using stream semantics from static context
 *
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT_STATIC(level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINT_STATIC__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
/*! Macro to print messages to explicitly specified domain using stream semantics from static context
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT_STATIC_TO(domain, level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINT_STATIC__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
/*! Macro to print messages using printf semantics
 *
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF(level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINTF__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
/*! Macro to print messages to explicitly specified domain using printf semantics
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF_TO(domain, level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINTF__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
/*! Macro to print messages using printf semantics from static context
 *
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF_STATIC(level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINTF_STATIC__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
/*! Macro to print messages to explicitly specified domain using printf semantics from static context
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF_STATIC_TO(domain, level, args...) \
  do \
  { \
    if ((__EXPAND_LEVEL__(level)) <= rrlib::logging::tLogLevel::DEBUG) \
    { \
      __RRLIB_LOG_PRINTF_STATIC__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args); \
    } \
  } while (0) \
     
#else

/*! Macro to print messages using stream semantics
 *
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT(level, args...) \
  __RRLIB_LOG_PRINT__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args) \
   
/*! Macro to print messages to explicitly specified domain using stream semantics
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT_TO(domain, level, args...) \
  __RRLIB_LOG_PRINT__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args) \
   
/*! Macro to print messages using stream semantics static context
 *
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT_STATIC(level, args...) \
  __RRLIB_LOG_PRINT_STATIC__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args) \
   
/*! Macro to print messages to explicitly specified using stream semantics static context
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The data to be put into the underlying stream
 */
#define RRLIB_LOG_PRINT_STATIC_TO(domain, level, args...) \
  __RRLIB_LOG_PRINT_STATIC__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args) \
   
/*! Macro to print messages using printf semantics
 *
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF(level, args...) \
  __RRLIB_LOG_PRINTF__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args) \
   
/*! Macro to print messages to explicitly specified using printf semantics
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF_TO(domain, level, args...) \
  __RRLIB_LOG_PRINTF__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args) \
   
/*! Macro to print messages using printf semantics from static context
 *
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF_STATIC(level, args...) \
  __RRLIB_LOG_PRINTF_STATIC__(rrlib::logging::GetConfiguration(__FILE__), __EXPAND_LEVEL__(level), args) \
   
/*! Macro to print messages to explicitly specified using printf semantics from static context
 *
 * \param domain   The domain the message should be printed to
 * \param level    The level of the message
 * \param args     The format string for printf and the optional arguments to be printed
 */
#define RRLIB_LOG_PRINTF_STATIC_TO(domain, level, args...) \
  __RRLIB_LOG_PRINTF_STATIC__(rrlib::logging::GetConfiguration(__FILE__, #domain), __EXPAND_LEVEL__(level), args) \
   
#endif

/*! Macro to throw and log exceptions in one line
 *
 * \param exception   The exception to be thrown
 */
#define RRLIB_LOG_THROW(exception) \
  do \
  { \
    RRLIB_LOG_PRINT(ERROR, exception); \
    throw exception; \
  } while (0) \
     
/*! Macro to throw and log exceptions to explicitly specified domain in one line
 *
 * \param domain      The domain the message should be printed to
 * \param exception   The exception to be thrown
 */
#define RRLIB_LOG_THROW_TO(domain, exception) \
  do \
  { \
    RRLIB_LOG_PRINT_TO(domain, ERROR, exception); \
    throw exception; \
  } while (0) \
     
/*! Macro to throw and log exceptions in one line from static context
 *
 * \param exception   The exception to be thrown
 */
#define RRLIB_LOG_THROW_STATIC(exception) \
  do \
  { \
    RRLIB_LOG_PRINT_STATIC(ERROR, exception); \
    throw exception; \
  } while (0) \
     
/*! Macro to throw and log exceptions to explicitly specified domain in one line from static context
 *
 * \param domain      The domain the message should be printed to
 * \param exception   The exception to be thrown
 */
#define RRLIB_LOG_THROW_STATIC_TO(domain, exception) \
  do \
  { \
    RRLIB_LOG_PRINT_STATIC_TO(domain, ERROR, exception); \
    throw exception; \
  } while (0) \
     

// The default global GetLogDescription definition
inline const char *GetLogDescription()
{
  return rrlib::logging::default_log_description;
}


#endif
