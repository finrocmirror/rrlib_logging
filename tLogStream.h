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
 * \date    2010-07-06
 *
 * \brief Contains tLogStream
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
 * operations are performed. Afterwards, the proxy will be desctructed
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

#ifndef rrlib_logging_tLogStreamProxy_h_
#define rrlib_logging_tLogStreamProxy_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <iostream>
#include <exception>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/thread/recursive_mutex.hpp>

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
class tLogStream
{
  std::ostream &stream;

  boost::recursive_mutex* mutex;

  // Prohibit assignment to non-const references
  tLogStream &operator = (const tLogStream &other);

  // Prohibit creation on heap
  void *operator new(size_t size);

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! The ctor of tLogStream
   *
   * Acquires a lock for the output.
   * This methods blocks until the lock can be acquired.
   *
   * \param stream   The std::ostream that is used via this proxy
   * \param mutex_   Mutex to acquire while this proxy is used
   */
  explicit tLogStream(std::ostream &stream, boost::recursive_mutex* mutex_)
      : stream(stream),
      mutex(mutex_)
  {
    mutex->lock();
  }

  /*! The dtor of tLogStreamProxy
   *
   * Releases the lock, adds a newline if necessary and flushes the stream
   */
  ~tLogStream()
  {
    tLogStreamBuffer *buffer = dynamic_cast<tLogStreamBuffer *>(this->stream.rdbuf());
    if (buffer && buffer->EndsWithNewline())
    {
      *this << std::flush;
    }
    else
    {
      *this << std::endl;
    }
    mutex->unlock();
  }

  /*! Streaming operator (forwarder)
   *
   * This method forwards the streaming operation to the wrapped
   * stream object
   *
   * \param value   The object to put into the stream
   *
   * \returns A reference to the altered stream (in this case the proxy)
   */
  template <typename T>
  inline typename boost::disable_if<boost::is_base_of<std::exception, T>, tLogStream &>::type operator << (const T &value)
  {
    this->stream << value;
    return *this;
  }

  /*! Streaming operator for exceptions
   *
   * This method implements log streaming for std::exception
   *
   * \param exception   The exception to put into the stream
   *
   * \returns A reference to the altered stream (in this case the proxy)
   */
  inline tLogStream &operator << (const std::exception &exception)
  {
    this->stream << "Exception (" << typeid(exception).name() << "): " << exception.what();
    return *this;
  }

  /*! Streaming operator for functions (forwarder)
   *
   * This method enables manipulators like std::endl, etc. for
   * this proxy class by calling their functions on the wrapped
   * stream
   *
   * \param manipulator   The function pointer for the manipulation
   *
   * \returns A reference to the altered stream (in this case the proxy)
   */
  inline tLogStream &operator << (std::ostream &(&manipulator)(std::ostream &))
  {
    manipulator(this->stream);
    return *this;
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
