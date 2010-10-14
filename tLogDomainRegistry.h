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
/*!\file    tLogDomainRegistry.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief Contains tLogDomainRegistry
 *
 * \b tLogDomainRegistry
 *
 * tLogDomainRegistry is a central management facility for logging
 * domains and their configuration.
 * In RRLib logging messages can be send via several logging domains.
 * These have to be created and maintained using a single instance of
 * tLogDomainRegistry. Thus, this class implements the singleton
 * pattern and contains a list of logging domains and configurations
 * that either were created along with active domains or were configured
 * by the user from a file or calling the appropriate methods.
 *
 */
//----------------------------------------------------------------------
#ifndef _rrlib_logging_include_guard_
#error Invalid include directive. Try #include "rrlib/logging/definitions.h" instead.
#endif

#ifndef _rrlib_logging_tLogDomainRegistry_h_
#define _rrlib_logging_tLogDomainRegistry_h_

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <vector>
#include <string>
#include <tr1/memory>

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
#include "rrlib/xml2_wrapper/tXMLNode.h"
#endif
//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/tLogDomain.h"
#include "rrlib/logging/tLogDomainConfiguration.h"

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
//! The central management facility for logging domains and their configuration
/*! In RRLib logging messages can be send via several logging domains.
 *  These have to be created and maintained using a single instance of
 *  this class. Thus, this class implements the singleton pattern and
 *  contains a list of active logging domains and configurations that
 *  either were created along with active domains or were configured by
 *  the user from a file or calling the appropriate methods.
 *
 */
class tLogDomainRegistry
{
  std::string file_name_prefix;
  std::vector<std::tr1::shared_ptr<tLogDomain> > domains;
  std::vector<tLogDomainConfigurationSharedPointer> domain_configurations;
  std::string::size_type max_domain_name_length;
  bool pad_prefix_columns;
  bool pad_multi_line_messages;

  /*! Ctor of tLogDomainRegistry
   *
   * Private default ctor for singleton pattern
   */
  tLogDomainRegistry();

  /*! Get the index of the domain with the given name
   *
   * Helper method that implements the lookup for existing domains
   *
   * \param name   The name of the wanted domain
   *
   * \returns The index of the domain if found. If not, the methods returns the size of the domain vector.
   */
  const size_t GetDomainIndexByName(const std::string &name) const;

  /*! Get a configuration object for a domain with the given name
   *
   * This methods implements the lookup for existing domain names and
   * creates a new configuration object for new names.
   *
   * \param name   The name of the domain to be configured
   *
   * \returns The wanted domain configuration as a shared pointer
   */
  tLogDomainConfigurationSharedPointer GetConfigurationByName(const std::string &name);

  /*! Update configuration the subtree of a domain for recursion
   *
   * If the configuration of one domain is changed start update of its
   * subtree. This method should always be called because the decision
   * about recursive configuration is done within its call.
   * That keeps update methods simpler.
   *
   * \param name   The name of the updated domain
   */
  void PropagateDomainConfigurationToChildren(const std::string &name);

  /*! Set the output streams that should be used by the given domain
   *
   * \param name   The full qualified name of the domain
   * \param mask   The mask that represents the streams to be used as sink
   */
  void SetDomainSinkMask(const std::string &name, int mask);

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
  /*! Add a domain configuration from a given XML node
   *
   * This method configures a logging domain using the values specified in
   * the given XML node. It also implements recursive configuration in case
   * of nested nodes.
   *
   * \param node          The XML node that contains the configuration
   * \param parent_name   For recursive calls the current domain name is build from parent_name and domain_name
   *
   * \returns Whether the domain was successfully configured or not
   */
  bool AddConfigurationFromXMLNode(const xml2::tXMLNode &node, const std::string &parent_name = "");
#endif

//----------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------
public:

  /*! Get an instance of this class (singleton)
   *
   * Due to the singleton pattern this class has no public constructor.
   * The only way to get an object is to call this method.
   *
   * \returns The only instance of this class that should exist
   */
  static std::tr1::shared_ptr<tLogDomainRegistry> GetInstance();

  /*! Get the default domain
   *
   * There is always on default domain registered that can be accessed
   * using this method as a shortcut.
   *
   * \returns The default domain object
   */
  static inline tLogDomainSharedPointer GetDefaultDomain()
  {
    return GetInstance()->domains[0];
  }

  /*! Get a subdomain with given name and parent.
   *
   * This method can be used to access a subdomain of the given parent
   * with given name. It then implements the lookup and if the domain
   * was not found creates a new one and applies an eventually existing
   * configuration.
   *
   * \param name     The name of the subdomain (local part)
   * \param parent   The parent of the subdomain
   *
   * \returns The found or newly created domain object as a shared pointer
   */
  tLogDomainSharedPointer GetSubDomain(const std::string &name, tLogDomainSharedPointer parent);

  /*! Set a prefix for filenames that are created as log
   *
   * If their output stream is set to eMS_FILE domains create a log
   * file with their name. Additionally, this name must have a prefix
   * to distinguish between programs, processes or runs.
   *
   * The method could be called with basename(argv[0]) in main, for example.
   *
   * \param file_name_prefix   The string the should be used as prefix
   */
  inline void SetOutputFileNamePrefix(const std::string &file_name_prefix)
  {
    assert(file_name_prefix.length() > 0);
    this->file_name_prefix = file_name_prefix;
  }

  /*! Get the configured file name prefix
   *
   * Get the file name prefix that was configured
   *
   * \returns The stored prefix
   */
  inline const std::string &GetOutputFileNamePrefix() const
  {
    return this->file_name_prefix;
  }

  /*! Set if columns in prefix output should be padded or not
   *
   * If the prefix output columns should be aligned, the fields for the
   * domain name and message level are padded with spaces to the right.
   *
   * \param value   The new value of this flag
   */
  inline void SetPadPrefixColumns(bool value)
  {
    this->pad_prefix_columns = value;
  }

  /*! Get if columns in prefix output should be padded or not
   *
   * If the prefix output columns should be aligned, the fields for the
   * domain name and message level are padded with spaces to the right.
   *
   * \returns Whether the columns in prefix output should be padded or not
   */
  inline const bool GetPadPrefixColumns() const
  {
    return this->pad_prefix_columns;
  }

  /*! Set if rows in output should be padded to start after the prefix or not
   *
   * For multiline output following line do not have a prefix. Thus, they start
   * in the first column whearas the first row started right after the prefix.
   * This flag allows to choose if subsequent lines should be padded to match the
   * first row or not.
   *
   * \param value   The new value of this flag
   */
  inline void SetPadMultiLineMessages(bool value)
  {
    this->pad_multi_line_messages = value;
  }

  /*! Get if rows in output should be padded to start after the prefix or not
   *
   * For multiline output following line do not have a prefix. Thus, they start
   * in the first column whearas the first row started right after the prefix.
   * This flag allows to choose if subsequent lines should be padded to match the
   * first row or not.
   *
   * \returns Whether the rows should be padded or not
   */
  inline const bool GetPadMultiLineMessages() const
  {
    return this->pad_multi_line_messages;
  }

  /*! Get the length of the longest full qualified domain name
   *
   * This method can be used for formatting user dialogs
   *
   * \returns The length of the longest domain name
   */
  std::string::size_type GetMaxDomainNameLength() const
  {
    return this->max_domain_name_length;
  }

  /*! Set if the domain configures its subtree or not
   *
   * If set to true every configuration update to the given domain
   * will be propagated to its subtree.
   *
   * \param name    The full qualified name of the domain
   * \param value   The new value of the setting
   */
  void SetDomainConfiguresSubTree(const std::string &name, bool value);

  /*! Set if the domain should prepend the current time to each message
   *
   * If set to true every message will start with the current time
   *
   * \param name    The full qualified name of the domain
   * \param value   The new value of the setting
   */
  void SetDomainPrintsTime(const std::string &name, bool value);

  /*! Set if the domain should prepend messages with its name
   *
   * If set to true every message will include the full qualified domain
   * name
   *
   * \param name    The full qualified name of the domain
   * \param value   The new value of the setting
   */
  void SetDomainPrintsName(const std::string &name, bool value);

  /*! Set if the domain should prepend messages with their level
   *
   * If set to true every message will include its level
   *
   * \param name    The full qualified name of the domain
   * \param value   The new value of the setting
   */
  void SetDomainPrintsLevel(const std::string &name, bool value);

  /*! Set if the domain should prepend the message with its location
   *
   * If set to true every message will include its location in code.
   * This is extremely helpful during debugging phases.
   *
   * \param name    The full qualified name of the domain
   * \param value   The new value of the setting
   */
  void SetDomainPrintsLocation(const std::string &name, bool value);

  /*! Set the maximal message level of the given domain
   *
   * The output of each message that has a level above the given value
   * will be suppressed. Default is eML_DEBUG or eML_WARNING depending on
   * compile flags.
   *
   * \param name    The full qualified name of the domain
   * \param value   The new value of the setting
   */
  void SetDomainMaxMessageLevel(const std::string &name, tLogLevel value);

  /*! Set the output streams that should be used as sink by the given domain
   *
   * \param name     The full qualified name of the domain
   * \param sink_1   The first sink to be used by the given domain
   * \param sink_2   The second sink to be used by the given domain
   * \param sink_3   The third sink to be used by the given domain
   * \param sink_4   The fourth sink to be used by the given domain
   */
  void SetDomainSink(const std::string &name, tLogSink sink_1, tLogSink sink_2 = eLS_DIMENSION, tLogSink sink_3 = eLS_DIMENSION, tLogSink sink_4 = eLS_DIMENSION);

  /*! Read domain configuration from a given XML file
   *
   * The overall configuration of the logging domains tends to be
   * too complicated for a classical command line option interface.
   * Therefore, it is possible to specify the configuration in form
   * of an XML file following the DTD -//RRLIB//logging
   *
   * \param file_name   The XML file to be read
   *
   * \returns Whether the configuration could be read and applied or not
   */
  bool ConfigureFromFile(const std::string &file_name);

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
  /*! Read domain configuration from a given XML node
   *
   * Instead of reading and parsing an XML file dedicated to configure
   * logging domains this method can be used after externally parsing
   * a document that contains an rrlib_logging node following the DTD
   * -//RRLIB//logging
   *
   * \param node   The XML node containing the configuration
   *
   * \returns Whether the configuration could be applied or not
   */
  bool ConfigureFromXMLNode(const xml2::tXMLNode &node);
#endif

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
