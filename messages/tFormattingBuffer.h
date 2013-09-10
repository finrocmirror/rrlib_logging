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
/*!\file    rrlib/logging/messages/tFormattingBuffer.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-23
 *
 * \brief   Contains tFormattingBuffer
 *
 * \b tFormattingBuffer
 *
 * tFormattingBuffer is a specialization of std::streambuf with formatting
 * capabilities. This stream buffer class implements functionality for decorated
 * terminal output and padding to align its output according to a prefix.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__tFormattingBuffer_h__
#define __rrlib__logging__tFormattingBuffer_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <streambuf>

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
enum tFormattingBufferEffect
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

enum tFormattingBufferColor
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
//! A specialization of std::streambuf with formatting capabilities
/*! This stream buffer class implements functionality for decorated
 * terminal output and padding to align its output according to a prefix.
 *
 */
class tFormattingBuffer : public std::streambuf
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*! The ctor of tFormattingBuffer
   *
   */
  explicit tFormattingBuffer(std::streambuf *sink);

  tFormattingBuffer(const tFormattingBuffer &other);

  tFormattingBuffer &operator = (const tFormattingBuffer &other);

  /*! Find out whether the last character put into this stream was a newline or not
   *
   * \returns Whether the last character put into this stream was \n or not
   */
  virtual bool EndsWithNewline() const
  {
    return this->ends_with_newline;
  }

  virtual void SetColor(tFormattingBufferEffect effect, tFormattingBufferColor color);

  virtual void ResetColor();

  virtual void InitializeMultiLinePadding();

  virtual void MarkEndOfPrefixForMultiLinePadding();

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  inline void SetEndsWithNewline(bool value)
  {
    this->ends_with_newline = value;
  }

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  std::streambuf *sink;

  bool ends_with_newline;

  size_t multi_line_pad_width;
  bool collect_multi_line_pad_width;
  bool pad_before_next_character;

  virtual int_type overflow(int_type c);

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
