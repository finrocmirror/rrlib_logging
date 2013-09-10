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
/*!\file    rrlib/logging/sinks/tSpeechSynthesis.h
 *
 * \author  Tobias Föhst
 *
 * \date    2013-08-07
 *
 * \brief   Contains tSpeechSynthesis
 *
 * \b tSpeechSynthesis
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__logging__sinks__tSpeechSynthesis_h__
#define __rrlib__logging__sinks__tSpeechSynthesis_h__

#include "rrlib/logging/sinks/tSink.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/speech_synthesis/tStreamBuffer.h"

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
namespace sinks
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Class declaration
//----------------------------------------------------------------------
//! SHORT_DESCRIPTION
/*!
 */
class tSpeechSynthesis : public tSink
{

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  tSpeechSynthesis(const xml::tNode &node, const tConfiguration &configuration);

  ~tSpeechSynthesis();

  virtual std::streambuf &GetStreamBuffer();

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  speech_synthesis::tStreamBuffer *stream_buffer;

};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
}


#endif
