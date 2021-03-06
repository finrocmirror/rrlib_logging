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
/*!\file    rrlib/logging/messages/implementation.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2011-09-15
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/messages/implementation.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdio>
#include <ctime>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
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
// GetConfiguration
//----------------------------------------------------------------------
const tConfiguration &GetConfiguration(const char *filename, const char *domain_name, const tDefaultConfigurationContext &default_context)
{
  return tDomainRegistry::Instance().GetConfiguration(default_context, filename, domain_name);
}

//----------------------------------------------------------------------
// SendFormattedTimeToStream
//----------------------------------------------------------------------
void SendFormattedTimeToStream(tStream &stream)
{
#if __linux__
  char time_string_buffer[9];
  timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  strftime(time_string_buffer, sizeof(time_string_buffer), "%T", localtime(&time.tv_sec));
  char nsec_string_buffer[11];
  snprintf(nsec_string_buffer, sizeof(nsec_string_buffer), ".%09ld", time.tv_nsec);
  stream << "[ " << time_string_buffer << nsec_string_buffer << " ] ";
#endif
}

//----------------------------------------------------------------------
// SetColor
//----------------------------------------------------------------------
void SetColor(tFormattingBuffer &stream_buffer, tLogLevel level)
{
  switch (level)
  {
  case tLogLevel::ERROR:
    stream_buffer.SetColor(eSBE_BOLD, eSBC_RED);
    break;
  case tLogLevel::WARNING:
    stream_buffer.SetColor(eSBE_BOLD, eSBC_BLUE);
    break;
  case tLogLevel::DEBUG_WARNING:
    stream_buffer.SetColor(eSBE_DARK, eSBC_YELLOW);
    break;
  case tLogLevel::DEBUG:
    stream_buffer.SetColor(eSBE_DARK, eSBC_GREEN);
    break;
  case tLogLevel::DEBUG_VERBOSE_1:
    stream_buffer.SetColor(eSBE_REGULAR, eSBC_CYAN);
    break;
  case tLogLevel::DEBUG_VERBOSE_2:
    stream_buffer.SetColor(eSBE_REGULAR, eSBC_CYAN);
    break;
  case tLogLevel::DEBUG_VERBOSE_3:
    stream_buffer.SetColor(eSBE_REGULAR, eSBC_CYAN);
    break;
  default:
    ;
  }
}

//----------------------------------------------------------------------
// SendFormattedDomainNameToStream
//----------------------------------------------------------------------
void SendFormattedDomainNameToStream(tStream &stream, const std::string &domain_name)
{
  char name_string_buffer[128];
  snprintf(name_string_buffer, sizeof(name_string_buffer), "%-*s ", static_cast<int>((tDomainRegistry::Instance().GetPadPrefixColumns() ? tDomainRegistry::Instance().MaxDomainNameLength() : 0)), domain_name.c_str());
  stream << name_string_buffer;
}

//----------------------------------------------------------------------
// SendFormattedLevelToStream
//----------------------------------------------------------------------
void SendFormattedLevelToStream(tStream &stream, tLogLevel level)
{
  const char *level_name = 0;
  switch (level)
  {
  case tLogLevel::ERROR:
    level_name = "[error]";
    break;
  case tLogLevel::WARNING:
    level_name = "[warning]";
    break;
  case tLogLevel::DEBUG_WARNING:
    level_name = "[debug]";
    break;
  case tLogLevel::DEBUG:
    level_name = "[debug]";
    break;
  case tLogLevel::DEBUG_VERBOSE_1:
    level_name = "[verbose]";
    break;
  case tLogLevel::DEBUG_VERBOSE_2:
    level_name = "[verbose]";
    break;
  case tLogLevel::DEBUG_VERBOSE_3:
    level_name = "[verbose]";
    break;
  default:
    level_name = "";
    break;
  }
  char name_string_buffer[128];
  snprintf(name_string_buffer, sizeof(name_string_buffer), "%-*s ", (tDomainRegistry::Instance().GetPadPrefixColumns() ? 9 : 0), level_name);
  stream << name_string_buffer;
}

//----------------------------------------------------------------------
// SendFormattedLocationToStream
//----------------------------------------------------------------------
void SendFormattedLocationToStream(tStream &stream, const char *filename, unsigned int line)
{
  char location_string_buffer[128];
  snprintf(location_string_buffer, sizeof(location_string_buffer), "[%s:%u] ", filename, line);
  stream << location_string_buffer;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
