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
 * \brief
 *
 * \b
 *
 * A few words for tLogStreamBuffer.cpp
 *
 */
//----------------------------------------------------------------------
#define _rrlib_logging_include_guard_
#include "logging/tLogStreamBuffer.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <cstdio>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------
using namespace rrlib::logging;

//----------------------------------------------------------------------
// Forward declarations
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// class tLogStreamBuffer overflow
//----------------------------------------------------------------------
int tLogStreamBuffer::overflow(int c)
{
  if (c == EOF)
  {
    return !EOF;
  }

  this->ends_with_newline = c == '\n';

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
// class tLogStreamBuffer sync
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
