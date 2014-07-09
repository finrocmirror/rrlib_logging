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
/*!\file    rrlib/logging/messages/tFormattingBuffer.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/messages/tFormattingBuffer.h"

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
#include "rrlib/logging/configuration/tDomainRegistry.h"
#include "rrlib/util/fstream/fileno.h"

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
// tFormattingBuffer constructors
//----------------------------------------------------------------------
tFormattingBuffer::tFormattingBuffer(std::streambuf *sink) :
  sink(sink),
  ends_with_newline(false),
  multi_line_pad_width(0),
  collect_multi_line_pad_width(false),
  pad_before_next_character(false)
{
  this->is_a_tty = isatty(util::GetFileDescriptor(this->sink));
}

tFormattingBuffer::tFormattingBuffer(const tFormattingBuffer &other) :
  sink(other.sink),
  ends_with_newline(other.ends_with_newline),
  multi_line_pad_width(other.multi_line_pad_width),
  collect_multi_line_pad_width(other.collect_multi_line_pad_width),
  pad_before_next_character(other.pad_before_next_character)
{
  this->is_a_tty = isatty(util::GetFileDescriptor(this->sink));
}

//----------------------------------------------------------------------
// tFormattingBuffer assignment operator
//----------------------------------------------------------------------
tFormattingBuffer &tFormattingBuffer::operator = (const tFormattingBuffer &other)
{
  if (this != &other)
  {
    this->sink = other.sink;
    this->ends_with_newline = other.ends_with_newline;
    this->multi_line_pad_width = other.multi_line_pad_width;
    this->collect_multi_line_pad_width = other.collect_multi_line_pad_width;
    this->pad_before_next_character = other.pad_before_next_character;
    this->is_a_tty = isatty(util::GetFileDescriptor(this->sink));
  }
  return *this;
}

//----------------------------------------------------------------------
// tFormattingBuffer SetColor
//----------------------------------------------------------------------
void tFormattingBuffer::SetColor(tFormattingBufferEffect effect, tFormattingBufferColor color)
{
  char control_sequence[16];
  const size_t length = snprintf(control_sequence, sizeof(control_sequence), "\033[;%u;3%um", effect, color);
  if (is_a_tty)
  {
    this->sink->sputn(control_sequence, length);
  }
}

//----------------------------------------------------------------------
// tFormattingBuffer ResetColor
//----------------------------------------------------------------------
void tFormattingBuffer::ResetColor()
{
  const char *control_sequence = "\033[;0m";
  const size_t length = strlen(control_sequence);
  if (is_a_tty)
  {
    this->sink->sputn(control_sequence, length);
  }
}

//----------------------------------------------------------------------
// tFormattingBuffer InitializeMultiLinePadding
//----------------------------------------------------------------------
void tFormattingBuffer::InitializeMultiLinePadding()
{
  if (!tDomainRegistry::Instance().GetPadMultiLineMessages())
  {
    return;
  }
  this->multi_line_pad_width = 0;
  this->collect_multi_line_pad_width = true;
  this->pad_before_next_character = false;
}

//----------------------------------------------------------------------
// tFormattingBuffer MarkEndOfPrefixForMultiLinePadding
//----------------------------------------------------------------------
void tFormattingBuffer::MarkEndOfPrefixForMultiLinePadding()
{
  this->collect_multi_line_pad_width = false;
}

//----------------------------------------------------------------------
// tFormattingBuffer overflow
//----------------------------------------------------------------------
tFormattingBuffer::int_type tFormattingBuffer::overflow(int_type c)
{
  if (c == traits_type::eof())
  {
    return traits_type::eof();
  }

  this->SetEndsWithNewline(c == '\n');

  int result = c;
  if (this->pad_before_next_character)
  {
    for (size_t i = 0; i < this->multi_line_pad_width; ++i)
    {
      result = this->sink->sputc(' ');
    }
    this->pad_before_next_character = false;
  }

  result = this->sink->sputc(c);

  if (this->collect_multi_line_pad_width)
  {
    this->multi_line_pad_width = this->EndsWithNewline() ? 0 : this->multi_line_pad_width + 1;
  }
  else
  {
    this->pad_before_next_character = this->EndsWithNewline();
  }

  return result;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
