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
/*!\file    messages/implementation.h
 *
 * \author  Tobias Foehst
 *
 * \date    2011-09-15
 *
 * \brief Contains the implementation of message printing
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
#include <iostream>
#include <cstdarg>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/log_levels.h"
#include "rrlib/logging/configuration/tLogDomainRegistry.h"
#include "rrlib/logging/configuration/tConfiguration.h"

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
//! The default description for global debugging. Can be set to whatever string the user likes to set it to
extern char *default_log_description;

const tConfiguration &GetConfiguration(const char *filename, const char *domain_name = 0);

template <typename TLogDescription, typename ... TArgs>
void Print(const tConfiguration &domain_configuration, const TLogDescription &log_description, const char *function, const char *filename, unsigned int line, tLogLevel level, const TArgs &... args)
{
  if (level > domain_configuration.MaxMessageLevel())
  {
    return;
  }

  std::cout << domain_configuration.GetFullQualifiedName() << " " << log_description << std::endl;

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
