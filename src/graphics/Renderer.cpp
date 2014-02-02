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
#include "nomlib/graphics/Renderer.hpp"

namespace nom {

Renderer::Renderer ( void ):  renderer_
    { SDL_RENDERER::UniquePtr ( nullptr, priv::FreeRenderTarget ) }
{
  //NOM_LOG_TRACE ( NOM );
}

Renderer::~Renderer ( void )
{
  //NOM_LOG_TRACE ( NOM );

  // Thanks for all the fish!
}

bool Renderer::create ( SDL_WINDOW::RawPtr window, int32 rendering_driver, uint32 context_flags )
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

const Point2f Renderer::scale_factor ( void ) const
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

const IntRect Renderer::viewport ( void ) const
{
  SDL_Rect v;
  IntRect viewport; // returned

  SDL_RenderGetViewport ( this->renderer(), &v );

  return IntRect ( v.x, v.y, v.w, v.h );
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
    return Point2i::null;
  }

  return size;
}

const IntRect Renderer::bounds ( void ) const
{
  SDL_Rect clip;

  SDL_RenderGetClipRect( this->renderer(), &clip );

  return IntRect ( clip.x, clip.y, clip.w, clip.h );
}

const RendererInfo Renderer::caps ( void ) const
{
  return this->caps ( this->renderer() );
}

const RendererInfo Renderer::caps ( SDL_RENDERER::RawPtr target )
{
  RendererInfo renderer_info;
  SDL_RendererInfo info;

  if ( SDL_GetRendererInfo ( target, &info ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return renderer_info;
  }

  renderer_info.name = info.name;
  renderer_info.flags = info.flags;

  NOM_ASSERT ( info.num_texture_formats > 1 );

  for ( uint32 idx = 0; idx < info.num_texture_formats; ++idx )
  {
    renderer_info.texture_formats.push_back ( info.texture_formats[idx] );
  }

  renderer_info.texture_width = info.max_texture_width;
  renderer_info.texture_height = info.max_texture_height;

  return renderer_info;
}

bool Renderer::reset ( void ) const
{
  RendererInfo caps = this->caps();

  // Graphics hardware does not support render to texture
  if ( caps.target_texture() == false )
  {
    NOM_LOG_ERR ( NOM, "Video hardware does not support render to texture" );
    return false;
  }

  // Honor the request to reset the renderer
  if ( SDL_SetRenderTarget ( this->renderer(), nullptr ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

void Renderer::update ( void ) const
{
  SDL_RenderPresent ( this->renderer() );
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

bool Renderer::fill ( const Color4i& color )
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

bool Renderer::set_viewport ( const IntRect& bounds )
{
  // Bounds argument is null -- try setting entire viewport bounds
  if ( bounds == IntRect::null )
  {
    if ( SDL_RenderSetViewport ( this->renderer(), nullptr ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }

    return true;
  }

  // Try honoring requested clipping bounds
  SDL_Rect clip = SDL_RECT(bounds);
  if ( SDL_RenderSetViewport ( this->renderer(), &clip ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Renderer::set_color ( const Color4i& color )
{
  if ( SDL_SetRenderDrawColor ( this->renderer(), color.r, color.g, color.b, color.a ) != 0 )
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

bool Renderer::set_bounds ( const IntRect& bounds )
{
  if ( bounds == IntRect::null ) // Disable clipping bounds rectangle
  {
    if ( SDL_RenderSetClipRect( this->renderer(), nullptr ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }

    return true;
  }

  // IntRect argument is not null -- try setting the requested bounds
  SDL_Rect clip = SDL_RECT ( bounds );
  if ( SDL_RenderSetClipRect( this->renderer(), &clip ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

void* Renderer::pixels ( void ) const
{
  SDL_Rect clip;
  void* pixels = static_cast<uint32*> ( malloc (this->size().x * this->size().y * 4) );

  // In order to accurately capture our window, with respect to aspect ratio,
  // such as the case of us using device independent scaling [1], we must
  // calculate the true bounding dimensions of our rendering target.
  //
  // We did not need to do this previously [2], due to the fact that we were
  // only setting the device independent resolution *after* toggling the full
  // screen state to true.
  //
  // I *think* that the changes are more accurate because of the greater
  // resolutions screen captured from [3], *with* regard to the aspect ratio.
  // Since the screen capture dumps are intended primarily for debugging
  // purposes, the more gory details we can glean, generally the better.
  //
  // Note that what we see is still after the "device independent resolution" is
  // taken into consideration.
  //
  // 1. nom::Renderer::set_logical_size
  // 2. Before changes made in git commit SHA:
  //    155628b8dcc47187bf6f03aa760e5aede728ac09
  // 3. nom::RenderWindow::save_screenshot
  clip.x = 0;
  clip.y = 0;
  clip.w = this->size().x;
  clip.h = this->size().y;

  if ( SDL_RenderReadPixels(  this->renderer(),
                              // Use calculated bounding dimensions;
                              // Pass null to dump the pixels of the entire
                              // render target
                              &clip,
                              // Use the most optimal pixel format;
                              // Pass zero here to obtain the pixel format of
                              // the render target
                              this->caps().optimal_texture_format(),
                              // Allocated pointer to be filled in with pixels
                              // from render target
                              pixels,
                              // Pitch of our pixels pointer
                              this->size().x * 4 ) != 0 )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
    return nullptr;
  }

  return pixels;
}

} // namespace nom
