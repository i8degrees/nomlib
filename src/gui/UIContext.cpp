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
#include "nomlib/gui/UIContext.hpp"

// Forward declarations (third-party)
#include <Rocket/Core/Core.h>
#include <Rocket/Core/Context.h>
#include <Rocket/Debugger/Debugger.h>

// Forward declarations
#include "nomlib/system/Event.hpp"
#include "nomlib/gui/UIContextEventHandler.hpp"

// Private headers
#include "nomlib/graphics/RenderWindow.hpp"
#include "nomlib/gui/RocketSDL2RenderInterface.hpp"

namespace nom {

UIContext::UIContext() :
  debugger_( false ),
  context_( nullptr )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_INFO );
}

UIContext::~UIContext()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_INFO );
}

void UIContext::shutdown()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::LogPriority::NOM_LOG_PRIORITY_INFO );

  if( this->context_ )
  {
    this->context_->RemoveReference();
  }
}

bool UIContext::debugger_enabled() const
{
  return this->debugger_;
}

void UIContext::enable_debugger()
{
  this->debugger_ = true;
}

void UIContext::disable_debugger()
{
  this->debugger_ = false;
}

Size2i UIContext::debugger_size() const
{
  Size2i dims( Size2i::null );

  if( this->valid() ) {

    Rocket::Core::ElementDocument* target =
      this->context()->GetDocument("rkt-debug-hook");

    NOM_ASSERT( target != nullptr );
    if( target ) {
      // NOM_DUMP( target->GetSourceURL().CString() );

      Rocket::Core::Element* body_tag =
        target->GetParentNode();

      NOM_ASSERT( body_tag != nullptr );
      if( body_tag ) {

        Rocket::Core::Element* debug_info =
          body_tag->GetElementById("rkt-debug-info");

        NOM_ASSERT( debug_info != nullptr );
        if( debug_info ) {

          dims.w = debug_info->GetBox().GetSize().x;
          dims.h = debug_info->GetBox().GetSize().y;

        } // end if debug_info
      } // end if body_tag
    } // end if target
  } // end if valid context

  return dims;
}

void UIContext::set_debugger_position(const Point2i& pos)
{
  if( this->valid() ) {

    Rocket::Core::ElementDocument* target =
      this->context()->GetDocument("rkt-debug-hook");

    NOM_ASSERT( target != nullptr );
    if( target ) {
      // NOM_DUMP( target->GetSourceURL().CString() );

      Rocket::Core::Element* body_tag =
        target->GetParentNode();

      NOM_ASSERT( body_tag != nullptr );
      if( body_tag ) {

        Rocket::Core::Element* debug_info =
          body_tag->GetElementById("rkt-debug-info");

        NOM_ASSERT( debug_info != nullptr );
        if( debug_info ) {

          Rocket::Core::Property pos_x( pos.x, Rocket::Core::Property::PX);
          Rocket::Core::Property pos_y( pos.y, Rocket::Core::Property::PX);

          debug_info->SetProperty("left", pos_x);
          debug_info->SetProperty("top", pos_y);

        } // end if debug_info
      } // end if body_tag
    } // end if target
  } // end if valid context

  // for( auto itr = tags.begin(); itr != tags.end(); ++itr ) {
  //   if( (*itr)->GetId() == "rkt-debug-info" ) {
  //     NOM_DUMP( (*itr)->GetTagName().CString() );
  //     NOM_DUMP( (*itr)->GetId().CString() );
  //   }
  // } // end for tags loop
}

Size2i UIContext::beacon_size() const
{
  Size2i dims( Size2i::null );

  if( this->valid() ) {

    Rocket::Core::ElementDocument* target =
      this->context()->GetDocument("rkt-debug-log-beacon");

    NOM_ASSERT( target != nullptr );
    if( target ) {
      // NOM_DUMP( target->GetSourceURL().CString() );

      Rocket::Core::Element* body_tag =
        target->GetParentNode();

      NOM_ASSERT( body_tag != nullptr );
      if( body_tag ) {

        Rocket::Core::Element* debug_info =
          body_tag->GetElementById("rkt-debug-log-beacon");

        NOM_ASSERT( debug_info != nullptr );
        if( debug_info ) {

          dims.w = debug_info->GetBox().GetSize().x;
          dims.h = debug_info->GetBox().GetSize().y;

        } // end if debug_info
      } // end if body_tag
    } // end if target
  } // end if valid context

  return dims;
}

void UIContext::set_beacon_position(const Point2i& pos)
{
  if( this->valid() ) {

    Rocket::Core::ElementDocument* target =
      this->context()->GetDocument("rkt-debug-log-beacon");

    NOM_ASSERT( target != nullptr );
    if( target ) {
      // NOM_DUMP( target->GetSourceURL().CString() );

      Rocket::Core::Element* body_tag =
        target->GetParentNode();

      NOM_ASSERT( body_tag != nullptr );
      if( body_tag ) {

        Rocket::Core::Element* debug_info =
          body_tag->GetElementById("rkt-debug-log-beacon");

        NOM_ASSERT( debug_info != nullptr );
        if( debug_info ) {

          Rocket::Core::Property pos_x( pos.x, Rocket::Core::Property::PX);
          Rocket::Core::Property pos_y( pos.y, Rocket::Core::Property::PX);

          debug_info->SetProperty("left", pos_x);
          debug_info->SetProperty("top", pos_y);

        } // end if debug_info
      } // end if body_tag
    } // end if target
  } // end if valid context
}

Rocket::Core::Context* UIContext::context() const
{
  return this->context_;
}

Size2i UIContext::size() const
{
  Rocket::Core::Vector2i dims( 0, 0 );

  NOM_ASSERT( this->context_ != nullptr );
  if( this->context_ )
  {
    dims = this->context_->GetDimensions();
  }

  return Size2i( dims.x, dims.y );
}

bool UIContext::valid() const
{
  NOM_ASSERT( this->context() != nullptr );

  return( this->context() != nullptr );
}

bool UIContext::create_context( const std::string& name, const Size2i& res,
                                Rocket::Core::RenderInterface* interface )
{
  Rocket::Core::Vector2i resolution( res.w, res.h );

  if( interface != nullptr )
  {
    Rocket::Core::SetRenderInterface( interface );
    this->renderer_ = interface;

    // Use provided rendering interface
    this->context_ = Rocket::Core::CreateContext( name.c_str(), resolution, interface );
  }
  else
  {
    // Use the default interface provided by Rocket::Core::SetRenderInterface.
    this->context_ = Rocket::Core::CreateContext( name.c_str(), resolution, nullptr );
  }

  if( this->context_ )
  {
    // ::initialize_debugger depends on this value
    this->res_ = res;

    this->set_size(this->res_);

    if( this->debugger_enabled() )
    {
      // Initialize Debugger as early as possible, so we can visually see
      // logging.
      this->initialize_debugger();
    }

    // Install the default event handler for this context
    this->evt_.reset( new UIContextEventHandler( this ) );

    return true;
  }

  NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                "Could not create context for libRocket." );

  return false;
}

Rocket::Core::ElementDocument*
UIContext::load_document_file( const std::string& filename )
{
  Rocket::Core::ElementDocument* doc = nullptr;

  NOM_ASSERT( this->context_ != nullptr );
  if( this->context_ != nullptr )
  {
    doc = this->context_->LoadDocument( filename.c_str() );

    if( doc ) {
      doc->RemoveReference();
    }
    else { // doc == nullptr
      NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                    "Could not load document from file:", filename );
    }
  }
  else {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not load document from file (invalid context):",
                  filename );
  }

  return doc;
}

Rocket::Core::ElementDocument* UIContext::load_mouse_cursor_file( const std::string& filename )
{
  Rocket::Core::ElementDocument* cursor = nullptr;

  if( this->context_ )
  {
    cursor = this->context_->LoadMouseCursor( filename.c_str() );

    if( cursor )
    {
      cursor->RemoveReference();
      cursor->Show();
    }
    else // cursor == nullptr
    {
      NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not load mouse cursor from file:", filename );
    }
  }
  else
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not load mouse cursor: invalid context with file:", filename );
  }

  return cursor;
}

void UIContext::show_mouse_cursor( bool state )
{
  NOM_ASSERT( this->valid() != false );
  if( this->context_ )
  {
    this->context()->ShowMouseCursor( state );
  }
}

bool UIContext::load_font( const std::string& filename )
{
  return Rocket::Core::FontDatabase::LoadFontFace( filename.c_str() );
}

void UIContext::set_size(const Size2i& dims)
{
  Point2f scale( 1.0f, 1.0f );
  Size2i res(Size2i::zero);

  nom::RocketSDL2RenderInterface* target =
    NOM_DYN_PTR_CAST( nom::RocketSDL2RenderInterface*,
                      Rocket::Core::GetRenderInterface() );
  NOM_ASSERT( target != nullptr );

  const RenderWindow* context = target->window_;
  NOM_ASSERT( context != nullptr );
  if( target && context )
  {
    SDL_RenderGetScale( context->renderer(), &scale.x, &scale.y );
  }

  // Translations for independent resolution scale dimensions (SDL2); this is
  // necessary things like the visual debugger elements to stay on-screen
  // without manually positioning it.
  res.w = dims.w / scale.x;
  res.h = dims.h / scale.y;

  this->context_->SetDimensions( Rocket::Core::Vector2i(res.w, res.h) );
}

void UIContext::process_event( const Event& ev )
{
  NOM_ASSERT( this->evt_ != nullptr );
  if( this->evt_ != nullptr )
  {
    this->evt_->process_event( ev );
  }
}

void UIContext::update()
{
  if( this->context_ )
  {
    this->context_->Update();
  }
}

void UIContext::draw()
{
  if( this->context_ )
  {
    this->context_->Render();
  }
}

// Private scope

void UIContext::initialize_debugger()
{
  // Alignment offsets for visual debugger tools
  // Point2f scale( 1.0f, 1.0f );
  // Point2i debugger_offset( Point2i::zero);
  // Point2i beacon_offset( Point2i::zero);
  // Size2i debugger_dims;
  // Size2i beacon_dims;
  // Size2i res_scale;

  if( this->valid() == false ||
      Rocket::Debugger::Initialise( this->context() ) == false )
  {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not create Debugger for libRocket." );
    return;
  }

  // Not necessary when calculations are done from ::set_size & called from
  // ::create.

  // nom::RocketSDL2RenderInterface* target =
  //   NOM_DYN_PTR_CAST( nom::RocketSDL2RenderInterface*,
  //                     Rocket::Core::GetRenderInterface() );
  // NOM_ASSERT( target != nullptr );

  // const RenderWindow* context = target->window_;
  // NOM_ASSERT( context != nullptr );
  // if( target && context )
  // {
  //   SDL_RenderGetScale( context->renderer(), &scale.x, &scale.y );
  // }

  // // Translations for independent resolution scale dimensions (SDL2)
  // res_scale.w = this->res_.w / scale.x;
  // res_scale.h = this->res_.h / scale.y;

  // debugger_dims = this->debugger_size();

  // if( debugger_dims != Size2i::null ) {

  //   // Translations for debugger position
  //   debugger_offset.x = debugger_offset.x * scale.x;
  //   debugger_offset.y = debugger_offset.y * scale.y;

  //   // Top-right corner of context output
  //   debugger_offset.x = debugger_offset.x + res_scale.w - debugger_dims.w;
  //   debugger_offset.y = debugger_offset.y;

  //   this->set_debugger_position(debugger_offset);
  // }

  // beacon_dims = this->beacon_size();

  // if( beacon_dims != Size2i::null ) {

  //   // Translations for beacon position
  //   beacon_offset.x = beacon_offset.x * scale.x;
  //   beacon_offset.y = beacon_offset.y * scale.y;

  //   // Top-right corner of context output
  //   beacon_offset.x = beacon_offset.x + res_scale.w - beacon_dims.w;
  //   beacon_offset.y = beacon_offset.y;

  //   this->set_beacon_position(beacon_offset);
  // }
}

} // namespace nom
