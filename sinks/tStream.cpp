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
/*!\file    rrlib/logging/sinks/tStream.cpp
 *
 * \author  Tobias Föhst
 *
 * \date    2013-08-07
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/sinks/tStream.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <iostream>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

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
namespace sinks
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
// tStream constructors
//----------------------------------------------------------------------
tStream::tStream(const xml::tNode &node, const tConfiguration &configuration) :
  stream_buffer(NULL)
{
  if (!node.HasAttribute("id"))
  {
    throw std::runtime_error("Attribute id is missing for stream logging sink!");
  }

  std::string id = node.GetStringAttribute("id");
  if (id == "stdout")
  {
    this->stream_buffer = tFormattingBuffer(std::cout.rdbuf());
    return;
  }
  if (id == "stderr")
  {
    this->stream_buffer = tFormattingBuffer(std::cerr.rdbuf());
    return;
  }

  throw std::runtime_error("Could not identify and use stream for logging");
}

//----------------------------------------------------------------------
// tStream GetStreamBuffer
//----------------------------------------------------------------------
std::streambuf &tStream::GetStreamBuffer()
{
  return this->stream_buffer;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
}
