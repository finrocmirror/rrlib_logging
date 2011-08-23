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
/*!\file    debugging.h
 *
 * \author  Tobias Foehst
 *
 * \date    2010-06-21
 *
 * \brief
 *
 * \b
 *
 * A few words for debugging.h
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib_logging__test__debugging_h__
#define __rrlib_logging__test__debugging_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/logging/definitions.h"

#define DEBUGMSG(args...)   RRLIB_LOG_PRINTF(rrlib::logging::eLL_DEBUG_VERBOSE_1, args)
#define INFOMSG(args...)    RRLIB_LOG_PRINTF(rrlib::logging::eLL_DEBUG, args)
#define WARNINGMSG(args...) RRLIB_LOG_PRINTF(rrlib::logging::eLL_WARNING, args)
#define ERRORMSG(args...)   RRLIB_LOG_PRINTF(rrlib::logging::eLL_ERROR, args)
#define USERMSG(args...)    RRLIB_LOG_PRINTF(rrlib::logging::eLL_USER, args)

#endif
