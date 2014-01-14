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
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/scale2x/scale2x.hpp"
#include "nomlib/graphics/hqx/hqx.hpp"

namespace nom {

Texture::Texture ( void ) :
  texture_ { nullptr, priv::FreeTexture },
  pixels_ ( nullptr ),
  pitch_ ( 0 ),
  position_ ( 0, 0 ),
  bounds_ ( 0, 0, -1, -1 ),
  colorkey_ { Color4i::Black }
{
  NOM_LOG_TRACE ( NOM );
}

Texture::~Texture ( void )
{
  NOM_LOG_TRACE ( NOM );
}

Texture::Texture ( const Texture& copy ) :
  texture_ { copy.texture(), priv::FreeTexture },
  pixels_ { copy.pixels() },
  pitch_ { copy.pitch() },
  position_ { copy.position() },
  bounds_ { copy.bounds() },
  colorkey_ { copy.colorkey() }
{
  NOM_LOG_TRACE ( NOM );
}

Texture& Texture::operator = ( const Texture& other )
{
  this->texture_ = other.texture_;
  this->pixels_ = other.pixels();
  this->pitch_ = other.pitch();
  this->position_ = other.position();
  this->bounds_ = other.bounds();

  return *this;
}

Texture::SharedPtr Texture::clone ( void ) const
{
  return Texture::SharedPtr ( new Texture ( *this ) );
}

bool Texture::initialize ( uint32 format, uint32 flags, int32 width, int32 height )
{
  this->texture_.reset ( SDL_CreateTexture ( Window::context(), format, flags, width, height ), priv::FreeTexture );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  // TODO: cache width & height?

  // Cache the size of our new Texture object with the existing surface info
  this->set_bounds( IntRect(0, 0, width, height) );

  return true;
}

bool Texture::create ( const Image& source )
{
  // Static access type
  this->texture_.reset ( source.texture(), priv::FreeTexture );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  // TODO: cache width & height?

  // Cache the size of our new Texture object with the existing surface info
  this->set_bounds ( IntRect(0, 0, source.width(), source.height()) );

  return true;
}

bool Texture::create ( const Image& source, uint32 pixel_format, enum Texture::Access type )
{
  if ( type == Texture::Access::Streaming )
  {
    if ( this->initialize ( pixel_format, SDL_TEXTUREACCESS_STREAMING, source.width(), source.height() ) == false )
    {
      NOM_LOG_ERR ( NOM, "Failed to initialize streaming texture." );
      return false;
    }

    if ( this->lock ( source.bounds() ) == false ) // NOT safe for writing
    {
      NOM_LOG_ERR(NOM, "Could not lock Texture for writing");
      return false;
    }

    // Copy source pixels to our new Texture
    if ( this->copy_pixels ( source.pixels(), source.pitch() * source.height() ) == false )
    {
      NOM_LOG_ERR(NOM, "Could not copy pixels to initialized Texture." );
      this->unlock();
      return false;
    }

    // Once we unlock the texture, it will be uploaded to the GPU
    this->unlock();
  }
  else if ( type == Texture::Access::RenderTarget )
  {
    if ( this->initialize ( pixel_format, SDL_TEXTUREACCESS_TARGET, source.width(), source.height() ) == false )
    {
      NOM_LOG_ERR ( NOM, "Failed to initialize texture to render target." );
      return false;
    }
  }
  else if ( type == Texture::Access::Static )
  {
/*
    if ( source.pixel_format() != pixel_format )
    {
      // TODO: conversion of existing pixel format to requested pixel format; is
      // this even necessary?
      NOM_LOG_ERR( NOM, "STUB FUNCTION" );
      return false;
    }
*/
    if ( this->create ( source ) == false )
    {
      NOM_LOG_ERR ( NOM, "Failed to create Texture with access type: " + std::to_string(type) );
      return false;
    }
  }
  else // Invalid Texture::Access type
  {
    NOM_LOG_ERR ( NOM, "Failed to initialize texture: invalid enumeration type." );
    return false;
  }

  return true;
}

SDL_TEXTURE::RawPtr Texture::texture ( void ) const
{
  return this->texture_.get();
}

bool Texture::valid ( void ) const
{
  if ( this->texture() != nullptr ) return true;

  return false;
}

enum Texture::Access Texture::access ( void ) const
{
  int type = 0;

  if ( SDL_QueryTexture ( this->texture(), nullptr, &type, nullptr, nullptr ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return Texture::Access::Invalid;
  }

  if ( type == SDL_TEXTUREACCESS_STATIC ) return Texture::Access::Static;
  if ( type == SDL_TEXTUREACCESS_STREAMING ) return Texture::Access::Streaming;

  return Texture::Access::Invalid;
}

const Point2i& Texture::position ( void ) const
{
  return this->position_;
}

const IntRect& Texture::bounds ( void ) const
{
  return this->bounds_;
}

void Texture::set_position ( const Point2i& pos )
{
  this->position_.x = pos.x;
  this->position_.y = pos.y;
}

void Texture::set_bounds ( const IntRect& bounds )
{
  this->bounds_ = bounds;
}

int32 Texture::width ( void ) const
{
  int32 tex_width;

  if ( SDL_QueryTexture ( this->texture(), nullptr, nullptr, &tex_width, nullptr ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return -1;
  }

  return tex_width;
}

int32 Texture::height ( void ) const
{
  int32 tex_height;

  if ( SDL_QueryTexture ( this->texture(), nullptr, nullptr, nullptr, &tex_height ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return -1;
  }

  return tex_height;
}

int Texture::pitch ( void ) const
{
  return pitch_;
}

void* Texture::pixels ( void ) const
{
  return this->pixels_;
}

uint8 Texture::bits_per_pixel ( void ) const
{
  switch ( this->bytes_per_pixel() )
  {
    case 1: return 8; break; // 8-bit bpp
    case 2: return 16; break; // 16-bit bpp
    case 3: return 24; break; // 24-bit bpp
    default: // Unknown color depth; log a debugging message & assume 32-bit bpp
    {
      NOM_LOG_ERR ( NOM, "Could not determine color depth for pixel reading; assuming 32-bit" );
    }
    case 4: return 32; break; // 32-bit bpp
  }
}

uint8 Texture::bytes_per_pixel ( void ) const
{
  return SDL_BYTESPERPIXEL ( this->pixel_format() );
}

uint32 Texture::pixel_format ( void ) const
{
  uint32 format = 0;

  if ( SDL_QueryTexture ( this->texture(), &format, nullptr, nullptr, nullptr ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return 0;
  }

  return format;
}

const SDL_BlendMode Texture::blend_mode ( void ) const
{
  SDL_BlendMode blend;

  if ( SDL_GetTextureBlendMode ( this->texture(), &blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return blend;
  }

  return blend;
}

const Color4i& Texture::colorkey ( void ) const
{
  return this->colorkey_;
}

const uint8 Texture::alpha ( void ) const
{
  uint8 alpha;

  if ( SDL_GetTextureAlphaMod ( this->texture(), &alpha ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return Color4i::ALPHA_OPAQUE;
  }

  return alpha;
}

const Color4i Texture::color_modulation ( void ) const
{
  SDL_Color c;

  if ( SDL_GetTextureColorMod ( this->texture(), &c.r, &c.g, &c.b ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return Color4i::null;
  }

  return Color4i ( c.r, c.g, c.b, Color4i::ALPHA_OPAQUE );
}

const Point2i Texture::maximum_size ( void )
{
  RendererInfo info;
  info = Window::caps ( Window::context() );

  return Point2i ( info.texture_width, info.texture_height );
}

bool Texture::locked ( void ) const
{
  if ( this->pixels() == nullptr ) return false;

  return true;
}

bool Texture::lock ( void )
{
  if ( this->lock( IntRect::null ) == false ) return false;

  return true;
}

bool Texture::lock ( const IntRect& bounds )
{
  if ( this->locked() )
  {
    NOM_LOG_ERR ( NOM, "Texture is already locked." );
    return false;
  }

  // Lock entire texture
  if ( bounds == IntRect::null )
  {
    if ( SDL_LockTexture ( this->texture(), nullptr, &this->pixels_, &this->pitch_ ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }

    return true;
  }

  // Lock specified area of texture
  SDL_Rect clip = SDL_RECT(bounds);
  if ( SDL_LockTexture ( this->texture(), &clip, &this->pixels_, &this->pitch_ ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

void Texture::unlock ( void )
{
  if ( this->pixels() == nullptr )
  {
NOM_LOG_INFO ( NOM, "Texture is not locked." );
    return;
  }

  SDL_UnlockTexture ( this->texture() );
  this->pixels_ = nullptr;
  this->pitch_ = 0;
}

bool Texture::load  ( const std::string& filename,
                      bool use_cache,
                      enum Texture::Access type
                    )
{
  Image source;

  if ( source.load ( filename ) == false ) return false;

  RendererInfo caps = Window::caps( Window::context() );

  if ( type != Texture::Access::Static )
  {
    if ( this->create ( source, caps.optimal_texture_format(), type ) == false )
    {
      NOM_LOG_ERR ( NOM, "Failed to create Texture with type: " + std::to_string(type) );
      return false;
    }
  }
  else // Default texture access type: Texture::Access::Static
  {
    if ( this->create ( source ) == false )
    {
      NOM_LOG_ERR ( NOM, "Failed to create Texture with access type: " + std::to_string(type) );
      return false;
    }
  }

  // Set our default blending mode for texture copies
  this->set_blend_mode( SDL_BLENDMODE_BLEND );
  // Update our Texture clipping bounds with the new source
  this->set_bounds ( source.bounds() );

  return true;
}

bool Texture::update ( const void* pixels, uint16 pitch, const IntRect& bounds )
{
  // Update entire texture
  if ( bounds == IntRect::null )
  {
    if ( SDL_UpdateTexture ( this->texture(), nullptr, pixels, pitch ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }
    return true;
  }

  // Try to honor requested bounds
  SDL_Rect clip = SDL_RECT(bounds);
  if ( SDL_UpdateTexture ( this->texture(), &clip, pixels, pitch ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

void Texture::draw ( SDL_RENDERER::RawPtr target ) const
{
  Point2i pos = this->position();
  SDL_Rect render_coords;

  render_coords.x = pos.x;
  render_coords.y = pos.y;

  // Use preset clipping bounds for the width and height of this texture
  //
  // These two coordinates can be used for rendering a rescaled texture
  render_coords.w = this->bounds().w;
  render_coords.h = this->bounds().h;

  // Render with set clipping bounds; we are rendering only a portion of a
  // larger Texture; think: sprite sheets.
  if ( this->bounds().w != -1 && this->bounds().h != -1 )
  {
    SDL_Rect render_bounds;
    render_bounds.x = this->bounds().x;
    render_bounds.y = this->bounds().y;
    render_bounds.w = this->bounds().w;
    render_bounds.h = this->bounds().h;
    if ( SDL_RenderCopy ( target, this->texture(), &render_bounds, &render_coords ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return;
    }
  }
  else // Render the entire Texture we have in memory
  {
    if ( SDL_RenderCopy ( target, this->texture(), nullptr, &render_coords ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return;
    }
  }
}

void Texture::draw ( const Window& target ) const
{
  this->draw ( target.renderer() );
}

void Texture::draw ( SDL_RENDERER::RawPtr target, const double angle ) const
{
  Point2i pos = this->position();
  SDL_Rect render_coords;

  render_coords.x = pos.x;
  render_coords.y = pos.y;

  // Use preset clipping bounds for the width and height of this texture
  render_coords.w = this->bounds().w;
  render_coords.h = this->bounds().h;

  // Render with set clipping bounds; we are rendering only a portion of a
  // larger Texture; think: sprite sheets.
  if ( this->bounds().w != -1 && this->bounds().h != -1 )
  {
    SDL_Rect render_bounds;
    render_bounds.x = this->bounds().x;
    render_bounds.y = this->bounds().y;
    render_bounds.w = this->bounds().w;
    render_bounds.h = this->bounds().h;
    if ( SDL_RenderCopyEx ( target, this->texture(), &render_bounds, &render_coords, angle, nullptr, SDL_FLIP_NONE ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return;
    }
  }
  else // Render the entire Texture we have in memory
  {
    if ( SDL_RenderCopyEx ( target, this->texture(), nullptr, &render_coords, angle, nullptr, SDL_FLIP_NONE ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
      return;
    }
  }
}

void Texture::draw ( const Window& target, const double degrees ) const
{
  this->draw ( target.renderer(), degrees );
}

bool Texture::set_alpha ( uint8 opacity )
{
NOM_ASSERT ( ! ( opacity > Color4i::ALPHA_OPAQUE ) || ( opacity < Color4i::ALPHA_TRANSPARENT ) );

  if ( SDL_SetTextureAlphaMod ( this->texture(), opacity ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

uint32 Texture::pixel ( int32 x, int32 y )
{
  switch ( this->bits_per_pixel() )
  {
    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( this->pixels() );

      return pixels[ ( y * this->pitch() ) + x ];
      break;
    }

    case 16:
    {
      uint16* pixels = static_cast<uint16*> ( this->pixels() );

      return pixels[ ( y * this->pitch() / 2 ) + x ];
      break;
    }

    case 24:
    {
      uint8* pixels = static_cast<uint8*> ( this->pixels() );

      return pixels[ ( y * this->pitch() ) + x ];
      break;
    }

    default: // Unknown color depth; assuming 32-bit bpp
    case 32:
    {
      uint32* pixels = static_cast<uint32*> ( this->pixels() );

      return pixels[ ( y * this->pitch()/4 ) + x ];
      break;
    }
  } // end switch
}

bool Texture::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  // Current texture access state; we *MUST* have access to its pixels, so only
  // SDL_TEXTUREACCESS_STREAMING types will work for us.
  enum Texture::Access type = this->access();

  if ( type != Texture::Access::Streaming )
  {
    NOM_LOG_ERR ( NOM, "This texture was not created with Texture::Access::Streaming." );
    return false;
  }

  Point2i source_size = Point2i ( this->width(), this->height() );
  Point2i destination_size;

  // Tru to ensure that our existing texture surface is OK first.
  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The existing video surface is not valid." );
    return false;
  }

  // Temporary memory buffer (pixels surface) that we use to rescale this
  // Texture to
  Image destination;

  // Pick out the suitable scaling factor for determining the new video surface
  // width and height.
  int scale = scale_factor ( scaling_algorithm );

  destination_size = Point2i  ( source_size.x * scale, source_size.y * scale );

  // Create a memory buffer twice our size, using our existing pixel format
  destination.create ( destination_size, this->pixel_format() );

  if ( destination.valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Lock our texture so we can access its pixel buffer
  if ( this->lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock video surface memory for reading." );
    return false;
  }

  // Lock our destination memory for copying our texture to
  if ( destination.lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock destination video surface for writing." );
    return false;
  }

  switch ( scaling_algorithm )
  {
    default: // No rescaling algorithm has been set -- aborting...
    {
      NOM_LOG_ERR ( NOM, "No rescaling algorithm has been set." );
      this->unlock();
      destination.unlock();
      return false;
    }

    case ResizeAlgorithm::scale2x:
    {
      if ( priv::scale2x  (
                            this->pixels(),
                            destination.pixels(),
                            source_size.x,
                            source_size.y,
                            this->bits_per_pixel(),
                            this->pitch(),
                            destination.pitch()
                          ) == false )
      {
        NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale2x." );
        this->unlock(); // Relinquish our read lock
        destination.unlock();
        return false;
      }
      break;
    }

    case ResizeAlgorithm::scale3x:
    {
      if ( priv::scale3x  (
                            this->pixels(),
                            destination.pixels(),
                            source_size.x,
                            source_size.y,
                            this->bits_per_pixel(),
                            this->pitch(),
                            destination.pitch()
                          ) == false )
      {
        NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale3x." );
        this->unlock(); // Relinquish our read lock
        destination.unlock();
        return false;
      }
      break;
    }

    case ResizeAlgorithm::scale4x:
    {
      if ( priv::scale2x  (
                            this->pixels(),
                            destination.pixels(),
                            source_size.x,
                            source_size.y,
                            this->bits_per_pixel(),
                            this->pitch(),
                            destination.pitch()
                          ) == false )
      {
        NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale4x." );
        this->unlock(); // Relinquish our read lock
        destination.unlock();
        return false;
      }
      break;
    }

    case ResizeAlgorithm::hq2x:
    {
      priv::hqxInit();
      priv::hq2x_32 (
                      static_cast<uint32*> ( this->pixels() ),
                      static_cast<uint32*> ( destination.pixels() ),
                      source_size.x,
                      source_size.y
                    );
      break;
    }

    case ResizeAlgorithm::hq3x:
    {
      priv::hqxInit();
      priv::hq3x_32 (
                      static_cast<uint32*> ( this->pixels() ),
                      static_cast<uint32*> ( destination.pixels() ),
                      source_size.x,
                      source_size.y
                    );
      break;
    }

    case ResizeAlgorithm::hq4x:
    {
      priv::hqxInit();
      priv::hq4x_32 (
                      static_cast<uint32*> ( this->pixels() ),
                      static_cast<uint32*> ( destination.pixels() ),
                      source_size.x,
                      source_size.y
                    );
      break;
    }
  } // end switch scaling_algorithm

  // Unlock destination memory buffer; we are done copying from the texture!
  destination.unlock();
  this->unlock();

  // Do one more sanity check on our new video surface before do the final
  // transfer
  if ( destination.valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The rescaled video surface is not valid." );
    return false;
  }

  // Dump rescaled surface memory as a PNG
  #if defined (NOM_DEBUG_SDL2_RESIZE_PNG)
    destination.save_png("rescaled_surface.png");
  #endif

  // Re-initialize our texture with the rescaled pixels from our temporary
  // software surface. There's no turning back once this is done... we will
  // need to reload the image file back into the Texture and start this all
  // over again.
  this->create ( destination ); // STATIC access type

  return true;
}

int Texture::scale_factor ( enum ResizeAlgorithm scaling_algorithm ) const
{
  switch ( scaling_algorithm )
  {
    default: // No rescaling algorithm set
      return 1;
    break;

    case ResizeAlgorithm::scale2x:
    case ResizeAlgorithm::hq2x:
      return 2;
    break;

    case ResizeAlgorithm::scale3x:
    case ResizeAlgorithm::hq3x:
      return 3;
    break;

    case ResizeAlgorithm::scale4x:
    case ResizeAlgorithm::hq4x:
      return 4;
    break;
  }
}

bool Texture::set_blend_mode ( const SDL_BlendMode blend )
{
  if ( SDL_SetTextureBlendMode ( this->texture(), blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Texture::set_colorkey ( const Color4i& colorkey )
{
  this->lock ( this->bounds() ); // Safe for writing

  uint32* pixels = static_cast<uint32*> ( this->pixels() );

  uint32 key = RGB ( colorkey, this->pixel_format() );
  uint32 transparent = RGBA ( colorkey, this->pixel_format() );

  for ( auto idx = 0; idx < ( this->pitch() / 4 ) * this->height(); ++idx )
  {
    if ( pixels[idx] == key ) pixels[idx] = transparent;
  }

  // Once we unlock the texture, it will be uploaded to the GPU
  this->unlock();

  this->colorkey_ = colorkey; // Cache the state of our color key used

  return true;
}

bool Texture::set_color_modulation ( const Color4i& color )
{
  if ( SDL_SetTextureColorMod ( this->texture(), color.r, color.g, color.b ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Texture::copy_pixels ( const void* source, int pitch )
{
  // Did we forget to lock the Texture first???
  if ( this->pixels() == nullptr )
  {
    NOM_LOG_ERR(NOM, "Could not copy the Texture's pixels: buffer was nullptr." );
    return false;
  }

  // Nothing to copy from
  if ( source == nullptr )
  {
    NOM_LOG_ERR(NOM, "Could not copy the source's pixels: buffer was nullptr." );
    return false;
  }

  // Copy pixels from our source buffer to our Texture's pixel buffer
  std::memcpy ( this->pixels(), source, pitch );

  return true;
}

bool Texture::set_render_target ( void )
{
  RendererInfo caps = Window::caps( Window::context() );

  // Graphics hardware does not support render to texture
  if ( caps.target_texture() == false )
  {
    NOM_LOG_ERR ( NOM, "Video hardware does not support render to texture" );
    return false;
  }

  // Try to honor the request; render to the source texture
  if ( SDL_SetRenderTarget ( Window::context(), this->texture() ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

} // namespace nom
