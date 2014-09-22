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

// Forward declarations
#include "nomlib/librocket/RocketSDL2SystemInterface.hpp"
#include "nomlib/graphics/RenderWindow.hpp"

// Private headers
#include <Rocket/Core/StyleSheetKeywords.h>

namespace nom {

UIWidget::UIWidget( const Point2i& pos, const Size2i& dims ) :
  Transformable( pos, dims ),
  title_id_( "title" )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_INFO );
}

UIWidget::~UIWidget()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_INFO );

  this->desktop_ = nullptr;
  this->document_ = nullptr;
}

void UIWidget::set_position( const Point2i& pos )
{
  Transformable::set_position( pos );

  NOM_ASSERT( this->document() != nullptr );

  if( this->document() != nullptr )
  {
    this->document()->SetProperty( "left", rocket::Property(pos.x, rocket::Property::PX) );
    this->document()->SetProperty( "top", rocket::Property(pos.y, rocket::Property::PX) );
  }
}

void UIWidget::set_size( const Size2i& dims )
{
  Transformable::set_size( dims );

  NOM_ASSERT( this->document() != nullptr );

  if( this->document() != nullptr )
  {
    this->document()->SetProperty( "width", rocket::Property(dims.w, rocket::Property::PX) );
    this->document()->SetProperty( "height", rocket::Property(dims.h, rocket::Property::PX) );
  }
}

rocket::Context* UIWidget::desktop() const
{
  return this->desktop_;
}

rocket::ElementDocument* UIWidget::document() const
{
  return this->document_;
}

bool UIWidget::valid() const
{
  NOM_ASSERT( this->desktop() != nullptr );
  NOM_ASSERT( this->document() != nullptr );

  if( this->desktop() != nullptr && this->document() != nullptr )
  {
    return true;
  }

  return false;
}

bool UIWidget::visible() const
{
  NOM_ASSERT( this->valid() != false );

  if( this->document() )
  {
    return this->document()->IsVisible();
  }

  return false;
}

std::string UIWidget::title() const
{
  NOM_ASSERT( this->valid() != false );

  if( this->document() != nullptr )
  {
    return this->document()->GetTitle().CString();
  }

  return "\0";
}

const std::string& UIWidget::title_id() const
{
  return this->title_id_;
}

uint32 UIWidget::alignment( rocket::Element* target ) const
{
  int align = -1;

  NOM_ASSERT( this->valid() != false );

  if( this->valid() == false || target == nullptr )
  {
    return Anchor::None;
  }

  // We cannot use the element's ::GetTextAlign method because it does not
  // reflect any updates we make through setting properties.
  const rocket::Property* alignment = target->GetProperty("text-align");

  align = alignment->Get<int>();

  switch( align )
  {
    default:
    case -1:
    case rocket::TEXT_ALIGN_JUSTIFY: return Anchor::None; break;

    case rocket::TEXT_ALIGN_LEFT: return Anchor::Left; break;
    case rocket::TEXT_ALIGN_RIGHT: return Anchor::Right; break;
    case rocket::TEXT_ALIGN_CENTER: return Anchor::Center; break;
  }
}

bool UIWidget::set_desktop( rocket::Context* ctx )
{
  NOM_ASSERT( ctx != nullptr );

  if( ctx == nullptr )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not set desktop: invalid pointer." );
    return false;
  }

  this->desktop_ = ctx;

  return true;
}

bool UIWidget::set_document_file( const std::string& filename )
{
  rocket::ElementDocument* doc = this->desktop()->LoadDocument( filename.c_str() );

  if( doc )
  {
    this->document_ = doc;

    this->document()->RemoveReference();

    return true;
  }

  NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not load document from file:", filename );

  return false;
}

void UIWidget::set_document_size()
{
  rocket::ElementList tags;

  rocket::ElementUtilities::GetElementsByTagName( tags, this->document()->GetParentNode(), "body" );

  for( auto itr = tags.begin(); itr != tags.end(); ++itr )
  {
    if( (*itr) && (*itr)->GetClassNames().Find( "window" ) != nom::npos )
    {
      if( this->size() == Size2i::null )
      {
        this->set_size( Size2i( (*itr)->GetBox().GetSize().x, (*itr)->GetBox().GetSize().y ) );
      }
      else
      {
        this->set_size( this->size() );
      }
    }
  }
}

void UIWidget::show()
{
  if( this->document() )
  {
    this->document()->Show();
  }
}

void UIWidget::hide()
{
  if( this->document() )
  {
    this->document()->Hide();
  }
}

void UIWidget::close()
{
  if( this->document() )
  {
    this->document()->Close();
  }
}

void UIWidget::set_title( const std::string& text )
{
  NOM_ASSERT( this->valid() != false );

  if( this->document() )
  {
    this->document()->GetElementById( this->title_id().c_str() )->SetInnerRML( text.c_str() );
  }
}

void UIWidget::set_title_id( const std::string& id )
{
  this->title_id_ = id;
}

void UIWidget::set_font( rocket::Element* target, const std::string& font )
{
  NOM_ASSERT( this->valid() != false );

  NOM_ASSERT( target != nullptr );

  if( target )
  {
    target->SetProperty( "font-family", rocket::Property( font.c_str(), rocket::Property::STRING ) );
  }
}

void UIWidget::set_font_size( rocket::Element* target, int point_size )
{
  NOM_ASSERT( this->document() != nullptr );

  NOM_ASSERT( target != nullptr );

  if( target )
  {
    target->SetProperty( "font-size", rocket::Property( point_size, rocket::Property::PX ) );
  }
}

void UIWidget::set_alignment( rocket::Element* element, uint32 alignment )
{
  std::string align = "\0";

  NOM_ASSERT( this->document() != nullptr );

  NOM_ASSERT( element != nullptr );

  if( this->document() == nullptr || element == nullptr ) return;

  // Convert uint32 nom::Alignment type to string for libRocket
  switch( alignment )
  {
    default:
    case Alignment::NONE:
    case Anchor::Left:
    case Anchor::TopLeft:
    case Anchor::MiddleLeft:
    case Anchor::BottomLeft:
    {
      align = "left";
      break;
    }

    case Anchor::Center:
    case Anchor::TopCenter:
    case Anchor::MiddleCenter:
    case Anchor::BottomCenter:
    {
      align = "center";
      break;
    }

    case Anchor::Right:
    case Anchor::TopRight:
    case Anchor::MiddleRight:
    case Anchor::BottomRight:
    {
      align = "right";
      break;
    }
  }

  if( element->SetProperty( "text-align", align.c_str() ) == false )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_GUI, "Failed to set text-align property for", element->GetId().CString() );
    // return false;
  }
}

void UIWidget::register_event_listener( rocket::Element* element,
                                        const std::string& ev,
                                        UIEventListener* observer,
                                        bool interruptible )
{
  element->AddEventListener( ev.c_str(), observer, interruptible );
}

UIMessageBox::UIMessageBox() :
  UIWidget( Point2i::null, Size2i::null )
  // message_id_( "message" )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

UIMessageBox::UIMessageBox( const Point2i& pos, const Size2i& dims ) :
  UIWidget( pos, dims ),
  message_id_( "message" )

{
  // NOM_LOG_TRACE( NOM );
}

UIMessageBox::~UIMessageBox()
{
  // NOM_LOG_TRACE( NOM );
}

bool UIMessageBox::initialize()
{
  NOM_ASSERT( this->valid() != false );

  // Resulting elements that match query
  rocket::ElementList tags;

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

void UIMessageBox::draw( RenderTarget& target ) const
{
  // ...
}

// Protected scope

void UIMessageBox::update()
{
  // ...
}

} // namespace nom
