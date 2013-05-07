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
/*!\file    rrlib/logging/messages/tStreamBuffer.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-23
 *
 * \brief   Contains tStreamBuffer
 *
 * \b tStreamBuffer
 *
 * tStreamBuffer is an implementation for std::ostreams with multiple
 * sinks. Using a std::ostream for output internally uses a std::streambuf
 * for the low level operations necessary to print stream input to a file
 * or terminal. By specializing std::streambuf in this class it is possible
 * to collect the streambuffers of several ostreams and supply all these
 * streams with own input, like tee in UNIX shells.
 *
 * Having an empty buffer vector also implements a null stream that does
 * not open /dev/null to swallow all input.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/messages.h" instead.
#endif

#ifndef __rrlib__logging__tStreamBuffer_h__
#define __rrlib__logging__tStreamBuffer_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <iostream>
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
enum tStreamBufferEffect
{
  eSBE_REGULAR,
  eSBE_BOLD,
  eSBE_DARK,
  eSBE_UNDERLINED = 4,
  eSBE_BLINKING,
  eSBE_INVERTED = 7,
  eSBE_CONCEALED,
  eSBE_DIMENSION
};

enum tStreamBufferColor
{
  eSBC_DEFAULT,
  eSBC_RED,
  eSBC_GREEN,
  eSBC_YELLOW,
  eSBC_BLUE,
  eSBC_MAGENTA,
  eSBC_CYAN,
  eSBC_GRAY,
  eSBC_DIMENSION
};

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! A streambuffer implementation for std::ostreams with multiple sinks
/*! Using a std::ostream for output internally uses a std::streambuf for
 *  the low level operations necessary to print stream input to a file
 *  or terminal. By specializing std::streambuf in this class it is
 *  possible to collect the streambuffers of several ostreams and supply
 *  all these streams with own input, like tee in UNIX shells.
 *
 *  Having an empty buffer vector also implements a null stream that does
 *  not open /dev/null to swallow all input.
 *
 */
class tStreamBuffer : public std::streambuf
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*! The ctor of tStreamBuffer
   *
   */
  tStreamBuffer();

  /*! Add a target output stream to this buffer
   *
   * This buffer forwards its input to a list of target streams.
   * This method is used to add such a stream to the list.
   *
   * \param stream   The output stream that should be added as target
   */
  inline void AddStream(std::ostream &stream)
  {
    this->buffers.push_back(stream.rdbuf());
  }

  /*! Clear the buffer's list of target streams
   *
   * This method completely clears the list of target streams.
   * After calling, the stream that uses this buffer acts as
   * null stream, swallowing every input without generating any
   * output.
   */
  inline void Clear()
  {
    this->buffers.clear();
  }

  /*! Find out whether the last character put into this stream was a newline or not
   *
   * \returns Whether the last character put into this stream was \n or not
   */
  inline bool EndsWithNewline() const
  {
    return this->ends_with_newline;
  }

  void SetColor(tStreamBufferEffect effect, tStreamBufferColor color);

  void ResetColor();

  void InitializeMultiLinePadding();

  void MarkEndOfPrefixForMultiLinePadding();

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  std::vector<std::streambuf *> buffers;

  bool ends_with_newline;

  size_t multi_line_pad_width;
  bool collect_multi_line_pad_width;
  bool pad_before_next_character;

  int WriteCharacterToBuffers(int c);

  virtual int overflow(int c);

  virtual int sync();

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
