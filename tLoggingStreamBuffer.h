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
/*!\file    tLoggingStreamBuffer.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-23
 *
 * \brief Contains tLoggingStreamBuffer
 *
 * \b tLoggingStreamBuffer
 *
 * tLoggingStreamBuffer is an implementation for std::ostreams with
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

#ifndef rrlib_logging_tLoggingStreamBuffer_h_
#define rrlib_logging_tLoggingStreamBuffer_h_

//----------------------------------------------------------------------
// External includes with <>
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
class tLoggingStreamBuffer : public std::streambuf
{
  std::vector<std::streambuf *> buffers;

  virtual int overflow(int c);

  virtual int sync();

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

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

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
