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
/*!\file    tLogDomain.h
 *
 * \author  Tobias Foehst
 * \author  Max Reichardt
 *
 * \date    2010-06-16
 *
 * \brief Contains tLogDomain
 *
 * \b tLogDomain
 *
 * The RRLib logging system is structured into hierarchical domains that
 * can be created and configured via tLogDomainRegistry. That given,
 * in the program implementation instances of the class tLogDomain
 * wrap the stream that can be access either in C++ iostream style via
 * operator << or in good old-fashioned C style using printf formatting.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef __rrlib__logging__tLogDomain_h__
#define __rrlib__logging__tLogDomain_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdarg>
#include <cassert>
#include <memory>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/tLogDomainConfiguration.h"
#include "rrlib/logging/tLogStreamBuffer.h"
#include "rrlib/logging/tLogStream.h"
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
//! This class implements messaging via a specific logging domain
/*! The RRLib logging system is structured into hierarchical domains that
 *  can be created and configured via tLogDomainRegistry. That given,
 *  in the program implementation instances of this class wrap the stream
 *  that can be access either in C++ iostream style via operator << or
 *  in the good old-fashioned C style using printf formatting.
 *
 */
class tLogDomain
{
  friend class tLogDomainRegistry;

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*! The dtor of tLogDomain
   */
  ~tLogDomain();

  /*! Get the full qualified name of this domain
   *
   * Each domain has a full qualified name consisting of its parent's name
   * and the local part that was given at creation time.
   *
   * \returns The full qualified domain name
   */
  inline const std::string GetName() const
  {
    return this->configuration->name;
  }

  /*! Get configuration status of this domain's print_time flag
   *
   * The current time is prepended to messages of this domain if the
   * print_time flag is set.
   *
   * \returns Whether the print_time flag is set or not.
   */
  inline const bool GetPrintTime() const
  {
    return this->configuration->print_time;
  }

  /*! Get configuration status of this domain's print_name flag
   *
   * The name of this domain prepended to messages of this domain if its
   * print_name flag is set.
   *
   * \returns Whether the print_name flag is set or not.
   */
  inline const bool GetPrintName() const
  {
    return this->configuration->print_name;
  }

  /*! Get configuration status of this domain's print_level flag
   *
   * The level of each message is contained in the output of this domain
   * if the print_level flag is set.
   *
   * \returns Whether the print_level flag is set or not.
   */
  inline const bool GetPrintLevel() const
  {
    return this->configuration->print_level;
  }

  /*! Get configuration status of this domain's print_location flag
   *
   * The location given to each message is contained in the output of this
   * domain if the print_location flag is set.
   *
   * \returns Whether the print_location flag is set or not.
   */
  inline const bool GetPrintLocation() const
  {
    return this->configuration->print_location;
  }

  /*! Get the maximal log level a message must have to be processed
   *
   * Each message has a log level that must not be above the configured limit to be processed.
   *
   * \returns The configured maximal log level
   */
  inline const tLogLevel GetMaxMessageLevel() const
  {
    return this->configuration->max_message_level;
  }

  /*! Get a message stream from this domain
   *
   * This method is the streaming interface to this logging domain.
   * It must be used for every output using operator <<.
   * The method then depending on the domain's configuration chooses
   * a stream, prints the prefix that should be prepended to every
   * message and returns the stream to process further input given as
   * operator << cascade in the user's program.
   * To properly specify the arguments of this method consider using
   * the macros defined in rrlib/logging/definitions.h
   *
   * \param description   A string that describes the global context of the message
   * \param function      The name of the function that contains the message (__FUNCTION__)
   * \param file          The file that contains the message
   * \param line          The line that contains the message
   * \param level         The log level of the message
   *
   * \returns A reference to the stream that can be used for the remaining message parts
   */
  template <typename TDescription>
  inline tLogStream GetMessageStream(const TDescription &description, const char *function, const char *file, unsigned int line, tLogLevel level) const
  {
    tLogStream stream_proxy(std::shared_ptr<tLogStreamContext>(new tLogStreamContext(this->stream_buffer, mutex.get())));
    this->stream_buffer.Clear();
    this->stream_buffer.InitializeMultiLinePadding();
    if (level > this->GetMaxMessageLevel())
    {
      return stream_proxy;
    }
    this->SetupOutputStream(this->configuration->sink_mask);
    if (level == eLL_USER)
    {
      return stream_proxy;
    }
    if (this->GetPrintTime())
    {
      stream_proxy << this->GetTimeString();
    }
    this->SetupOutputStreamColor(level);
#ifndef _RRLIB_LOGGING_LESS_OUTPUT_
    if (this->GetPrintName())
    {
      stream_proxy << this->GetNameString();
    }
    if (this->GetPrintLevel())
    {
      stream_proxy << this->GetLevelString(level);
    }
#endif
    stream_proxy << description << "::" << function << " ";
#ifndef _RRLIB_LOGGING_LESS_OUTPUT_
    if (this->GetPrintLocation())
    {
      stream_proxy << this->GetLocationString(file, line);
    }
#endif
    stream_proxy << ">> ";
    this->stream_buffer.ResetColor();

    switch (level)
    {
    case eLL_ERROR:
      stream_proxy << "ERROR: ";
      break;
    case eLL_WARNING:
    case eLL_DEBUG_WARNING:
      stream_proxy << "WARNING: ";
      break;
    default:
      ;
    }

    this->stream_buffer.MarkEndOfPrefixForMultiLinePadding();

    return stream_proxy;
  }

  /*! A printf like variant of using logging domains for message output
   *
   * Instead of using operator << to output messages this method can be
   * used. It then itself uses printf to format the given message and
   * streams the result through the result obtained from GetMessageStream.
   * That way the message prefix is only generated in one place and - more
   * important - the underlying technique is the more sane one from
   * iostreams instead of file descriptors.
   * Apart from that: iostreams and file descriptors can not be mixed. So
   * a decision had to be made.
   *
   * \param description   A string that describes the global context of the message
   * \param function      The name of the function that contains the message (__FUNCTION__)
   * \param file          The file that contains the message
   * \param line          The line that contains the message
   * \param level         The log level of the message
   * \param fmt           The format string for printf
   * \param ...           The remaining arguments for printf
   */
  template <typename TDescription>
  inline void PrintMessage(const TDescription &description, const char *function, const char *file, int line, tLogLevel level, const char *fmt, ...) const
  {
    if (level > this->GetMaxMessageLevel())
    {
      return;
    }

    va_list printf_args0;
    va_start(printf_args0, fmt);
    char temp;
    int needed_buffer_size = vsnprintf(&temp, 1, fmt, printf_args0);
    va_end(printf_args0);

    va_list printf_args;
    va_start(printf_args, fmt);
    this->VPrintMessage(description, function, file, line, level, fmt, needed_buffer_size, printf_args);
    va_end(printf_args);
  }

  template <typename TDescription>
  inline void PrintMessage(const TDescription &description, const char *function, const char *file, int line, tLogLevel level, tLogDomainSharedPointer(&)(), const char *fmt, ...) const
  {
    if (level > this->GetMaxMessageLevel())
    {
      return;
    }

    va_list printf_args0;
    va_start(printf_args0, fmt);
    char temp;
    int needed_buffer_size = vsnprintf(&temp, 1, fmt, printf_args0);
    va_end(printf_args0);

    va_list printf_args;
    va_start(printf_args, fmt);
    this->VPrintMessage(description, function, file, line, level, fmt, needed_buffer_size, printf_args);
    va_end(printf_args);
  }

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  tLogDomain *parent;
  std::vector<tLogDomain *> children;

  tLogDomainConfigurationSharedPointer configuration;

  mutable tLogStreamBuffer stream_buffer;
  mutable std::ofstream file_stream;

  std::shared_ptr<boost::recursive_mutex> mutex;

  /*! The ctor of a top level domain
   *
   * This ctor is to be called by the registry that creates the top level
   * domain.
   *
   * \param configuration   The configuration for the new domain
   */
  tLogDomain(tLogDomainConfigurationSharedPointer configuration);

  /*! The ctor for a new sub domain
   *
   * This ctor is to be called by the registry to create a new subdomain
   * with a given configuration
   *
   * \param configuration   The configuration for the new domain
   * \param parent          The parent domain
   */
  tLogDomain(tLogDomainConfigurationSharedPointer configuration, tLogDomain &parent);

  /*!
   * \returns Shared Pointer to output mutex that is shared by all logging domains
   */
  static std::shared_ptr<boost::recursive_mutex> GetMutex()
  {
    static std::shared_ptr<boost::recursive_mutex> mutex(new boost::recursive_mutex());
    return mutex;
  }

  /*! Recursively configure the subtree that begins in this domain
   *
   * If the domain is configured by its parent, the configuration is
   * copied and propagated to this domain's children
   */
  void ConfigureSubTree();

  /*! Open the file stream for file output
   *
   * This method creates a new file which name is build using a prefix
   * and the full qualified domain name.
   * If the file already exists, it will be truncated.
   *
   * \returns Whether the file stream could be opened or not
   */
  const bool OpenFileOutputStream() const;

  /*! Setup the output stream to be used in this domain
   *
   * A domain can stream its input to stdout, stderr, an own file and/or its parent's file.
   *
   *\param mask   The bitmask that selects the output streams to use
   */
  void SetupOutputStream(int mask) const;

  /*! Get the current time as string for internal use in messages
   *
   * This method formats the current time as string that can be used in
   * messages.
   *
   * \returns The current time as string
   */
  const std::string GetTimeString() const;

  /*! Get the domain's name as string for internal use in messages
   *
   * This method formats the name as string that can be used in
   * messages. This string is padded with spaces to the length of the
   * longest domain name
   *
   * \returns The padded name as string
   */
  const std::string GetNameString() const;

  /*! Get the given message level as string for internal use in messages
   *
   * This method formats the given level as string that can be used in
   * messages.
   *
   * \param level   The level that should be represented as string
   *
   * \returns The given level as padded string
   */
  const std::string GetLevelString(tLogLevel level) const;

  /*! Get the given location as string for internal use in messages
   *
   * This method formats given location consisting of a file name and a
   * line number as string that can be used in messages.
   *
   * \param file   The file name (e.g. from __FILE__)
   * \param line   The line number (e.g. from __LINE__)
   *
   * \returns The given location as string
   */
  const std::string GetLocationString(const char *file, unsigned int line) const;

  /*! Setup the underlying streambuffer to produce colored output
   *
   * This method sets up the underlying streambuffer for colored
   * output according to the given level.
   *
   * \param level   The according log level
   */
  void SetupOutputStreamColor(tLogLevel level) const;

  template <typename TDescription>
  inline void VPrintMessage(const TDescription &description, const char *function, const char *file, int line, tLogLevel level, const char *fmt, int needed_buffer_size, va_list printf_args) const
  {
    if (needed_buffer_size > 0)
    {
      char formatted_string_buffer[needed_buffer_size + 1];
      vsnprintf(formatted_string_buffer, sizeof(formatted_string_buffer), fmt, printf_args);
      this->GetMessageStream(description, function, file, line, level) << formatted_string_buffer;
    }
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
