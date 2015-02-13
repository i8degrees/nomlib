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
#include "nomlib/graphics/shapes/Rectangle.hpp"

// Forward declarations
#include "nomlib/graphics/Texture.hpp"

namespace nom {

Rectangle::Rectangle()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_RENDER, NOM_LOG_PRIORITY_VERBOSE);
}

Rectangle::~Rectangle()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_RENDER, NOM_LOG_PRIORITY_VERBOSE);
}

Rectangle::Rectangle(const IntRect& rect, const Color4i& fill)
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_RENDER, NOM_LOG_PRIORITY_VERBOSE);

  this->set_bounds(rect);
  this->set_fill_color(fill);
}

ObjectTypeInfo Rectangle::type() const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

Shape* Rectangle::clone() const
{
  return( new self_type(*this) );
}

Texture* Rectangle::texture() const
{
  Texture* texture = new Texture();

  NOM_ASSERT(texture != nullptr);
  if( texture == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_RENDER, "Could not update cache:",
                  "failed to allocate texture memory." );
    NOM_DELETE_PTR(texture);
    return nullptr;
  }

  RenderWindow* context = nom::render_interface();
  NOM_ASSERT(context != nullptr);

  if( context == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_RENDER, "Could not update cache",
                  "invalid renderer." );
    NOM_DELETE_PTR(texture);
    return nullptr;
  }

  // Obtain the optimal pixel format for the platform
  RendererInfo caps = context->caps();

  if( texture->initialize( caps.optimal_texture_format(),
      SDL_TEXTUREACCESS_TARGET, this->size() ) == false )
  {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Could not update cache:",
                  "failed texture creation." );
    NOM_DELETE_PTR(texture);
    return nullptr;
  }

  texture->set_position( this->position() );

  if( context->set_render_target(texture) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Could not update cache:",
                  "could not set rendering target." );
    NOM_DELETE_PTR(texture);
    return nullptr;
  }

  if( context->fill( this->fill_color() ) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Could not update cache:",
                  "failed to set texture color." );
    NOM_DELETE_PTR(texture);
    return nullptr;
  }

  this->draw(*context);

  if( context->reset_render_target() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION, "Could not update cache:",
                  "failed to reset the rendering target." );
    NOM_DELETE_PTR(texture);
    return nullptr;
  }

  return texture;
}

void Rectangle::update()
{
  // Stub
}

void Rectangle::draw(RenderTarget& target) const
{
  SDL_SetRenderDrawBlendMode( target.renderer(), SDL_BLENDMODE_BLEND );

  if ( SDL_SetRenderDrawColor ( target.renderer(), this->fill_color().r, this->fill_color().g, this->fill_color().b, this->fill_color().a ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return;
  }

  // nom::Point2i & nom::Size2i -> SDL_Rect
  SDL_Rect render_coords = SDL_RECT( this->position(), this->size() );

  if ( SDL_RenderFillRect ( target.renderer(), &render_coords ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return;
  }
}

} // namespace nom
