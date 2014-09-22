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
#include "nomlib/librocket/UIContext.hpp"

// Forward declarations (third-party)
#include <Rocket/Core/Core.h>
#include <Rocket/Core/Context.h>
#include <Rocket/Debugger/Debugger.h>

// Forward declarations
#include "nomlib/system/Event.hpp"

// Private headers
#include "nomlib/librocket/UIContextEventHandler.hpp"

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

Rocket::Core::Context* UIContext::context() const
{
  return this->context_;
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
    if( this->debugger_enabled() )
    {
      // Initialize Debugger as early as possible, so we can visually see logging.
      if( Rocket::Debugger::Initialise( this->context_ ) == false )
      {
        NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not create Debugger for libRocket." );
        return false;
      }
    }

    // Install the default event handler for this context
    this->evt_.reset( new UIContextEventHandler( this ) );

    return true;
  }

  NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not create context for libRocket." );

  return false;
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

bool UIContext::load_font( const std::string& filename )
{
  return Rocket::Core::FontDatabase::LoadFontFace( filename.c_str() );
}

void UIContext::resize( const Size2i& dims )
{
  this->context_->SetDimensions( Rocket::Core::Vector2i( dims.w, dims.h ) );
}

void UIContext::process_event( const Event& ev )
{
  NOM_ASSERT( this->evt_ != nullptr );

  this->evt_->process_event( ev );
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

} // namespace nom
