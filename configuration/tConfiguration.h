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
/*!\file    rrlib/logging/configuration/tConfiguration.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-20
 *
 * \brief   Contains tConfiguration and corresponding enumerations
 *
 * \b tConfiguration
 *
 * tConfiguration encapsulates the configuration of logging
 * domains in the RRLib logging facility. It therefore stores settings
 * like enabled output fields, max. message level, etc.
 *
 *
 * \b tLogLevel
 *
 * The enumeration that encodes the message levels. These levels are
 * predefined and can be used to give the messages different priorities,
 * as messages with too low level will be suppressed by a configuration
 * setting or when RRLIB_LOGGING_LESS_OUTPUT is defined (e.g. in
 * release mode).
 * They are also used for colored output to stdout or stderr.
 *
 *
 * \b tLogSink
 *
 * The enumeration that encodes the sinks used by a logging domain.
 * Messages can be streamed to stdout, stderr, into one file per domain
 * or into one combined file for all domains that are recursively
 * configured in one subtree of the domain hierarchy.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/configuration.h" instead.
#endif

#ifndef __rrlib__logging__configuration__tConfiguration_h__
#define __rrlib__logging__configuration__tConfiguration_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <list>
#include <memory>
#include <mutex>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/log_levels.h"
#include "rrlib/logging/messages/tFanOutBuffer.h"
#include "rrlib/logging/sinks/tSink.h"

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


//! Enumeration type that contains the available sinks for message domains
enum tLogSink
{
  eLOG_SINK_STDOUT,          //!< Messages are printed to stdout
  eLOG_SINK_STDERR,          //!< Messages are printed to stderr
  eLOG_SINK_FILE,            //!< Messages are printed to single files for each subdomain
  eLOG_SINK_COMBINED_FILE,   //!< Messages are printed into one combined file
  eLOG_SINK_DIMENSION        //!< Endmarker and dimension of tLogSink
} __attribute__((deprecated));

struct tDefaultConfigurationContext
{
  const bool cPRINTS_NAME;
  const bool cPRINTS_TIME;
  const bool cPRINTS_LEVEL;
  const bool cPRINTS_LOCATION;
  const tLogLevel cMAX_LOG_LEVEL;
};
#ifdef RRLIB_LOGGING_LESS_OUTPUT
static const tDefaultConfigurationContext cDEFAULT_CONTEXT
{
  false, false, false, false, tLogLevel::WARNING
};
#else
static const tDefaultConfigurationContext cDEFAULT_CONTEXT
{
  false, false, false, true, tLogLevel::DEBUG
};
#endif

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! This class encapsulates the configuration of logging domains
/*! Each logging domain has its own configuration consisting of settings
 *  like enabled output fields, max. message level, etc.
 *  It is a implemented common structure that can be used by instances of
 *  tLogDomain and tLogDomainRegistry. Thus, it should not be
 *  instantiated or used by other classes and is declared totally private
 *  with the mentioned classes as friends.
 *
 */
class tConfiguration
{
  friend class tDomainRegistryImplementation;

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  ~tConfiguration();

  inline const std::string &Name() const
  {
    return this->name;
  }

  inline const std::string GetFullQualifiedName() const
  {
    return (this->parent && this->parent->parent ? this->parent->GetFullQualifiedName() : "") + "." + this->Name();
  }

  void SetPrintsName(bool value);
  void SetPrintsTime(bool value);
  void SetPrintsLevel(bool value);
  void SetPrintsLocation(bool value);
  void SetMaxMessageLevel(tLogLevel level);
  void SetSinkMask(int sink_mask) __attribute__((deprecated));
  void ClearSinks();
  void AddSink(std::shared_ptr<sinks::tSink> sink);

  inline bool PrintsName() const
  {
    return this->prints_name;
  }

  inline bool PrintsTime() const
  {
    return this->prints_time;
  }

  inline bool PrintsLevel() const
  {
    return this->prints_level;
  }

  inline bool PrintsLocation() const
  {
    return this->prints_location;
  }

  inline const tLogLevel MaxMessageLevel() const
  {
    return this->max_message_level;
  }

  inline tFanOutBuffer &StreamBuffer() const
  {
    if (!this->stream_buffer_ready)
    {
      this->PrepareStreamBuffer();
    }
    return this->stream_buffer;
  }

  inline const std::list<tConfiguration *> &Children() const
  {
    return this->children;
  }

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  const tConfiguration *parent;
  std::string name;

  bool prints_name;
  bool prints_time;
  bool prints_level;
  bool prints_location;

  tLogLevel max_message_level;

  std::vector<std::shared_ptr<sinks::tSink>> sinks;
  mutable bool stream_buffer_ready;
  mutable tFanOutBuffer stream_buffer;

  mutable std::list<tConfiguration *> children;
  mutable std::mutex children_mutex;

  tConfiguration(const tDefaultConfigurationContext &default_context, const tConfiguration *parent, const std::string &name);

  tConfiguration(const tConfiguration &other);

  tConfiguration &operator = (const tConfiguration other);

  const tConfiguration &GetConfigurationByName(const tDefaultConfigurationContext &default_context, const char *domain_name) const;

  const tConfiguration &GetConfigurationByFilename(const tDefaultConfigurationContext &default_context, const char *filename) const;

  const tConfiguration &GetChild(const tDefaultConfigurationContext &default_context, const char *name, size_t length) const;

  tConfiguration *FindChild(const char *name, size_t length) const;

  std::list<tConfiguration *>::iterator FindInsertionPoint(size_t length) const;

  void PrepareStreamBuffer() const;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
