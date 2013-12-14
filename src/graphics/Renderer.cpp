/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/graphics/Renderer.hpp"

namespace nom {

Renderer::Renderer ( void ):  renderer_
    { SDL_RENDERER::UniquePtr ( nullptr, priv::FreeRenderTarget ) }
{
NOM_LOG_TRACE ( NOM );

}

Renderer::~Renderer ( void )
{
NOM_LOG_TRACE ( NOM );

  // Thanks for all the fish!
}

bool Renderer::initialize ( SDL_WINDOW::RawPtr window, int32 rendering_driver, uint32 context_flags )
{
NOM_LOG_TRACE ( NOM );

  this->renderer_.reset ( SDL_CreateRenderer ( window, rendering_driver, context_flags ) );

  if ( this->renderer_valid() == false ) return false;

  return true;
}

SDL_RENDERER::RawPtr Renderer::renderer ( void ) const
{
  return this->renderer_.get();
}

bool Renderer::renderer_valid ( void ) const
{
  if ( this->renderer() != nullptr )
  {
    return true;
  }
  else
  {
    return false;
  }
}

const Point2i Renderer::scale_factor ( void ) const
{
  Point2f scale_factor;

  SDL_RenderGetScale ( this->renderer(), &scale_factor.x, &scale_factor.y );

  return scale_factor;
}

const Point2i Renderer::logical_size ( void ) const
{
  Point2i size;

  SDL_RenderGetLogicalSize ( this->renderer(), &size.x, &size.y );

  return size;
}

const Coords Renderer::viewport ( void ) const
{
  SDL_Rect v;
  Coords view_pos;

  SDL_RenderGetViewport ( this->renderer(), &v );

  view_pos.x = v.x;
  view_pos.y = v.y;
  view_pos.width = v.w;
  view_pos.height = v.h;

  return view_pos;
}

const SDL_BlendMode Renderer::blend_mode ( void ) const
{
  SDL_BlendMode blend;

  if ( SDL_GetRenderDrawBlendMode ( this->renderer(), &blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return blend;
  }

  return blend;
}

const Point2i Renderer::size ( void ) const
{
  Point2i size; // width & height in pixels

  if ( SDL_GetRendererOutputSize ( this->renderer(), &size.x, &size.y ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return Point2i( -1, -1 );
    //return Point2i::null;
  }

  return size;
}

const Coords Renderer::bounds ( void ) const
{
  SDL_Rect clip_bounds;

  SDL_RenderGetClipRect( this->renderer(), &clip_bounds );

  return Coords( clip_bounds.x, clip_bounds.y, clip_bounds.w, clip_bounds.h );
}

void Renderer::update ( void ) const
{
  SDL_RenderPresent ( this->renderer() );
}

bool Renderer::update ( SDL_TEXTURE::RawPtr input_texture )
{
  if ( SDL_SetRenderTarget ( this->renderer(), input_texture ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

bool Renderer::clear ( void ) const
{
  if ( SDL_RenderClear ( this->renderer() ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Renderer::fill ( const Color4u& color )
{
  if ( this->set_color ( color ) == false )
  {
    return false;
  }

  if ( this->clear() == false )
  {
    return false;
  }

  return true;
}

bool Renderer::set_logical_size ( int width, int height )
{
  if ( SDL_RenderSetLogicalSize ( this->renderer(), width, height ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

bool Renderer::set_viewport ( const Coords& rect )
{
  if ( rect != Coords::null )
  {
    SDL_Rect area = SDL_RECT(rect);

    if ( SDL_RenderSetViewport ( this->renderer(), &area ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }
  }
  else
  {
    if ( SDL_RenderSetViewport ( this->renderer(), nullptr ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }
  }

  return true;
}

bool Renderer::set_color ( const Color4u& color )
{
  if ( SDL_SetRenderDrawColor ( this->renderer(), color.red, color.green, color.blue, color.alpha ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

bool Renderer::set_blend_mode ( const SDL_BlendMode mode )
{
  if ( SDL_SetRenderDrawBlendMode ( this->renderer(), mode ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

bool Renderer::set_bounds ( const Coords& clip_bounds )
{
  if ( clip_bounds == Coords::null ) // Disable clipping bounds rectangle
  {
    if ( SDL_RenderSetClipRect( this->renderer(), nullptr ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }

    return true;
  }

  SDL_Rect bounds = SDL_RECT ( clip_bounds );
  if ( SDL_RenderSetClipRect( this->renderer(), &bounds ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

void* Renderer::pixels ( void ) const
{
  void* pixels = static_cast<uint32*> ( malloc (this->size().x * this->size().y * 4) );

  if ( SDL_RenderReadPixels(  this->renderer(),
                              nullptr,  // Dump pixels of the entire target
                              0,        // Use the pixel format of the render
                                        // target.
                              pixels,   // Pointer to be filled in with pixels
                                        // from rendering target.
                              this->size().x * 4 ) != 0 )
  {
NOM_LOG_ERR( NOM, SDL_GetError() );
    return nullptr;
  }

  return pixels;
}

} // namespace nom
