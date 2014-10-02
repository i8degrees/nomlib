/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "nomlib/librocket/UIQuestionDialogBox.hpp"

// Private headers
#include <Rocket/Core/ElementUtilities.h>

namespace nom {

UIQuestionDialogBox::UIQuestionDialogBox() :
  UIMessageBox( Point2i::null, Size2i::null )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  // Defaults
  this->set_message_id( "question" );
  this->set_response_id( "response" );
  this->set_element_type( "button" );
}

UIQuestionDialogBox::UIQuestionDialogBox  (
                                            const Point2i& pos,
                                            const Size2i& size
                                          ) :
  UIMessageBox( pos, size ),
  selection_( -1 )
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  // Defaults
  this->set_message_id( "question" );
  this->set_response_id( "response" );
  this->set_element_type( "button" );
}

UIQuestionDialogBox::~UIQuestionDialogBox()
{
  // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

const std::string& UIQuestionDialogBox::response_id() const
{
  return this->response_id_;
}

const std::string& UIQuestionDialogBox::element_type() const
{
  return this->element_type_;
}

nom::size_type UIQuestionDialogBox::num_responses() const
{
  NOM_ASSERT( this->valid() != false );

  rocket::ElementList tags;
  rocket::ElementUtilities::GetElementsByTagName( tags, this->document()->GetElementById( this->response_id().c_str() ), this->element_type().c_str() );

  return tags.size();
}

int UIQuestionDialogBox::selection() const
{
  return this->selection_;
}

std::vector<std::string> UIQuestionDialogBox::responses() const
{
  std::vector<std::string> res;
  rocket::ElementList tags;

  rocket::ElementUtilities::GetElementsByTagName( tags, this->document()->GetElementById( this->response_id().c_str() ), this->element_type().c_str() );

  for( auto itr = tags.begin(); itr != tags.end(); ++itr )
  {
    res.push_back( (*itr)->GetId().CString() );
  }

  return res;
}

std::string UIQuestionDialogBox::response( int pos ) const
{
  int idx = 0;
  std::vector<std::string> res = this->responses();

  for( auto itr = res.begin(); itr != res.end(); ++itr )
  {
    // Match found!
    if( pos == idx ) return *itr;

    ++idx;
  }

  // No matches found...
  return "\0";
}

void UIQuestionDialogBox::set_response_id( const std::string& id )
{
  this->response_id_ = id;
}

void UIQuestionDialogBox::set_element_type( const std::string& type )
{
  this->element_type_ = type;
}

void UIQuestionDialogBox::set_selection( int pos )
{
  this->selection_ = pos;
}

} // namespace nom
