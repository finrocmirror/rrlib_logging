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
 * \author  Max Reichardt
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
#error Invalid include directive. Try #include "logging/definitions.h" instead.
#endif

#ifndef rrlib_logging_tLogStreamProxy_h_
#define rrlib_logging_tLogStreamProxy_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
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
//! Shared pointer to instances of tLogDomain for user space
class tLogDomain;
typedef std::tr1::shared_ptr<const tLogDomain> tLogDomainSharedPointer;

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

  boost::recursive_mutex *mutex;

  // Prohibit assignment
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
   * \param mutex    Mutex to acquire while this proxy is used
   */
  explicit tLogStream(std::ostream &stream, boost::recursive_mutex *mutex)
      : stream(stream),
      mutex(mutex)
  {
    mutex->lock();
  }

  /*! The copy ctor of tLogStream
   *
   * Copies a tLogStream object and its lock.
   * This methods blocks until the lock can be re-acquired.
   *
   * \param other   The other tLogStream object
   */
  tLogStream(const tLogStream &other)
      : stream(other.stream),
      mutex(other.mutex)
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

  inline tLogStream &Evaluate()
  {
    return *this;
  }

  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)())
  {
    return *this;
  }

  template <typename T1>
  inline tLogStream &Evaluate(const T1 &arg1)
  {
    return *this << arg1;
  }

  template <typename T1>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1)
  {
    return *this << arg1;
  }

  template <typename T1, typename T2>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2)
  {
    return *this << arg1 << arg2;
  }

  template <typename T1, typename T2>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2)
  {
    return *this << arg1 << arg2;
  }

  template <typename T1, typename T2, typename T3>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3)
  {
    return *this << arg1 << arg2 << arg3;
  }

  template <typename T1, typename T2, typename T3>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3)
  {
    return *this << arg1 << arg2 << arg3;
  }

  template <typename T1, typename T2, typename T3, typename T4>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4)
  {
    return *this << arg1 << arg2 << arg3 << arg4;
  }

  template <typename T1, typename T2, typename T3, typename T4>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4)
  {
    return *this << arg1 << arg2 << arg3 << arg4;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13, const T14 &arg14)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13 << arg14;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13, const T14 &arg14)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13 << arg14;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13, const T14 &arg14, const T15 &arg15)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13 << arg14 << arg15;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13, const T14 &arg14, const T15 &arg15)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13 << arg14 << arg15;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
  inline tLogStream &Evaluate(const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13, const T14 &arg14, const T15 &arg15, const T16 &arg16)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13 << arg14 << arg15 << arg16;
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
  inline tLogStream &Evaluate(tLogDomainSharedPointer(&)(), const T1 &arg1, const T2 &arg2, const T3 &arg3, const T4 &arg4, const T5 &arg5, const T6 &arg6, const T7 &arg7, const T8 &arg8, const T9 &arg9, const T10 &arg10, const T11 &arg11, const T12 &arg12, const T13 &arg13, const T14 &arg14, const T15 &arg15, const T16 &arg16)
  {
    return *this << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8 << arg9 << arg10 << arg11 << arg12 << arg13 << arg14 << arg15 << arg16;
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
