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
#include "nomlib/librocket/UIMessageBox.hpp"

// Private headers
#include <Rocket/Core/ElementUtilities.h>
// #include <Rocket/Core/StyleSheetKeywords.h>

namespace nom {

UIMessageBox::UIMessageBox() :
  UIWidget( Point2i::null, Size2i::null )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  // Defaults
  this->set_message_id( "message" );
}

UIMessageBox::UIMessageBox( const Point2i& pos, const Size2i& dims ) :
  UIWidget( pos, dims )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  // Defaults
  this->set_message_id( "message" );
}

UIMessageBox::~UIMessageBox()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

bool UIMessageBox::initialize()
{
  NOM_ASSERT( this->valid() != false );

  if( this->valid() == true )
  {
    this->set_position( this->position() );

    this->set_document_size();

    this->show();

    return true;
  }

  return false;
}

bool UIMessageBox::enabled() const
{
  return this->visible();
}

std::string UIMessageBox::title_text() const
{
  return this->title();
}

std::string UIMessageBox::message_text() const
{
  NOM_ASSERT( this->valid() != false );

  if( this->document() != nullptr )
  {
    return this->document()->GetElementById( this->message_id().c_str() )->GetInnerRML().CString();
  }

  return "\0";
}

IntRect UIMessageBox::title_bounds() const
{
  IntRect bounds;

  NOM_ASSERT( this->valid() != false );

  rocket::Element* title = this->document()->GetElementById( this->title_id().c_str() );

  NOM_ASSERT( title != nullptr );

  if( title )
  {
    bounds.x = title->GetAbsoluteOffset().x;
    bounds.y = title->GetAbsoluteOffset().y;
    bounds.w = rocket::ElementUtilities::GetStringWidth( title, this->title_text().c_str() );
    bounds.h = rocket::ElementUtilities::GetLineHeight( title );

    return bounds;
  }

  return IntRect::null;
}

IntRect UIMessageBox::message_bounds() const
{
  IntRect bounds;

  NOM_ASSERT( this->valid() != false );

  rocket::Element* msg = this->document()->GetElementById( this->message_id().c_str() );

  NOM_ASSERT( msg != nullptr );

  if( msg )
  {
    bounds.x = msg->GetAbsoluteOffset().x;
    bounds.y = msg->GetAbsoluteOffset().y;
    bounds.w = rocket::ElementUtilities::GetStringWidth( msg, this->message_text().c_str() );
    bounds.h = rocket::ElementUtilities::GetLineHeight( msg );

    return bounds;
  }

  return IntRect::null;
}

uint32 UIMessageBox::title_alignment() const
{
  NOM_ASSERT( this->valid() != false );

  rocket::Element* target =
    this->document()->GetElementById( this->title_id().c_str() );

  return this->alignment( target );
}

uint32 UIMessageBox::message_alignment() const
{
  NOM_ASSERT( this->valid() != false );

  rocket::Element* target =
    this->document()->GetElementById( this->message_id().c_str() );

  return this->alignment( target );
}

const std::string& UIMessageBox::message_id() const
{
  return this->message_id_;
}

void UIMessageBox::disable()
{
  this->hide();
}

void UIMessageBox::enable()
{
  this->show();
}

void UIMessageBox::set_title( const std::string& text, const std::string& font, uint point_size )
{
  this->set_title_text( text );
  this->set_title_font( font );
  this->set_title_font_size( point_size );
  this->set_title_alignment( Anchor::TopLeft );
}

void UIMessageBox::set_title_text( const std::string& text )
{
  UIWidget::set_title( text );
}

void UIMessageBox::set_title_font( const std::string& font )
{
  this->set_font( this->document()->GetElementById( this->title_id().c_str() ), font );
}

void UIMessageBox::set_title_font_size( int point_size )
{
  this->set_font_size( this->document()->GetElementById( this->title_id().c_str() ), point_size );
}

void UIMessageBox::set_title_alignment( uint32 align )
{
  NOM_ASSERT( this->valid() != false );

  rocket::Element* element = this->document()->GetElementById( this->title_id().c_str() );

  this->set_alignment( element, align );
}

void UIMessageBox::set_message( const std::string& text, const std::string& font, uint point_size )
{
  this->set_message_text( text );
  this->set_message_font( font );
  this->set_message_font_size( point_size );
  this->set_message_alignment( Anchor::MiddleCenter );
}

void UIMessageBox::set_message_text( const std::string& text )
{
  NOM_ASSERT( this->valid() != false );

  rocket::Element* content = this->document()->GetElementById( this->message_id().c_str() );

  NOM_ASSERT( content != nullptr );

  // std::shared_ptr<rocket::Element> container;
  // container.reset( this->document()->CreateElement( "p" ), free_element );

  // NOM_ASSERT( container != nullptr );

  // std::shared_ptr<rocket::ElementText> message;
  // message.reset( this->document()->CreateTextNode( text.c_str() ), free_element );

  // NOM_ASSERT( message != nullptr );

  content->SetInnerRML( text.c_str() );

  // FIXME: I believe that the second method call is the correct way -- we get
  // '<p>Hello, World</p>' if we disable the first and enable the second...
  // Otherwise, what's the point of creating the container???
  // content->AppendChild( message.get() );

  // content->AppendChild( container.get() );
}

void UIMessageBox::set_message_font( const std::string& font )
{
  this->set_font( this->document()->GetElementById( this->message_id().c_str() ), font );
}

void UIMessageBox::set_message_font_size( int point_size )
{
  this->set_font_size( this->document()->GetElementById( this->message_id().c_str() ), point_size );
}

void UIMessageBox::set_message_alignment( uint32 align )
{
  NOM_ASSERT( this->valid() != false );

  rocket::Element* element = this->document()->GetElementById( this->message_id().c_str() );

  this->set_alignment( element, align );
}

void UIMessageBox::set_message_id( const std::string& id )
{
  this->message_id_ = id;
}

} // namespace nom
