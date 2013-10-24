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
/*!\file    rrlib/logging/messages/implementation.h
 *
 * \author  Tobias Foehst
 *
 * \date    2011-09-15
 *
 * \brief   Contains the implementation of message printing
 *
 * A few words for implementation.h
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/messages.h" instead.
#endif

#ifndef __rrlib__logging__messages__implementation_h__
#define __rrlib__logging__messages__implementation_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdarg>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/log_levels.h"
#include "rrlib/logging/configuration/tConfiguration.h"
#include "rrlib/logging/messages/tStream.h"

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace logging
{

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

const tConfiguration &GetConfiguration(const char *filename, const char *domain_name = 0, const tDefaultConfigurationContext &default_context = cDEFAULT_CONTEXT);

void SendFormattedTimeToStream(tStream &stream);
void SendFormattedDomainNameToStream(tStream &stream, const std::string &domain_name);
void SetColor(tFormattingBuffer &stream_buffer, tLogLevel level);
void SendFormattedLevelToStream(tStream &stream, tLogLevel level);
void SendFormattedLocationToStream(tStream &stream, const char *filename, unsigned int line);



namespace
{
template <typename THead>
void SendDataToStream(tStream &stream, const THead &head)
{
  stream << head;
}

template <typename THead, typename ... TTail>
void SendDataToStream(tStream &stream, const THead &head, const TTail &... tail)
{
  stream << head;
  SendDataToStream(stream, tail...);
}
}



template <typename TLogDescription, typename ... TArgs>
void Print(const tConfiguration &domain_configuration, const TLogDescription &log_description, const char *function, const char *filename, unsigned int line, tLogLevel level, const TArgs &... args)
{
  if (level > domain_configuration.MaxMessageLevel())
  {
    return;
  }

  tStream stream(&domain_configuration.StreamBuffer());
  domain_configuration.StreamBuffer().InitializeMultiLinePadding();

  if (level != tLogLevel::USER)
  {

    if (domain_configuration.PrintsTime())
    {
      SendFormattedTimeToStream(stream);
    }

    SetColor(domain_configuration.StreamBuffer(), level);

#ifndef RRLIB_LOGGING_LESS_OUTPUT
    if (domain_configuration.PrintsName())
    {
      SendFormattedDomainNameToStream(stream, domain_configuration.GetFullQualifiedName());
    }
    if (domain_configuration.PrintsLevel())
    {
      SendFormattedLevelToStream(stream, level);
    }
#endif

    stream << log_description << "::" << function << " ";

#ifndef RRLIB_LOGGING_LESS_OUTPUT
    if (domain_configuration.PrintsLocation())
    {
      SendFormattedLocationToStream(stream, filename, line);
    }
#endif

    stream << ">> ";

    domain_configuration.StreamBuffer().ResetColor();

    switch (level)
    {
    case tLogLevel::ERROR:
      stream << "ERROR: ";
      break;
    case tLogLevel::WARNING:
    case tLogLevel::DEBUG_WARNING:
      stream << "WARNING: ";
      break;
    default:
      ;
    }

  }

  domain_configuration.StreamBuffer().MarkEndOfPrefixForMultiLinePadding();

  SendDataToStream(stream, args...);
}

template <typename TLogDescription>
void PrintFormatted(const tConfiguration &domain_configuration, const TLogDescription &log_description, const char *function, const char *filename, unsigned int line, tLogLevel level, const char *fmt, ...)
{
  if (level > domain_configuration.MaxMessageLevel())
  {
    return;
  }

  va_list printf_args0;
  va_start(printf_args0, fmt);
  char temp;
  int needed_buffer_size = vsnprintf(&temp, 1, fmt, printf_args0);
  va_end(printf_args0);

  va_list printf_args1;
  va_start(printf_args1, fmt);
  char buffer[needed_buffer_size + 1];
  vsnprintf(buffer, sizeof(buffer), fmt, printf_args1);
  va_end(printf_args1);

  Print(domain_configuration, log_description, function, filename, line, level, static_cast<const char *>(buffer));
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}



#endif
