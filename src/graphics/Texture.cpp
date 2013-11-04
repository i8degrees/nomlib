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

    scale2x Algorithm
1. Copyright (c) Andrea Mazzoleni

******************************************************************************/
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/scale2x/scale2x.hpp"
#include "nomlib/graphics/hqx/hqx.hpp"

namespace nom {

Texture::Texture ( void )  :  texture_ ( nullptr, priv::FreeTexture ),
    position_ ( 0, 0 ), bounds_ ( 0, 0, -1, -1 )
{
NOM_LOG_TRACE ( NOM );
}

Texture::~Texture ( void )
{
NOM_LOG_TRACE ( NOM );
}

Texture::Texture ( SDL_Surface* video_buffer )
{
NOM_LOG_TRACE ( NOM );

  if ( this->initialize ( video_buffer ) == false )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
  }
}

/* TODO
Texture::Texture ( SDL_Texture* video_buffer )  :
    texture_ { video_buffer, priv::FreeTexture }
{
NOM_LOG_TRACE ( NOM );

  Point2i size;

  SDL_QueryTexture ( this->texture(), nullptr, nullptr, &size.x, &size.y );

  this->bounds_.setSize ( size.x, size.y );
}
TODO */

Texture::Texture ( const Texture& other ) :
    texture_ { other.texture(), nom::priv::FreeTexture },
    position_ ( other.position_.x, other.position_.y ),
    bounds_ ( other.bounds_.width, other.bounds_.height )
{
NOM_LOG_TRACE ( NOM );
}

Texture::Texture ( int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask, uint32 flags )
{
NOM_LOG_TRACE ( NOM );

  this->initialize ( flags, width, height, bitsPerPixel, Rmask, Gmask, Bmask, Amask );
}

Texture::Texture ( void* pixels, int32 width, int32 height, int32 depth, uint16 pitch, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask )
{
NOM_LOG_TRACE ( NOM );

  //this->texture_.reset ( SDL_CreateRGBSurfaceFrom ( pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask ), nom::priv::FreeSurface );

  this->bounds_.setSize ( width, height );
}

void Texture::initialize ( uint32 flags, int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask )
{
NOM_LOG_TRACE ( NOM );

  //this->texture_.reset ( SDL_CreateRGBSurface ( flags, width, height, bitsPerPixel, Rmask, Gmask, Bmask, Amask ), nom::priv::FreeSurface );
  this->bounds_.setSize ( width, height );

  // If the video surface is marked for color keying transparency, we must do
  // so here.
  /*if ( flags & SDL_TRUE )//SDL_SRCCOLORKEY )
  {
    if ( this->setTransparent ( this->getTextureColorKey(), SDL_RLEACCEL | SDL_TRUE ) == false )//SDL_SRCCOLORKEY ) == false )
    {
NOM_LOG_ERR ( NOM, "Could not create the video surface with color key transparency." );
    }
  }*/
}

bool Texture::initialize ( SDL_Surface* video_buffer )
{
  this->texture_.reset ( SDL_CreateTextureFromSurface ( Window::context(), video_buffer ), priv::FreeTexture );

  if ( this->valid() == false ) return false;

  // Cache the size of our new Texture object with the existing surface info
  this->bounds_.setSize ( video_buffer->w, video_buffer->h );

  return true;
}

Texture::SharedPtr Texture::clone ( void ) const
{
  return Texture::SharedPtr ( new Texture ( *this ) );
}

Texture& Texture::operator = ( const Texture& other )
{
  this->texture_ = other.texture_;
  this->position_ = other.position_;
  this->bounds_ = other.bounds_;

  return *this;
}

SDL_Texture* Texture::texture ( void ) const
{
  return this->texture_.get();
}

bool Texture::valid ( void ) const
{
  if ( this->texture() != nullptr ) return true;

  return false;
}

/* RELOCATE
void Texture::setTexture ( const Texture& surface )
{
  //this->texture_.reset ( SDL_ConvertSurface ( surface.texture_.get(), surface.getTexturePixelsFormat(), surface.getTextureFlags() ), nom::priv::FreeSurface );

  //this->offsets.setSize ( surface.width(), surface.height() );
}
RELOCATE */

const Point2i& Texture::position ( void ) const
{
  return this->position_;
}

const Coords& Texture::bounds ( void ) const
{
  return this->bounds_;
}

void Texture::set_position ( const Point2i& pos )
{
  this->position_.x = pos.x;
  this->position_.y = pos.y;
}

void Texture::set_bounds ( const Coords& clip )
{
  this->bounds_ = clip;
}

int32 Texture::width ( void ) const
{
  int32 tex_width;

  SDL_QueryTexture ( this->texture(), nullptr, nullptr, &tex_width, nullptr );

  return tex_width;
}

int32 Texture::height ( void ) const
{
  int32 tex_height;

  SDL_QueryTexture ( this->texture(), nullptr, nullptr, nullptr, &tex_height );

  return tex_height;
}

uint16 Texture::pitch ( void ) const
{
  uint16 pitch;

  // Pixel pitch calculation borrowed from SDL2/video/SDL_pixels.c
  pitch = this->width() * this->bytes_per_pixel();
  switch ( this->bits_per_pixel() )
  {
    default: break;

    case 1: // 8-bit bpp
    {
      pitch = ( pitch + 7 ) / 8;
    }
    break;

    case 4: // 32-bit bpp
    {
      pitch = ( pitch + 1 ) / 2;
    }
    break;
  }
  pitch = ( pitch + 3 ) & ~3; // 4-byte aligned for speed

  return pitch;
}

void* Texture::pixels ( void ) const
{
  return this->pixels_;
}

uint8 Texture::bits_per_pixel ( void ) const
{
  switch ( this->bytes_per_pixel() )
  {
    default: return 0; break; // Unknown color depth

    case 1: return 8; break; // 8-bit bpp
    case 2: return 16; break; // 16-bit bpp
    case 3: return 24; break; // 24-bit bpp
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
/*
const uint8 Texture::getTextureAlphaValue ( void ) const
{
  return 0;
  //return this->getTexturePixelsFormat()->alpha;
}
*/
/*
const uint32 Texture::getTextureRedMask ( void ) const
{
  return 0;
  //return this->getTexturePixelsFormat()->Rmask;
}
*/
/*
const uint32 Texture::getTextureGreenMask ( void ) const
{
  return 0;
  //return this->getTexturePixelsFormat()->Gmask;
}
*/
/*
const uint32 Texture::getTextureBlueMask ( void ) const
{
  return 0;
  //return this->getTexturePixelsFormat()->Bmask;
}
*/
/*
const uint32 Texture::getTextureAlphaMask ( void ) const
{
  return 0;
  //return this->getTexturePixelsFormat()->Amask;
}
*/

/* RELOCATE
void Texture::setTextureBounds ( const Coords& clip_bounds )
{
  SDL_Rect clip = IntRect::asSDLRect ( clip_bounds ); // temporary storage struct for setting

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( this->texture_.get(), &clip );
}
RELOCATE */

/* TODO
bool Texture::getTextureLock ( void ) const
{
  return false;
  //return this->texture_.get()->locked;
}
TODO */

bool Texture::lock ( void )
{
  if ( SDL_LockTexture ( this->texture(), nullptr, &this->pixels_, this->pitch_ ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

bool Texture::lock ( const Coords& lock_coords )
{
  SDL_Rect area = SDL_RECT(lock_coords);

  if ( SDL_LockTexture ( this->texture(), &area, &this->pixels_, this->pitch_ ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

void Texture::unlock ( void ) const
{
  SDL_UnlockTexture ( this->texture() );
}

bool Texture::load ( const std::string& filename, const Color& colorkey,
                    bool use_cache
                  )
{
  Image image;
/* TODO
  // By default -- for peace of mind above all else -- we have caching turned
  // off
  if ( use_cache )
  {
    priv::ObjectCache cache;

    this->texture_ = cache.getObject ( filename );

    if ( this->texture_ == nullptr )
    {
      this->texture_ = cache.addObject ( filename, image.load ( filename ) );
    }
  }
  else // Do not use the object cache
  {
TODO */
    //this->texture_ = image.load ( filename );
  //}

  if ( image.load ( filename ) == false ) return false;

  if ( colorkey != Color::null )
  {
    if ( image.set_colorkey ( colorkey, true ) == false )
    {
      return false;
    }
  }

  // We produce a segmentation fault here if we do not have SDL's video
  // subsystem initialized before making the following calls -- transparency
  // and display format conversion.
NOM_ASSERT ( SDL_WasInit ( SDL_INIT_VIDEO) );

  if ( this->initialize ( image.image() ) == false ) return false;

  /*if ( flags & SDL_SRCALPHA )
  {
    this->displayFormatAlpha(); // Optimized video surface with an alpha channel
  }
  else
  {*/
    //this->displayFormat(); // Optimized video surface without an alpha channel
  //}

  // Update our Texture clipping bounds with the new source; not sure if we still
  // need to be doing this.
  this->bounds_.setSize ( this->width(), this->height() );

  return true;
}

void Texture::draw ( SDL_Renderer* target ) const
{
  Point2i pos = this->position();
  SDL_Rect render_coords;

  render_coords.x = pos.x;
  render_coords.y = pos.y;

  // Use set clipping bounds for the width and height of this texture
  if ( this->bounds_.w != -1 && this->bounds_.h != -1 )
  {
    render_coords.w = this->bounds_.w;
    render_coords.h = this->bounds_.h;
  }
  else // Use Texture's known total width and height
  {
    render_coords.w = this->width();
    render_coords.h = this->height();
  }

  // Render with set clipping bounds; we are rendering only a portion of a
  // larger Texture, like how it is done with sprite sheets.
  if ( this->bounds_.w != -1 && this->bounds_.h != -1 )
  {
    SDL_Rect render_bounds;
    render_bounds.x = this->bounds_.x;
    render_bounds.y = this->bounds_.y;
    render_bounds.w = this->bounds_.w;
    render_bounds.h = this->bounds_.h;
    if ( SDL_RenderCopy ( target, this->texture(), &render_bounds, &render_coords ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    }
  }
  else // Render the entire Texture we have in memory
  {
    if ( SDL_RenderCopy ( target, this->texture(), nullptr, &render_coords ) != 0 )
    {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    }
  }

  //NOM_DUMP_VAR(this->position()); NOM_DUMP_VAR(this->width()); NOM_DUMP_VAR(this->height()); NOM_DUMP_VAR(this->bounds_.w); NOM_DUMP_VAR(this->bounds_.h);
}
/*
void Texture::draw ( SDL_Renderer* target ) const
{
  // temporary vars to store our wrapped Coords
  SDL_Rect blit_coords = IntRect::asSDLRect ( this->coords );
  SDL_Rect blit_offsets = IntRect::asSDLRect ( this->offsets );

  // Perhaps also check to see if video_buffer is nullptr?
  if ( this->valid() )
  {
    if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
    {
      if ( SDL_BlitSurface ( this->texture_.get(), &blit_offsets, video_buffer, &blit_coords ) != 0 )
NOM_LOG_ERR ( NOM, SDL_GetError() );
        return;
    }
    else
    {
      if ( SDL_BlitSurface ( this->texture_.get(), nullptr, (SDL_SDL_Surface*) video_buffer, &blit_coords ) != 0 )
NOM_LOG_ERR ( NOM, SDL_GetError() );
        return;
    }
  }
}
*/

bool Texture::update ( const void* pixels, uint16 pitch, const Coords& update_area )
{
  return false; // Stub
}

void Texture::draw ( const Window& target ) const
{
  if ( SDL_RenderCopy ( target.renderer(), this->texture(), nullptr, nullptr ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
  }
}

bool Texture::set_alpha ( uint8 opacity )
{
NOM_ASSERT ( ! ( opacity > SDL_ALPHA_OPAQUE ) || ( opacity < SDL_ALPHA_TRANSPARENT ) );

  if ( SDL_SetTextureAlphaMod ( this->texture(), opacity ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

/* RELOCATE
bool Texture::displayFormat ( void )
{
  //this->texture_.reset ( SDL_DisplayFormat ( this->texture_.get() ), nom::priv::FreeSurface );

//NOM_ASSERT ( this->valid() );

  return true;
}
RELOCATE */

/* RELOCATE
bool Texture::displayFormatAlpha ( void )
{
  //this->texture_.reset ( SDL_DisplayFormatAlpha ( this->texture_.get()), nom::priv::FreeSurface );

//NOM_ASSERT ( this->valid() );

  return true;
}
RELOCATE */

uint32 Texture::getPixel ( int32 x, int32 y )
{
  switch ( this->bits_per_pixel() )
  {
    default: return -1; break; // Unknown

    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( this->pixels() );

      return pixels[ ( y * this->pitch() ) + x ];
    }
    break;

    case 16:
    {
      uint16* pixels = static_cast<uint16*> ( this->pixels() );

      return pixels[ ( y * this->pitch() / 2 ) + x ];
    }
    break;

    case 24:
    {
      uint8* pixels = static_cast<uint8*> ( this->pixels() );

      return pixels[ ( y * this->pitch() ) + x ];
    }
    break;

    case 32:
    {
      uint32* pixels = static_cast<uint32*> ( this->pixels() );

      return pixels[ ( y * this->pitch()/4 ) + x ];
    }
    break;
  } // end switch
}

bool Texture::resize ( enum ResizeAlgorithm scaling_algorithm )
{
/*
  // Ensure that our existing video surface is OK first
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "The existing video surface is not valid." );
    return false;
  }

  // Current video surface flags state -- the destination buffer will be set
  // with these.
  uint32 flags = this->getTextureFlags();

  // This is the target video surface object that is created from the existing
  // video surface, with the existing width & height recomputed to whatever the
  // chosen algorithm expects. The target buffer (upon success) becomes the new
  // video surface of this instance.
  Texture destination_buffer;

  // Pick out the suitable scaling factor for determining the new video surface
  // width and height.
  int32 scale_factor = getResizeScaleFactor ( scaling_algorithm );

  // We must not set an alpha mask value if our existing video surface is color
  // keyed (or bad things ensue -- like many hours spent reading up on this
  // surprisingly confusing subject).
  uint32 alpha_mask = 0; // no alpha mask is default (works with color keying)

  // If the video surface does *NOT* have color keying set
  if ( ! ( flags & SDL_TRUE ) )//SDL_SRCCOLORKEY ) )
  {
    alpha_mask = this->getTextureAlphaMask();
  }

  destination_buffer = Texture (
                                this->width() * scale_factor,
                                this->height() * scale_factor,
                                this->getTextureBitsPerPixel(),
                                this->getTextureRedMask(),
                                this->getTextureGreenMask(),
                                this->getTextureBlueMask(),
                                alpha_mask,
                                flags
                              );

  // Ensure that our new video surface is sane before feeding
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Lock pixels buffer for writing to
  if ( this->lock() == false )
  {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
    return false;
  }

  switch ( scaling_algorithm )
  {
    default: this->unlock(); return false; break; // No resizing is applied

    case ResizeAlgorithm::scale2x:
    {
      if ( priv::scale2x  (
                            this->getTexturePixels(),
                            destination_buffer.getTexturePixels(),
                            this->width(),
                            this->height(),
                            this->getTextureColorDepth(),
                            this->getTexturePitch(),
                            destination_buffer.getTexturePitch()
                          ) == false )
      {
NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale2x." );
        this->unlock(); // Relinquish our write lock
        return false;
      }
    }
    break;

    case ResizeAlgorithm::scale3x:
    {
      if ( priv::scale3x  (
                            this->getTexturePixels(),
                            destination_buffer.getTexturePixels(),
                            this->width(),
                            this->height(),
                            this->getTextureColorDepth(),
                            this->getTexturePitch(),
                            destination_buffer.getTexturePitch()
                          ) == false )
      {
NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale3x." );
        this->unlock(); // Relinquish our write lock
        return false;
      }
    }
    break;

    case ResizeAlgorithm::scale4x:
    {
      if ( priv::scale2x  (
                            this->getTexturePixels(),
                            destination_buffer.getTexturePixels(),
                            this->width(),
                            this->height(),
                            this->getTextureColorDepth(),
                            this->getTexturePitch(),
                            destination_buffer.getTexturePitch()
                          ) == false )
      {
NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale4x." );
        this->unlock(); // Relinquish our write lock
        return false;
      }
    }
    break;

    case ResizeAlgorithm::hq2x:
    {
      priv::hqxInit();
      // Note that we must pass the *source* width and height here
      priv::hq2x_32 (
                      static_cast<uint32*> ( this->getTexturePixels() ),
                      static_cast<uint32*> ( destination_buffer.getTexturePixels() ),
                      this->width(), this->height()
                    );
    }
    break;

    case ResizeAlgorithm::hq3x:
    {
      priv::hqxInit();
      // Note that we must pass the *source* width and height here
      priv::hq3x_32 (
                      static_cast<uint32*> ( this->getTexturePixels() ),
                      static_cast<uint32*> ( destination_buffer.getTexturePixels() ),
                      this->width(), this->height()
                    );
    }
    break;

    case ResizeAlgorithm::hq4x:
    {
      priv::hqxInit();
      // Note that we must pass the *source* width and height here
      priv::hq4x_32 (
                      static_cast<uint32*> ( this->getTexturePixels() ),
                      static_cast<uint32*> ( destination_buffer.getTexturePixels() ),
                      this->width(), this->height()
                    );
    }
    break;
  } // end switch scaling_algorithm

  this->unlock(); // Relinquish our write lock

  // Do one more sanity check on our new video surface before do the transfer
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The rescaled video surface is not valid." );
    return false;
  }

  // Reset the video surface object's video memory to the rescaled pixel data.
  this->setTexture ( destination_buffer );
*/
  return true;
}

bool Texture::resize ( const Point2f& scale_factor )
{
/*
  // set ResizeAlgorithm::Stretch

  // Ensure that our existing video surface is OK first
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "The existing video surface is not valid." );
    return false;
  }

  // Current video surface flags state -- the destination buffer will be set
  // with these.
  uint32 flags = this->getTextureFlags();

  // This is the target video surface object that is created from the existing
  // video surface, with the existing width & height recomputed to whatever the
  // chosen algorithm expects. The target buffer (upon success) becomes the new
  // video surface of this instance.
  Texture destination_buffer;

  // Pick out the suitable scaling factor for determining the new video surface
  // width and height.
  //int32 scale_factor = getResizeScaleFactor ( scaling_algorithm );

  // We must not set an alpha mask value if our existing video surface is color
  // keyed (or bad things ensue -- like many hours spent reading up on this
  // surprisingly confusing subject).
  uint32 alpha_mask = 0; // no alpha mask is default (works with color keying)

  // If the video surface does *NOT* have color keying set
  if ( ! ( flags & SDL_TRUE ) )//SDL_SRCCOLORKEY ) )
  {
    //alpha_mask = this->getTextureAlphaMask();
  }

  destination_buffer = Texture (
                                this->width()*scale_factor.x,
                                this->height()*scale_factor.y,
                                this->getTextureBitsPerPixel(),
                                this->getTextureRedMask(),
                                this->getTextureGreenMask(),
                                this->getTextureBlueMask(),
                                alpha_mask,
                                flags
                              );

  // Ensure that our new video surface is sane before feeding
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Lock pixels buffer for writing to
  if ( this->lock() == false )
  {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
    return false;
  }

  Vector2f r;
  Color color;
  Pixel pixels;

  double stretch_x = (double) this->width() * scale_factor.x / (double) this->width();
  double stretch_y = (double) this->height() * scale_factor.y / (double) this->height();

  for ( int y = 0; y < this->height(); y++ )
  {
    for ( int x = 0; x < this->width(); x++ )
    {
      for ( int sY = 0; sY < stretch_y; sY++ )
      {
        for ( int sX = 0; sX < stretch_x; sX++ )
        {
          uint32 pixel = this->getPixel ( x, y );
          RGBA::asRGB ( pixel, this->getTexturePixelsFormat(), color );
          //NOM_DUMP_VAR(color);
          if ( color.red == 0 && color.green == 0 && color.blue == 0 ) continue;

          pixels = Pixel ( ( stretch_x * x ) + sX, ( stretch_y * y ) + sY, color );
          pixels.Draw ( destination_buffer.get() );
        }
      }
    }
  }

  this->unlock(); // Relinquish our write lock

  // Do one more sanity check on our new video surface before do the transfer
  if ( destination_buffer.valid() == false )
  {
NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Reset the video surface object's video memory to the rescaled pixel data.
  this->setTexture ( destination_buffer );
*/
  return true;
}

int32 Texture::getResizeScaleFactor ( enum ResizeAlgorithm scaling_algorithm )
{
  switch ( scaling_algorithm )
  {
    default: return 1; break;

    case ResizeAlgorithm::hq2x:
    case ResizeAlgorithm::scale2x:
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


} // namespace nom
