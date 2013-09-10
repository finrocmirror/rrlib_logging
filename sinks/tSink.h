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
/*!\file    rrlib/logging/sinks/tSink.h
 *
 * \author  Tobias Föhst
 *
 * \date    2013-08-07
 *
 * \brief   Contains tSink
 *
 * \b tSink
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__sinks__tSink_h__
#define __rrlib__logging__sinks__tSink_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include <streambuf>

#include "rrlib/design_patterns/factory.h"
#include "rrlib/design_patterns/singleton.h"

#include "rrlib/xml/tNode.h"

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
class tConfiguration;

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace sinks
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------
class tSink;
typedef design_patterns::tSingletonHolder<design_patterns::tFactory<tSink, std::string, std::function<tSink *(const xml::tNode &, const tConfiguration &)>>> tSinkFactory;

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! SHORT_DESCRIPTION
/*!
 */
class tSink
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  virtual ~tSink() = 0;

  virtual std::streambuf &GetStreamBuffer() = 0;
};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
}


#endif
