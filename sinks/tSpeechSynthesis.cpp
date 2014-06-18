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
/*!\file    rrlib/logging/sinks/tSpeechSynthesis.cpp
 *
 * \author  Tobias Föhst
 *
 * \date    2013-08-07
 *
 */
//----------------------------------------------------------------------
#include "rrlib/logging/sinks/tSpeechSynthesis.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/speech_synthesis/voices.h"
#include <iostream>
//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

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
const std::string cSEPARATOR = "://";

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

namespace
{
speech_synthesis::tStreamBuffer *SpeechStreamBufferFromVoiceString(const std::string &voice_string)
{
  size_t separator_position = voice_string.find(cSEPARATOR);
  if (separator_position != std::string::npos)
  {
    std::string synthesis = voice_string.substr(0, separator_position);
    std::string voice = voice_string.substr(separator_position + cSEPARATOR.length());
    return new speech_synthesis::tStreamBuffer(*speech_synthesis::tVoiceFactory::Instance().Create(synthesis, voice));
  }
  return NULL;
}
}

//----------------------------------------------------------------------
// tSpeechSynthesis constructors
//----------------------------------------------------------------------
tSpeechSynthesis::tSpeechSynthesis(const std::string &voice) :
  stream_buffer(SpeechStreamBufferFromVoiceString(voice))
{}

#ifdef _LIB_RRLIB_XML_PRESENT_
tSpeechSynthesis::tSpeechSynthesis(const xml::tNode &node, const tConfiguration &configuration) :
  stream_buffer(NULL)
{
  if (!node.HasAttribute("voice"))
  {
    throw std::runtime_error("Attribute voice is missing for speech_synthesis logging sink!");
  }

  this->stream_buffer = SpeechStreamBufferFromVoiceString(node.GetStringAttribute("voice"));
}
#endif

//----------------------------------------------------------------------
// tSpeechSynthesis destructor
//----------------------------------------------------------------------
tSpeechSynthesis::~tSpeechSynthesis()
{
  delete this->stream_buffer;
}

//----------------------------------------------------------------------
// tSpeechSynthesis GetStreamBuffer
//----------------------------------------------------------------------
std::streambuf &tSpeechSynthesis::GetStreamBuffer()
{
  return *this->stream_buffer;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
}
