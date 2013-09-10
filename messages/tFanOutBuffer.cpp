//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
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
/*!\file    rrlib/logging/messages/tFanOutBuffer.cpp
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/messages/tFanOutBuffer.h"

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
// tFanOutBuffer constructors
//----------------------------------------------------------------------
tFanOutBuffer::tFanOutBuffer() :
  tFormattingBuffer(NULL)
{}

//----------------------------------------------------------------------
// tFanOutBuffer SetColor
//----------------------------------------------------------------------
void tFanOutBuffer::SetColor(tFormattingBufferEffect effect, tFormattingBufferColor color)
{
  for (auto it = this->formatting_buffers.begin(); it != this->formatting_buffers.end(); ++it)
  {
    it->SetColor(effect, color);
  }
}

//----------------------------------------------------------------------
// tFanOutBuffer ResetColor
//----------------------------------------------------------------------
void tFanOutBuffer::ResetColor()
{
  for (auto it = this->formatting_buffers.begin(); it != this->formatting_buffers.end(); ++it)
  {
    it->ResetColor();
  }
}

//----------------------------------------------------------------------
// tFanOutBuffer InitializeMultiLinePadding
//----------------------------------------------------------------------
void tFanOutBuffer::InitializeMultiLinePadding()
{
  for (auto it = this->formatting_buffers.begin(); it != this->formatting_buffers.end(); ++it)
  {
    it->InitializeMultiLinePadding();
  }
}

//----------------------------------------------------------------------
// tFanOutBuffer MarkEndOfPrefixForMultiLinePadding
//----------------------------------------------------------------------
void tFanOutBuffer::MarkEndOfPrefixForMultiLinePadding()
{
  for (auto it = this->formatting_buffers.begin(); it != this->formatting_buffers.end(); ++it)
  {
    it->MarkEndOfPrefixForMultiLinePadding();
  }
}

//----------------------------------------------------------------------
// tFanOutBuffer overflow
//----------------------------------------------------------------------
tFanOutBuffer::int_type tFanOutBuffer::overflow(int_type c)
{
  if (c == traits_type::eof())
  {
    return traits_type::eof();
  }

  this->SetEndsWithNewline(c == '\n');

  int_type result = c;
  for (auto it = this->formatting_buffers.begin(); it != this->formatting_buffers.end(); ++it)
  {
    if (it->sputc(c) == traits_type::eof())
    {
      result = traits_type::eof();
    }
  }
  for (auto it = this->buffers.begin(); it != this->buffers.end(); ++it)
  {
    if ((*it)->sputc(c) == traits_type::eof())
    {
      result = traits_type::eof();
    }
  }
  return result;
}

//----------------------------------------------------------------------
// tFanOutBuffer sync
//----------------------------------------------------------------------
int tFanOutBuffer::sync()
{
  int result = 0;
  for (auto it = this->formatting_buffers.begin(); it != this->formatting_buffers.end(); ++it)
  {
    if (it->pubsync() != 0)
    {
      result = -1;
    }
  }
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
// End of namespace declaration
//----------------------------------------------------------------------
}
}
