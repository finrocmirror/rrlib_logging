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
/*!\file    rrlib/logging/sinks/tStream.h
 *
 * \author  Tobias Föhst
 *
 * \date    2013-08-07
 *
 * \brief   Contains tStream
 *
 * \b tStream
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/configuration.h" instead.
#endif

#ifndef __rrlib__logging__sinks__tStream_h__
#define __rrlib__logging__sinks__tStream_h__

#include "rrlib/logging/sinks/tSink.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#ifdef _LIB_RRLIB_XML_PRESENT_
#include "rrlib/xml/tNode.h"
#endif

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/messages/tFormattingBuffer.h"

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
// Class declaration
//----------------------------------------------------------------------
//! SHORT_DESCRIPTION
/*!
 */
class tStream : public tSink
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  tStream(const std::string &id);
#ifdef _LIB_RRLIB_XML_PRESENT_
  tStream(const xml::tNode &node, const tConfiguration &configuration);
#endif

  virtual std::streambuf &GetStreamBuffer();

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  tFormattingBuffer stream_buffer;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
}


#endif
