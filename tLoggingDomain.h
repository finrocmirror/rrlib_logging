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
/*!\file    tLoggingDomain.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief
 *
 * \b
 *
 * A few words for tLoggingDomain.h
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_logging_include_guard_
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef rrlib_logging_tLoggingDomain_h_
#define rrlib_logging_tLoggingDomain_h_

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
#include "tLoggingDomainConfiguration.h"

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
//! Short description of tLoggingDomain
/*! A more detailed description of tLoggingDomain, which
 *  Tobias Foehst hasn't done yet!
 *
 */
class tLoggingDomain
{
  friend class tLoggingDomainRegistry;

  tLoggingDomain *parent;
  std::vector<tLoggingDomain *> children;

  tLoggingDomainConfigurationSharedPointer configuration;

//  std::ostream *stream;
  mutable std::ofstream null_stream;
  mutable std::ofstream file_stream;

  /*! The ctor of a top level domain
   *
   * This ctor is to be called by the registry that creates the top level
   * domain.
   *
   * \param configuration   The configuration for the new domain
   */
  tLoggingDomain(tLoggingDomainConfigurationSharedPointer configuration);

  /*! The ctor for a new sub domain
   *
   * This ctor is to be called by the registry to create a new subdomain
   * with a given configuration
   *
   * \param configuration   The configuration for the new domain
   * \param parent          The parent domain
   */
  tLoggingDomain(tLoggingDomainConfigurationSharedPointer configuration, tLoggingDomain &parent);

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

  /*! Get the output stream to be used in this domain
   *
   * A domain can stream its input either to stdout, stderr or to a file.
   *
   * \returns A reference to the stream that should be used for messages
   */
  std::ostream &GetOutputStream() const;

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
  const std::string GetLevelString(eLogLevel level) const;

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
   * \returns The string containg the control sequence
   */
  const std::string GetColoredOutputString(eLogLevel level) const;

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! The dtor of tLoggingDomain
   */
  ~tLoggingDomain();

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

  /*! Get configuration status of this domain's enabled flag
   *
   * If a domain is enabled it processes log messages that are not below a
   * specified min level. Otherwise it is totally quite.
   *
   * \returns Whether the domain is enabled or not
   */
  inline const bool IsEnabled() const
  {
    return this->configuration->enabled;
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

  /*! Get the minimal log level a message must have to be processed
   *
   * Each message has a log level that must not below the configured limit to be processed.
   *
   * \returns The configured minimal log level
   */
  inline const eLogLevel GetMinMessageLevel() const
  {
    return this->configuration->min_message_level;
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
  inline std::ostream &GetMessageStream(const char *description, const char *function, const char *file, unsigned int line, eLogLevel level) const
  {
    if (level < this->GetMinMessageLevel() || !this->IsEnabled())
    {
      return this->null_stream;
    }
    std::ostream &stream(this->GetOutputStream());

    if (this->GetPrintTime())
    {
      stream << this->GetTimeString();
    }
    if (stream != this->file_stream)
    {
      stream << this->GetColoredOutputString(level);
    }
#ifndef _RRLIB_LOGGING_LESS_OUTPUT_
    if (this->GetPrintName())
    {
      stream << this->GetNameString();
    }
    if (this->GetPrintLevel())
    {
      stream << this->GetLevelString(level);
    }
#endif
    stream << description << "::" << function << " ";
#ifndef _RRLIB_LOGGING_LESS_OUTPUT_
    if (this->GetPrintLocation())
    {
      stream << this->GetLocationString(file, line);
    }
#endif
    stream << ">> ";
    if (stream != this->file_stream)
    {
      stream << "\033[;0m";
    }
    return stream;
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
  inline void PrintMessage(const char *description, const char *function, const char *file, int line, eLogLevel level, const char *fmt, ...) const
  {
    if (level < this->GetMinMessageLevel() || !this->IsEnabled())
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