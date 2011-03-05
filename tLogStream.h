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
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef __rrlib__logging__tLogStream_h__
#define __rrlib__logging__tLogStream_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <exception>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <memory>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/tLogStreamContext.h"

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
typedef std::shared_ptr<const tLogDomain> tLogDomainSharedPointer;

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

//----------------------------------------------------------------------
// Public methods and typedefs
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
  explicit tLogStream(std::shared_ptr<tLogStreamContext> stream_context)
      : stream_context(stream_context)
  {}

  /*! The copy ctor of tLogStream
   *
   * Copies a tLogStream object and its lock.
   * This methods blocks until the lock can be re-acquired.
   *
   * \param other   The other tLogStream object
   */
  tLogStream(const tLogStream &other)
      : stream_context(other.stream_context)
  {}

  /*! Implicit conversion to std::ostream for e.g. std::ostream_iterator
   *
   * To use this proxy class together with e.g. std::copy, which needs
   * a std::ostream_iterator, this method is implemented.
   *
   * \returns A reference to the underlying std::ostream
   */
  operator std::ostream &()
  {
    return this->stream_context->GetStream();
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
    this->stream_context->GetStream() << value;
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
    this->stream_context->GetStream() << "Exception (" << typeid(exception).name() << "): " << exception.what();
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
  inline tLogStream &operator << (bool value)
  {
    this->stream_context->GetStream() << (value ? "<true>" : "<false>");
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
  inline tLogStream &operator << (const T *pointer)
  {
    if (pointer == 0)
    {
      this->stream_context->GetStream() << "<nullptr>";
      return *this;
    }
    this->stream_context->GetStream() << pointer;
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
    manipulator(this->stream_context->GetStream());
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

  // FIXME: with -std=gnu++0x the following set of methods can be replaced by a proper variadic template implementation like this:
//  template <typename THead, typename ... TRest>
//  inline tLogStream &Evaluate(const THead &head, TRest... rest)
//  {
//    *this << head;
//    this->Evaluate(rest...);
//    return *this;
//  }
//
//  template <typename THead, typename ... TRest>
//  inline tLogStream &Evaluate(tLogDomainSharedPointer (&)(), const THead &head, TRest... rest)
//  {
//    return this->Evaluate(head, rest...);
//  }
//
//  template <typename THead>
//  inline tLogStream &Evaluate(const THead &head)
//  {
//    return *this << head;
//  }

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


//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  std::shared_ptr<tLogStreamContext> stream_context;

  // Prohibit assignment
  tLogStream &operator = (const tLogStream &other);

  // Prohibit creation on heap
  void *operator new(size_t size);

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
