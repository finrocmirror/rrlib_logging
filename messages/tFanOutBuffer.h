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
/*!\file    rrlib/logging/messages/tFanOutBuffer.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-23
 *
 * \brief   Contains tFanOutBuffer
 *
 * \b tFanOutBuffer
 *
 * tFanOutBuffer is a specialization of std::streambuf with multiple
 * sinks. Using a std::ostream for output internally uses a std::streambuf
 * for the low level operations necessary to print stream input to a file
 * or terminal, etc. By specializing std::streambuf in this class it is
 * possible to fan out the stream's content to several other stream buffers,
 * like tee in UNIX shells.
 *
 * Having an empty buffer vector also implements a null stream that does
 * not open /dev/null to swallow all input.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/messages.h" instead.
#endif

#ifndef __rrlib__logging__tFanOutBuffer_h__
#define __rrlib__logging__tFanOutBuffer_h__

#include "rrlib/logging/messages/tFormattingBuffer.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <streambuf>
#include <vector>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
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
// Class declaration
//----------------------------------------------------------------------
//! A specialization of std::streambuf with multiple sinks
/*! Using a std::ostream for output internally uses a std::streambuf
 * for the low level operations necessary to print stream input to a file
 * or terminal, etc. By specializing std::streambuf in this class it is
 * possible to fan out the stream's content to several other stream buffers,
 * like tee in UNIX shells.
 *
 *  Having an empty buffer vector also implements a null stream that does
 *  not open /dev/null to swallow all input.
 *
 */
class tFanOutBuffer : public tFormattingBuffer
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  tFanOutBuffer();

  /*! Add a sink (stream buffer) to this buffer
   *
   * This buffer forwards its input to a list of target stream buffers.
   * This method is used to add such a sink to the list.
   *
   * \param stream_buffer   The output stream buffer that should be added as sink
   */
  inline void AddSink(tFormattingBuffer stream_buffer)
  {
    this->formatting_buffers.push_back(stream_buffer);
  }

  inline void AddSink(std::streambuf &stream_buffer)
  {
    try
    {
      tFormattingBuffer &formatting_buffer = dynamic_cast<tFormattingBuffer &>(stream_buffer);
      this->formatting_buffers.push_back(formatting_buffer);
    }
    catch (std::bad_cast)
    {
      this->buffers.push_back(&stream_buffer);
    }
  }

  /*! Clear the buffer's list of sinks
   *
   * This method completely clears the list of sinks.
   * After calling, the stream that uses this buffer acts as
   * null stream, swallowing every input without generating any
   * output.
   */
  inline void Clear()
  {
    this->formatting_buffers.clear();
    this->buffers.clear();
  }

  virtual void SetColor(tFormattingBufferEffect effect, tFormattingBufferColor color);

  virtual void ResetColor();

  virtual void InitializeMultiLinePadding();

  virtual void MarkEndOfPrefixForMultiLinePadding();


//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  std::vector<tFormattingBuffer> formatting_buffers;
  std::vector<std::streambuf *> buffers;

  virtual int_type overflow(int_type c);

  virtual int sync();

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
