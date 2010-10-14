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
/*!\file    tLogStreamBuffer.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-23
 *
 * \brief Contains tLogStreamBuffer
 *
 * \b tLogStreamBuffer
 *
 * tLogStreamBuffer is an implementation for std::ostreams with
 * multiple sinks. Using a std::ostream for output internally uses a
 * std::streambuf for the low level operations necessary to print the
 * stream input to a file or terminal. By specializing std::streambuf in
 * this class it is possible to collect the streambuffers of several
 * ostreams and supply all these streams with own input,  like tee in
 * UNIX shells.
 *
 * Having an empty buffer vector also implements a null stream that does
 * not open /dev/null to swallow all input.
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_logging_include_guard_
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef _rrlib_logging_tLogStreamBuffer_h_
#define _rrlib_logging_tLogStreamBuffer_h_

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
enum tLogStreamBufferEffect
{
  eLSBE_REGULAR,
  eLSBE_BOLD,
  eLSBE_DARK,
  eLSBE_UNDERLINED = 4,
  eLSBE_BLINKING,
  eLSBE_INVERTED = 7,
  eLSBE_CONCEALED,
  eLSBE_DIMENSION
};

enum tLogStreamBufferColor
{
  eLSBC_DEFAULT,
  eLSBC_RED,
  eLSBC_GREEN,
  eLSBC_YELLOW,
  eLSBC_BLUE,
  eLSBC_MAGENTA,
  eLSBC_CYAN,
  eLSBC_GRAY,
  eLSBC_DIMENSION
};

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! A streambuffer implementation for std::ostreams with multiple sinks
/*! Using a std::ostream for output internally uses a std::streambuf for
 *  the low level operations necessary to print the stream input to a
 *  file or terminal. By specializing std::streambuf in this class it is
 *  possible to collect the streambuffers of several ostreams and supply
 *  all these streams with own input, like tee in UNIX shells.
 *  Having an empty buffer vector also implements a null stream that does
 *  not open /dev/null to swallow all input.
 *
 */
class tLogStreamBuffer : public std::streambuf
{
  std::vector<std::streambuf *> buffers;

  bool ends_with_newline;

  size_t multi_line_pad_width;
  bool collect_multi_line_pad_width;
  bool pad_before_next_character;

  int WriteCharacterToBuffers(int c);

  virtual int overflow(int c);

  virtual int sync();

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! The ctor of tLogStreamBuffer
   *
   */
  tLogStreamBuffer()
      : ends_with_newline(false),
      multi_line_pad_width(0),
      collect_multi_line_pad_width(false),
      pad_before_next_character(false)
  {}

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

  void SetColor(tLogStreamBufferEffect effect, tLogStreamBufferColor color);

  void ResetColor();

  void InitializeMultiLinePadding();

  void MarkEndOfPrefixForMultiLinePadding();

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
