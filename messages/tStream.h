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
/*!\file    rrlib/logging/messages/tStream.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-07-06
 *
 * \brief   Contains tStream
 *
 * \b tStream
 *
 * This proxy class for std::ostream allows synchronized streaming.
 * Streaming typically has the problem that one can not easily determine
 * the time all output for one message is processed. Thus, locking the
 * stream for synchronized output is not directly possible.
 *
 * By returning a proxy for each output stream the logging domain creates
 * a temporary object that lives as long as consecutive streaming
 * operations are performed. Afterwards, the proxy will be destroyed
 * immediately. Hence, the correct times for acquiring and releasing the
 * lock are defined.
 *
 * Assigning the proxy object to a local variable allows locking over
 * multiple lines of code if this is really necessary. However, creation
 * on the heap is not supported to avoid dangling locks.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/messages.h" instead.
#endif

#ifndef __rrlib__logging__tStream_h__
#define __rrlib__logging__tStream_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <iostream>
#include <mutex>

#include <exception>
#include "rrlib/time/time.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/messages/tStreamBuffer.h"

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
 *  a temporary object that lives as long as consecutive streaming
 *  operations are performed. Afterwards, the proxy will be destroyed
 *  immediately. Hence, the correct times for acquiring and releasing the
 *  lock are defined.
 *
 *  Assigning the proxy object to a local variable allows locking over
 *  multiple lines of code if this is really necessary. However, creation
 *  on the heap is not supported to avoid dangling locks.
 *
 */
class tStream
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*! The ctor of tStream
   *
   * Acquires a lock for the output.
   * This methods blocks until the lock can be acquired.
   *
   * \param stream   The std::ostream that is used via this proxy
   */
  explicit tStream(tStreamBuffer &stream_buffer);

  /*! The ctor of tStream
   *
   * Releases the lock for the output and takes care of flushing and trailing newlines.
   */
  ~tStream();

  /*! Implicit conversion to std::ostream for e.g. std::ostream_iterator
   *
   * To use this proxy class together with e.g. std::copy, which needs
   * a std::ostream_iterator, this method is implemented.
   *
   * \returns A reference to the underlying std::ostream
   */
  inline operator std::ostream &()
  {
    return this->stream;
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
  inline typename std::enable_if < !std::is_base_of<std::exception, T>::value, tStream >::type &operator << (const T &value)
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
  inline tStream &operator << (const std::exception &exception)
  {
    this->stream << "Exception (" << typeid(exception).name() << "): " << exception.what();
    return *this;
  }
  inline tStream &operator << (std::exception &exception)
  {
    this->stream << "Exception (" << typeid(exception).name() << "): " << exception.what();
    return *this;
  }

  /*! Streaming operator for bool values
   *
   * This method implements more appropriate log streaming for bool values,
   * printing "<true>" and "<false>" instead of 1 and 0.
   *
   * \param value   The bool value to put into the stream
   *
   * \returns A reference to the altered stream (in this case the proxy)
   */
  inline tStream &operator << (bool value)
  {
    this->stream << (value ? "<true>" : "<false>");
    return *this;
  }

  /*! Streaming operator for bool values
   *
   * This method implements more appropriate log streaming for bool values,
   * printing "<true>" and "<false>" instead of 1 and 0.
   *
   * \param value   The bool value to put into the stream
   *
   * \returns A reference to the altered stream (in this case the proxy)
   */
  inline tStream &operator << (char value)
  {
    if (value == 0)
    {
      this->stream << "<null>";
      return *this;
    }
    this->stream << value;
    return *this;
  }

  /*! Streaming operator for timestamps
   *
   * There are no iostream operators for the std::chrono::time_point
   * template. Therefore, we need an operator implementation here.
   *
   * \param value   The duration value to put into the stream.
   *
   * \returns A reference to the altered stream (in this case the proxy)
   */
  inline tStream &operator << (const rrlib::time::tTimestamp& value)
  {
    this->stream << rrlib::time::ToIsoString(value);
    return *this;
  }

  /*! Streaming operator for pointers
   *
   * This method implements more appropriate log streaming for pointer
   * types, printing "<nullptr>" instead of segfaulting in case of char *.
   * Also with other pointer types, seeing the string "<nullptr>" can be
   * easily understood.
   *
   * \param exception   The pointer to put into the stream
   *
   * \returns A reference to the altered stream (in this case the proxy)
   */
  template <typename T>
  inline tStream &operator << (const T *pointer)
  {
    if (pointer == 0)
    {
      this->stream << "<nullptr>";
      return *this;
    }
    this->stream << pointer;
    return *this;
  }

  template <typename T>
  inline tStream &operator << (T *pointer)
  {
    *this << const_cast<const T *>(pointer);
    return *this;
  }

  template <typename T, typename ... Args>
  inline tStream &operator << (T(*function)(Args...))
  {
    *this << reinterpret_cast<void *>(function);
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
  inline tStream &operator << (std::ios_base & (*manipulator)(std::ios_base &))
  {
    manipulator(this->stream);
    return *this;
  }

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  std::ostream stream;
  std::unique_lock<std::mutex> lock;

  // Prohibit copy
  tStream(const tStream &other);

  // Prohibit assignment
  tStream &operator = (const tStream &other);

  // Prohibit creation on heap
  void *operator new(size_t size);

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
