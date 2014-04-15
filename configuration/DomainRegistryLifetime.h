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
/*!\file    rrlib/logging/configuration/DomainRegistryLifetime.h
 *
 * \author  Tobias Föhst
 *
 * \date    2014-04-15
 *
 * \brief   Contains DomainRegistryLifetime
 *
 * \b DomainRegistryLifetime
 *
 * This is the lifetime policy for tDomainRegistry.
 *
 * In principle tDomainRegistry has a longevity lifetime, set to maximum
 * to survive until everyone else is destroyed that might want to print
 * a message. However, objects that do not use the lifetime tracker that
 * works behind longevity singletons and were created before the registry
 * might also use the RRLIB_LOG_* macros.
 *
 * In this case, the registry is resurrected with default configuration
 * to avoid an exception that would be thrown on dead reference. Hence,
 * all further messages are processed with default configuration, unless
 * one of these objects starts to configure log domain again from its
 * destructor.
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging_configuration_DomainRegistryLifetime_h__
#define __rrlib__logging_configuration_DomainRegistryLifetime_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/design_patterns/singleton/tLifetimeTracker.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

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
//!
/*! Lifetime policy for tDomainRegistry
 *
 * In principle tDomainRegistry has a longevity lifetime, set to maximum
 * to survive until everyone else is destroyed that might want to print
 * a message. However, objects that do not use the lifetime tracker that
 * works behind longevity singletons and were created before the registry
 * might also use the RRLIB_LOG_* macros.
 *
 * In this case, the registry is resurrected with default configuration
 * to avoid an exception that would be thrown on dead reference. Hence,
 * all further messages are processed with default configuration, unless
 * one of these objects starts to configure log domain again from its
 * destructor.
 */
template <typename T>
struct DomainRegistryLifetime
{
  static void ScheduleDestruction(T *object, void (*function)())
  {
    design_patterns::singleton::SetLongevity(GetLongevity(object), function);
  }
  static void OnDeadReference()
  {
    std::cout << "INFO: Resurrecting log domain registry (probably during termination)." << std::endl;
    std::cout << "      The following messages will all be processed with default configuration!" << std::endl;
  }
};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}

#endif
