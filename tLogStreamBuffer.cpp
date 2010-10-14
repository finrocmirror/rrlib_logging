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
/*!\file    tLogStreamBuffer.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "rrlib/logging/tLogStreamBuffer.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdio>
#include <cstring>

extern "C"
{
#include <unistd.h>
}

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/tLogDomainRegistry.h"
#include "rrlib/util/fstream/fileno.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::logging;

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
// tLogStreamBuffer WriteCharacterToBuffers
//----------------------------------------------------------------------
int tLogStreamBuffer::WriteCharacterToBuffers(int c)
{
  int result = c;
  for (std::vector<std::streambuf *>::iterator it = this->buffers.begin(); it != this->buffers.end(); ++it)
  {
    if ((*it)->sputc(c) == EOF)
    {
      result = EOF;
    }
  }
  return result;
}

//----------------------------------------------------------------------
// tLogStreamBuffer overflow
//----------------------------------------------------------------------
int tLogStreamBuffer::overflow(int c)
{
  if (c == EOF)
  {
    return !EOF;
  }

  this->ends_with_newline = c == '\n';

  int result = c;
  if (this->pad_before_next_character)
  {
    for (size_t i = 0; i < this->multi_line_pad_width; ++i)
    {
      result = this->WriteCharacterToBuffers(' ');
    }
    this->pad_before_next_character = false;
  }

  if (result != EOF)
  {
    result = this->WriteCharacterToBuffers(c);
  }

  if (this->collect_multi_line_pad_width)
  {
    this->multi_line_pad_width = this->ends_with_newline ? 0 : this->multi_line_pad_width + 1;
  }
  else
  {
    this->pad_before_next_character = this->ends_with_newline;
  }

  return result;
}

//----------------------------------------------------------------------
// tLogStreamBuffer sync
//----------------------------------------------------------------------
int tLogStreamBuffer::sync()
{
  int result = 0;
  for (std::vector<std::streambuf *>::iterator it = this->buffers.begin(); it != this->buffers.end(); ++it)
  {
    if ((*it)->pubsync() != 0)
    {
      result = -1;
    }
  }
  return result;
}

//----------------------------------------------------------------------
// tLogStreamBuffer SetColor
//----------------------------------------------------------------------
void tLogStreamBuffer::SetColor(tLogStreamBufferEffect effect, tLogStreamBufferColor color)
{
  char control_sequence[16];
  snprintf(control_sequence, sizeof(control_sequence), "\033[;%u;3%um", effect, color);
  const size_t length = strlen(control_sequence);
  for (std::vector<std::streambuf *>::iterator it = this->buffers.begin(); it != this->buffers.end(); ++it)
  {
    int file_descriptor = rrlib::util::GetFileDescriptor(*it);
    if (file_descriptor > 0 && isatty(file_descriptor))
    {
      for (size_t i = 0; i < length; ++i)
      {
        (*it)->sputc(control_sequence[i]);
      }
    }
  }
}

//----------------------------------------------------------------------
// tLogStreamBuffer ResetColor
//----------------------------------------------------------------------
void tLogStreamBuffer::ResetColor()
{
  const char *control_sequence = "\033[;0m";
  const size_t length = strlen(control_sequence);
  for (std::vector<std::streambuf *>::iterator it = this->buffers.begin(); it != this->buffers.end(); ++it)
  {
    int file_descriptor = rrlib::util::GetFileDescriptor(*it);
    if (file_descriptor > 0 && isatty(file_descriptor))
    {
      for (size_t i = 0; i < length; ++i)
      {
        (*it)->sputc(control_sequence[i]);
      }
    }
  }
}

//----------------------------------------------------------------------
// tLogStreamBuffer InitializeMultiLinePadding
//----------------------------------------------------------------------
void tLogStreamBuffer::InitializeMultiLinePadding()
{
  if (!tLogDomainRegistry::GetInstance()->GetPadMultiLineMessages())
  {
    return;
  }
  this->multi_line_pad_width = 0;
  this->collect_multi_line_pad_width = true;
  this->pad_before_next_character = false;
}

//----------------------------------------------------------------------
// tLogStreamBuffer MarkEndOfPrefixForMultiLinePadding
//----------------------------------------------------------------------
void tLogStreamBuffer::MarkEndOfPrefixForMultiLinePadding()
{
  this->collect_multi_line_pad_width = false;
}

