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
/*!\file    log_levels.h
 *
 * \author  Tobias Foehst
 *
 * \date    2011-09-15
 *
 * \brief Contains the available log level definitions
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__include_guard__
#error Invalid include directive. Try #include "rrlib/logging/messages.h" instead.
#endif

#ifndef __rrlib__logging__log_levels_h__
#define __rrlib__logging__log_levels_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
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
//! Enumeration type that contains the available message levels
enum tLogLevel
{
  eLL_USER,             //!< Information for user (including end-users). Is always shown.
  eLL_ERROR,            //!< Error message. Used to inform about _certain_ malfunction of application. Is always shown.
  eLL_WARNING,          //!< Critical warning. Warns about possible application malfunction and invalid (and therefore discarded) user input. (default max level with _RRLIB_LOGGING_LESS_OUTPUT_)
  eLL_DEBUG_WARNING,    //!< Debug info with warning character (e.g. "Parameter x not set - using default y")
  eLL_DEBUG,            //!< Debug info about coarse program flow (default max level without _RRLIB_LOGGING_LESS_OUTPUT_) - information possibly relevant to developers outside of respective domain
  eLL_DEBUG_VERBOSE_1,  //!< Higher detail debug info (not available in release mode) - only relevant to developers in respective domain
  eLL_DEBUG_VERBOSE_2,  //!< Higher detail debug info (not available in release mode) - only relevant to developers in respective domain
  eLL_DEBUG_VERBOSE_3,  //!< Higher detail debug info (not available in release mode) - only relevant to developers in respective domain
  eLL_DIMENSION         //!< Endmarker and dimension of eLogLevel
};

}
}



#endif
