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
/*!\file    tDomainRegistry.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-16
 *
 * \brief Contains tDomainRegistry
 *
 * \b tDomainRegistry
 *
 * tDomainRegistry is a central management facility for logging domains
 * and their configuration.
 * In RRLib logging messages can be send via several logging domains.
 * These have to be created and maintained using a single instance of
 * tDomainRegistry. Thus, this class implements the singleton pattern
 * and contains a list of logging domains and configurations that
 * either were created along with active domains or were configured by
 * the user from a file or calling the appropriate methods.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/configuration.h" instead.
#endif

#ifndef RRLIB_LOG_PATH
#warning RRLIB_LOG_PATH not set. Using default path "/usr/include:/usr/include/finroc" which most likely is not sufficient for your setup.
#define RRLIB_LOG_PATH /usr/include:/usr/include/finroc
#endif

#ifndef __rrlib__logging__configuration__tDomainRegistry_h__
#define __rrlib__logging__configuration__tDomainRegistry_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
//#include <string>
#include <vector>

#include "rrlib/util/patterns/singleton.h"

#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
#include "rrlib/xml2_wrapper/tXMLNode.h"
#endif

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/configuration/tConfiguration.h"

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
//! The central management facility for logging domains and theirconfiguration
/*! In RRLib logging messages can be send via several logging domains.
 *  These have to be created and maintained using a single instance of
 *  this class. Thus, this class registers a list of active logging domain
 *  configurations that either were created by sending messages or were
 *  configured by the user from a file or calling the appropriate methods.
 *  It must be used via the singleton holder typedef below.
 */
class tDomainRegistryImplementation
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*! Ctor of tDomainRegistryImplementation
   *
   */
  tDomainRegistryImplementation();

  /*! Dtor of tDomainRegistryImplementation
   *
   */
  ~tDomainRegistryImplementation();

  const tConfiguration &GetConfiguration(const char *filename, const char *domain_name = 0) const;

  const tConfiguration &GetConfigurationByFilename(const char *filename) const;

//  /*! Set a prefix for filenames that are created as log
//   *
//   * If their output stream is set to eMS_FILE domains create a log
//   * file with their name. Additionally, this name must have a prefix
//   * to distinguish between programs, processes or runs.
//   *
//   * The method could be called with basename(argv[0]) in main, for example.
//   *
//   * \param file_name_prefix   The string that should be used as prefix
//   */
//  inline void SetOutputFileNamePrefix(const std::string &file_name_prefix)
//  {
//    assert(file_name_prefix.length() > 0);
//    this->file_name_prefix = file_name_prefix;
//  }
//
//  /*! Get the configured file name prefix
//   *
//   * Get the file name prefix that was configured
//   *
//   * \returns The stored prefix
//   */
//  inline const std::string &GetOutputFileNamePrefix() const
//  {
//    return this->file_name_prefix;
//  }

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

  void UpdateMaxDomainNameLength(size_t added_domain_name_length);

  /*! Get the length of the longest full qualified domain name
   *
   * This method can be used for formatting user dialogs
   *
   * \returns The length of the longest domain name
   */
  inline size_t MaxDomainNameLength() const
  {
    return this->max_domain_name_length;
  }

//#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
//  /*! Read domain configuration from a given XML node
//   *
//   * Instead of reading and parsing an XML file dedicated to configure
//   * logging domains this method can be used after externally parsing
//   * a document that contains an rrlib_logging node following the DTD
//   * -//RRLIB//logging
//   *
//   * \param node   The XML node containing the configuration
//   *
//   * \returns Whether the configuration could be applied or not
//   */
//  bool ConfigureFromXMLNode(const xml2::tXMLNode &node);
//#endif

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  std::vector<const char *> prefix_pointers;
  std::vector<size_t> prefix_lengths;
  std::vector<size_t> prefix_indices_sorted_by_length;

  mutable tConfiguration *global_configuration;

//  std::string file_name_prefix;
  size_t max_domain_name_length;
  bool pad_prefix_columns;
  bool pad_multi_line_messages;

  const tConfiguration &GetConfigurationByFilename(const char *filename) const;

//#ifdef _LIB_RRLIB_XML2_WRAPPER_PRESENT_
//  /*! Add a domain configuration from a given XML node
//   *
//   * This method configures a logging domain using the values specified in
//   * the given XML node. It also implements recursive configuration in case
//   * of nested nodes.
//   *
//   * \param node          The XML node that contains the configuration
//   * \param parent_name   For recursive calls the current domain name is build from parent_name and domain_name
//   *
//   * \returns Whether the domain was successfully configured or not
//   */
//  bool AddConfigurationFromXMLNode(const xml2::tXMLNode &node, const std::string &parent_name = "");
//#endif

};

typedef util::tSingletonHolder<tDomainRegistryImplementation, util::singleton::NoDestruction> tDomainRegistry;

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
