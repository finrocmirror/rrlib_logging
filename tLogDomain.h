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
#ifndef _rrlib_logging_include_guard_
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef rrlib_logging_tLogDomain_h_
#define rrlib_logging_tLogDomain_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <cassert>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "logging/tLogDomainConfiguration.h"
#include "logging/tLogStreamBuffer.h"
#include "logging/tLogStream.h"

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

  tLogDomain *parent;
  std::vector<tLogDomain *> children;

  tLogDomainConfigurationSharedPointer configuration;

  mutable tLogStreamBuffer stream_buffer;
  mutable std::ostream stream;
  mutable std::ofstream file_stream;

  std::tr1::shared_ptr<boost::recursive_mutex> mutex;

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
  static std::tr1::shared_ptr<boost::recursive_mutex> GetMutex()
  {
    static std::tr1::shared_ptr<boost::recursive_mutex> mutex(new boost::recursive_mutex());
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

  /*! Get a string to setup colored output in a terminal
   *
   * This method creates a string that contains the control sequence to
   * setup colored output according to the given level.
   *
   * \param level   The according log level
   *
   * \returns The string containing the control sequence
   */
  const std::string GetControlStringForColoredOutput(tLogLevel level) const;

//----------------------------------------------------------------------
// Public methods
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

//  /*! Get the mask representing which streams are used for message output
//   *
//   * For message output several streams can be used. This bitmask configures
//   * which of them are enabled.
//   *
//   * \returns The bitmask that contains the enabled message streams
//   */
//  inline const eLogStreamMask GetStreamMask() const
//  {
//    return this->configuration->stream_mask;
//  }

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
    tLogStream stream_proxy(this->stream, mutex.get());
    this->stream_buffer.Clear();
    if (level > this->GetMaxMessageLevel())
    {
      return stream_proxy;
    }
    this->SetupOutputStream(this->configuration->sink_mask);

    if (this->GetPrintTime())
    {
      this->stream << this->GetTimeString();
    }
    this->SetupOutputStream(this->configuration->sink_mask & ~((1 << eLS_FILE) | (1 << eLS_COMBINED_FILE)));
    this->stream << this->GetControlStringForColoredOutput(level);
    this->SetupOutputStream(this->configuration->sink_mask);

#ifndef _RRLIB_LOGGING_LESS_OUTPUT_
    if (this->GetPrintName())
    {
      this->stream << this->GetNameString();
    }
    if (this->GetPrintLevel())
    {
      this->stream << this->GetLevelString(level);
    }
#endif
    this->stream << description << "::" << function << " ";
#ifndef _RRLIB_LOGGING_LESS_OUTPUT_
    if (this->GetPrintLocation())
    {
      this->stream << this->GetLocationString(file, line);
    }
#endif
    this->stream << ">> ";
    this->SetupOutputStream(this->configuration->sink_mask & ~((1 << eLS_FILE) | (1 << eLS_COMBINED_FILE)));
    this->stream << "\033[;0m";
    this->SetupOutputStream(this->configuration->sink_mask);

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
    char formatted_string_buffer[1024];

    va_list printf_args;
    va_start(printf_args, fmt);
    vsnprintf(formatted_string_buffer, sizeof(formatted_string_buffer), fmt, printf_args);
    va_end(printf_args);

    this->GetMessageStream(description, function, file, line, level) << formatted_string_buffer;
  }

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
