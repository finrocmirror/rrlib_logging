//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//----------------------------------------------------------------------
/*!\file    rrlib/logging/sinks/tFile.cpp
 *
 * \author  Tobias Föhst
 *
 * \date    2013-08-07
 *
 */
//----------------------------------------------------------------------
#define __rrlib__logging__include_guard__
#include "rrlib/logging/sinks/tFile.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <string>
#include <stdexcept>

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/configuration/tDomainRegistry.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace logging
{
namespace sinks
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tFile constructors
//----------------------------------------------------------------------
tFile::tFile(const tConfiguration &configuration) :
  configuration(configuration)
{}

tFile::tFile(const xml::tNode &node, const tConfiguration &configuration) :
  configuration(configuration)
{}

//----------------------------------------------------------------------
// tFile destructors
//----------------------------------------------------------------------
tFile::~tFile()
{
  if (this->file_stream.is_open())
  {
    this->file_stream.close();
  }
}

//----------------------------------------------------------------------
// tFile GetStreamBuffer
//----------------------------------------------------------------------
std::streambuf &tFile::GetStreamBuffer()
{
  if (!this->file_stream.is_open())
  {
    const std::string &file_name_prefix(tDomainRegistry::Instance().LogFilenamePrefix());
    if (file_name_prefix.length() == 0)
    {
      std::stringstream message;
      message << "RRLib Logging >> Prefix for log filenames not set. Can not use file sink." << std::endl
              << "                 Consider calling e.g. rrlib::logging::SetLogFilenamePrefix(basename(argv[0])) from main." << std::endl;
      throw std::runtime_error(message.str());
    }

    std::string fqdn = this->configuration.GetFullQualifiedName();
    std::string file_name(file_name_prefix + (fqdn != "." ? fqdn : "") + ".log");
    this->file_stream.open(file_name.c_str(), std::ios::out | std::ios::trunc);
    if (!this->file_stream.is_open())
    {
      std::stringstream message;
      message << "RRLib Logging >> Could not open file `" << file_name << "'!" << std::endl;
      throw std::runtime_error(message.str());
    }
  }

  return *this->file_stream.rdbuf();
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
}
