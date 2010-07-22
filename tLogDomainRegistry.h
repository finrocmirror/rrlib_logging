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

#ifndef rrlib_logging_tLogDomainRegistry_h_
#define rrlib_logging_tLogDomainRegistry_h_

//----------------------------------------------------------------------
// External includes with <>
//----------------------------------------------------------------------
#include <vector>
#include <string>
#include <tr1/memory>

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
#include "xml2_wrapper/tXMLNode.h"
#endif
//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "logging/tLogDomain.h"
#include "logging/tLogDomainConfiguration.h"

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
typedef std::tr1::shared_ptr<const tLogDomain> tLogDomainSharedPointer;

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
   * \param mask   The mask that represents the streams to be used
   */
  void SetDomainStreamMask(const std::string &name, int mask);

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
  static tLogDomainRegistry &GetInstance();

  /*! Get the default domain
   *
   * There is always on default domain registered that can be accessed
   * using this method as a shortcut.
   *
   * \returns The default domain object
   */
  static inline tLogDomainSharedPointer GetDefaultDomain()
  {
    return GetInstance().domains[0];
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
   * \returns the stored prefix
   */
  inline const std::string &GetOutputFileNamePrefix() const
  {
    return this->file_name_prefix;
  }

  /*! Get the length of the longest full qualified domain name
   *
   * This method can be used for formatting user dialogs
   *
   * \returns The length of the longest domain name
   */
  size_t GetMaxDomainNameLength() const;

  /*! Enable a given domain and switch recursion on or off
   *
   * This is a shortcut for setting the configuration of the domain's
   * sub tree and enabling it.
   *
   * \param name            The full qualified name of the domain
   * \param with_sub_tree   Switch recursion on or off
   */
  void EnableDomain(const std::string &name, bool with_sub_tree = false)
  {
    this->SetDomainConfiguresSubTree(name, with_sub_tree);
    this->SetDomainIsEnabled(name, true);
  }

  /*! Disable a given domain and switch recursion on or off
   *
   * This is a shortcut for setting the configuration of the domain's
   * sub tree and disabling it.
   *
   * \param name            The full qualified name of the domain
   * \param with_sub_tree   Switch recursion on or off
   */
  void DisableDomain(const std::string &name, bool with_sub_tree = false)
  {
    this->SetDomainConfiguresSubTree(name, with_sub_tree);
    this->SetDomainIsEnabled(name, false);
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

  /*! Set if the domain is enabled or not
   *
   * If a domain is not enabled, none of its messages will be visible
   * regardless of its max message level.
   *
   * \param name    The full qualified name of the domain
   * \param value   The new value of the setting
   */
  void SetDomainIsEnabled(const std::string &name, bool value);

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
  void SetDomainMaxMessageLevel(const std::string &name, eLogLevel value);

  /*! Set the output streams that should be used by the given domain
   *
   * \param name       The full qualified name of the domain
   * \param stream_1   The first stream to be used by the given domain
   * \param stream_2   The second stream to be used by the given domain
   * \param stream_3   The third stream to be used by the given domain
   * \param stream_4   The fourth stream to be used by the given domain
   */
  void SetDomainStream(const std::string &name, eLogStream stream_1, eLogStream stream_2 = eLS_DIMENSION, eLogStream stream_3 = eLS_DIMENSION, eLogStream stream_4 = eLS_DIMENSION);

#ifdef _RRLIB_XML2_WRAPPER_PRESENT_
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
