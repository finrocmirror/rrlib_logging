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
/*!\file    tLogStream.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-30
 *
 * \brief Contains tLogStreamContext
 *
 * \b tLogStream
 *
 * This proxy class for std::ostream allows synchronized streaming.
 * Streaming typically has the problem that one can not easily determine
 * the time all output for one message is processed. Thus, locking the
 * stream for synchronized output is not directly possible.
 *
 * By returning a proxy for each output stream the logging domain creates
 * creates a temporary object that lives as long as consecutive streaming
 * operations are performed. Afterwards, the proxy will be destructed
 * immediately. Hence, the correct times for acquiring and releasing the
 * lock are defined.
 *
 * Assigning the proxy object to a local variable allows locking over
 * multiple lines of code if this is really necessary. However, creation
 * on the heap is not supported to avoid dangling locks.
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_logging_include_guard_
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef rrlib_logging_tLogStreamContext_h_
#define rrlib_logging_tLogStreamContext_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <iostream>
#include <boost/thread/recursive_mutex.hpp>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/tLogStreamBuffer.h"

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
//! A proxy class for std::ostream that allows synchronized streaming
/*! Using streaming has the problem that one can not easily determine
 *  the time all output for one message is processed. Thus, locking the
 *  stream for synchronized output is not directly possible.
 *
 *  By returning a proxy for each output stream the logging domain creates
 *  creates a temporary object that lives as long as consecutive streaming
 *  operations are performed. Afterwards, the proxy will be desctructed
 *  immediately. Hence, the correct times for acquiring and releasing the
 *  lock are defined.
 *
 *  Assigning the proxy object to a local variable allows locking over
 *  multiple lines of code if this is really necessary. However, creation
 *  on the heap is not supported to avoid dangling locks.
 *
 */
class tLogStreamContext
{
  std::ostream stream;

  boost::recursive_mutex *mutex;

  // Prohibit copy
  tLogStreamContext(const tLogStreamContext &other);

  // Prohibit assignment
  tLogStreamContext &operator = (const tLogStreamContext &other);

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! The ctor of tLogStreamContext
   *
   * Acquires a lock for the output.
   * This methods blocks until the lock can be acquired.
   *
   * \param stream   The std::ostream that is used via this proxy
   * \param mutex    Mutex to acquire while this proxy is used
   */
  tLogStreamContext(tLogStreamBuffer &stream_buffer, boost::recursive_mutex *mutex)
      : stream(&stream_buffer),
      mutex(mutex)
  {
    mutex->lock();
  }

  /*! The dtor of tLogStreamContext
   *
   * Releases the lock, adds a newline if necessary and flushes the stream
   */
  ~tLogStreamContext()
  {
    tLogStreamBuffer *buffer = dynamic_cast<tLogStreamBuffer *>(this->stream.rdbuf());
    if (buffer && buffer->EndsWithNewline())
    {
      this->GetStream() << std::flush;
    }
    else
    {
      this->GetStream() << std::endl;
    }
    mutex->unlock();
  }

  inline std::ostream &GetStream()
  {
    return this->stream;
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
