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
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/scale2x/scale2x.hpp"
#include "nomlib/graphics/hqx/hqx.hpp"

namespace nom {

Texture::Texture ( void )  :  texture_ { nullptr, priv::FreeTexture },
    pixels_ ( nullptr ), pitch_ ( 0 ), position_ ( 0, 0 ),
    bounds_ ( 0, 0, -1, -1 ), colorkey_ { NOM_COLOR4U_BLACK }
{
NOM_LOG_TRACE ( NOM );
}

Texture::~Texture ( void )
{
NOM_LOG_TRACE ( NOM );
  delete this->pixels_;
}

Texture::Texture ( const Texture& other ) :
    texture_ { other.texture(), priv::FreeTexture },
    pixels_ { other.pixels() }, pitch_ { other.pitch() },
    position_ { other.position() }, bounds_ { other.bounds() },
    colorkey_ { other.colorkey() }
{
NOM_LOG_TRACE ( NOM );
}

bool Texture::initialize ( SDL_SURFACE::RawPtr video_buffer )
{
  // We leak memory, or even worse, crash, depending on state at the time, if we
  // do not ensure that we free existing buffers! Certain rendering methods --
  // *cough* nom::TrueTypeFont -- need this done because they create new
  // surfaces (instead of updating what they have) upon every update cycle.
  if ( this->valid() ) this->texture_.reset();

  this->texture_.reset ( SDL_CreateTextureFromSurface ( Window::context(), video_buffer ), priv::FreeTexture );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  // TODO: cache width & height

  // Cache the size of our new Texture object with the existing surface info
  this->set_bounds ( Coords ( 0, 0, video_buffer->w, video_buffer->h ) );

  // FIXME: See GitHub Issue #8
  //
  // By commenting this line out, we are creating a massive memory leak in
  // TrueTypeFont
  //priv::FreeSurface ( video_buffer );

  return true;
}

bool Texture::initialize ( int32 width, int32 height, uint32 format, uint32 flags )
{
  //if ( this->valid() ) this->texture_.reset();

  this->texture_.reset ( SDL_CreateTexture ( Window::context(), format, flags, width, height ), priv::FreeTexture );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  // TODO: cache width & height

  // Cache the size of our new Texture object with the existing surface info
  this->set_bounds( Coords ( 0, 0, width, height ) );

  return true;
}

Texture::SharedPtr Texture::clone ( void ) const
{
  return Texture::SharedPtr ( new Texture ( *this ) );
}

Texture& Texture::operator = ( const Texture& other )
{
  this->texture_ = other.texture_;
  this->pixels_ = other.pixels_;
  this->pitch_ = other.pitch_;
  this->position_ = other.position_;
  this->bounds_ = other.bounds_;

  return *this;
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

int Texture::access ( void ) const
{
  int type;

  if ( SDL_QueryTexture ( this->texture(), nullptr, &type, nullptr, nullptr ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return type;
  }

  return type;
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

bool Texture::locked ( void ) const
{
  if ( this->pixels() != nullptr ) return true;

  return false;
}

const Color4u& Texture::colorkey ( void ) const
{
  return this->colorkey_;
}

const uint8 Texture::alpha ( void ) const
{
  uint8 alpha;

  if ( SDL_GetTextureAlphaMod ( this->texture(), &alpha ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return nom::ALPHA_OPAQUE;
  }

  return alpha;
}

const Color4u Texture::color_modulation ( void ) const
{
  Color4u color;

  if ( SDL_GetTextureColorMod ( this->texture(), &color.r, &color.g, &color.b ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return NOM_COLOR4U_WHITE;
  }

  return color;
}

bool Texture::lock ( void )
{
  if ( this->locked() )
  {
NOM_LOG_ERR ( NOM, "Texture is already locked." );
    return false;
  }

  if ( SDL_LockTexture ( this->texture(), nullptr, &this->pixels_, &this->pitch_ ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Texture::lock ( const Coords& lock_coords )
{
  SDL_Rect area = SDL_RECT(lock_coords);

  if ( this->locked() )
  {
NOM_LOG_ERR ( NOM, "Texture is already locked." );
    return false;
  }

  if ( SDL_LockTexture ( this->texture(), &area, &this->pixels_, &this->pitch_ ) != 0 )
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

bool Texture::load  (
                      const std::string& filename, uint32 flags,
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

    this->texture_ = image.load ( filename );
  }
TODO */

  if ( image.load ( filename ) == false ) return false;

  // We produce a segmentation fault here if we do not have SDL's video
  // subsystem initialized before making the following calls -- transparency
  // and display format conversion.
NOM_ASSERT ( SDL_WasInit ( SDL_INIT_VIDEO) );

  if ( this->initialize ( image.width(), image.height(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING ) == false )
  {
NOM_LOG_ERR ( NOM, "Error: Failed to initialize texture." );
    return false;
  }

  // Set our default blending mode for texture copies
  this->set_blend_mode( SDL_BLENDMODE_BLEND );

  this->lock ( image.bounds() ); // Safe for writing

  // Copy pixels from image into our freshly initialized texture
  memcpy ( this->pixels(), image.pixels(), image.pitch() * image.height() );

  // Once we unlock the texture, it will be uploaded to the GPU for us!
  this->unlock();

  // Update our Texture clipping bounds with the new source
  this->set_bounds ( image.bounds() );

  return true;
}

bool Texture::update ( const void* pixels, uint16 pitch, const Coords& update_area )
{
  if ( SDL_UpdateTexture ( this->texture(), nullptr, pixels, pitch ) != 0 )
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
NOM_ASSERT ( ! ( opacity > nom::ALPHA_OPAQUE ) || ( opacity < nom::ALPHA_TRANSPARENT ) );

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
  int bpp = 0; // bits per pixel
  uint32 red_mask = 0;
  uint32 green_mask = 0;
  uint32 blue_mask = 0;
  uint32 alpha_mask = 0;

  // Ensure that our existing video surface is OK first
  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The existing video surface is not valid." );
    return false;
  }

  if ( SDL_BOOL( SDL_PixelFormatEnumToMasks ( this->pixel_format(), &bpp, &red_mask, &green_mask, &blue_mask, &alpha_mask ) ) != true )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
    return false;
  }

  // Current video surface flags state -- the destination buffer will be set
  // with these.
  //uint32 flags = this->getTextureFlags();

  // This is the target video surface object that is created from the existing
  // video surface, with the existing width & height recomputed to whatever the
  // chosen algorithm expects. The target buffer (upon success) becomes the new
  // video surface of this instance.
  //Texture destination_buffer;
  Image destination_buffer;

  // Pick out the suitable scaling factor for determining the new video surface
  // width and height.
  int32 scale_factor = getResizeScaleFactor ( scaling_algorithm );

  // We must not set an alpha mask value if our existing video surface is color
  // keyed (or bad things ensue -- like many hours spent reading up on this
  // surprisingly confusing subject).
  //uint8 alpha_mask = 0; // no alpha mask is default (works with color keying)

  // If the video surface does *NOT* have color keying set
  //if ( ! ( flags & SDL_TRUE ) )//SDL_SRCCOLORKEY ) )
  //{
  //alpha_mask = this->alpha();
  //}

  destination_buffer.initialize (
                                  this->width() * scale_factor,
                                  this->height() * scale_factor,
                                  bpp, // bits per pixel
                                  red_mask,
                                  green_mask,
                                  blue_mask,
                                  alpha_mask,
                                  true // color key enabled
                                );

#if defined (NOM_DEBUG_SDL2_RESIZE)
  NOM_DUMP_VAR((int)this->alpha());
  NOM_DUMP_VAR(this->width());
  NOM_DUMP_VAR(this->height());
  NOM_DUMP_VAR(scale_factor);
  NOM_DUMP_VAR(bpp);
  NOM_DUMP_VAR(red_mask);
  NOM_DUMP_VAR(green_mask);
  NOM_DUMP_VAR(blue_mask);
  NOM_DUMP_VAR(alpha_mask);
  NOM_DUMP_VAR(PIXEL_FORMAT_NAME(this->pixel_format()));

  NOM_DUMP_VAR((int)destination_buffer.alpha());
  NOM_DUMP_VAR(destination_buffer.pitch());
#endif

  // Ensure that our new video surface is sane before feeding
  if ( destination_buffer.valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The destination video surface is not valid." );
    return false;
  }

  // Lock pixels buffer so we can obtain access to its pixels & pitch
  if ( this->lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
    return false;
  }

  // Lock destination memory for writing to
  if ( destination_buffer.lock() == false )
  {
    NOM_LOG_ERR ( NOM, "Could not lock destination video surface." );
    return false;
  }

  // Dump pixels from existing texture onto the destination texture
  memcpy  ( this->pixels(), destination_buffer.pixels(),
            destination_buffer.pitch() / destination_buffer.height()
          );

  switch ( scaling_algorithm )
  {
    default: // Error: insufficient input; no resizing is applied.
    {
      NOM_LOG_ERR ( NOM, "Invalid resizing algorithm specified." );
      this->unlock();
      destination_buffer.unlock();
      return false;
    }

    case ResizeAlgorithm::scale2x:
    {
      if ( priv::scale2x  (
                            this->pixels(), // source
                            destination_buffer.pixels(), // destination
                            this->width(), // source
                            this->height(), // source
                            this->bits_per_pixel(), // source
                            this->pitch(), // source
                            destination_buffer.pitch() // destination
                          ) == false )
      {
        NOM_LOG_ERR ( NOM, "Failed to resize video surface with scale2x." );
        this->unlock(); // Relinquish our write lock
        return false;
      }
      break;
    }
/*
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
*/
  } // end switch scaling_algorithm

  // Once we unlock the texture, it will be uploaded to the GPU for us!
  destination_buffer.unlock();
  this->unlock();

  // Do one more sanity check on our new video surface before do the transfer
  if ( destination_buffer.valid() == false )
  {
    NOM_LOG_ERR ( NOM, "The rescaled video surface is not valid." );
    return false;
  }

  // This appears fine -- something past this is where everything goes south...
  destination_buffer.save_png("testme.png");

  if ( this->initialize ( destination_buffer.width(), destination_buffer.height(), SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING ) == false )
  {
    NOM_LOG_ERR ( NOM, "The rescaled video surface is not valid." );
    return false;
  }

  // Lock pixels buffer so we can obtain access to its pixels & pitch
  this->lock ( destination_buffer.bounds() );

  // Copy pixels from image into our freshly initialized texture
  memcpy ( destination_buffer.pixels(), this->pixels(), destination_buffer.pitch() );

  // Once we unlock the texture, it will be uploaded to the GPU for us!
  this->unlock();

  //this->set_colorkey ( Color4u(0,0,0,0) );

  this->set_bounds ( destination_buffer.bounds() );

  // Reset the video surface object's video memory to the rescaled pixel data.
  //this->setTexture ( destination_buffer );

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

bool Texture::set_blend_mode ( const SDL_BlendMode blend )
{
  if ( SDL_SetTextureBlendMode ( this->texture(), blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Texture::set_colorkey ( const Color4u& colorkey )
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

bool Texture::set_color_modulation ( const Color4u& color )
{
  if ( SDL_SetTextureColorMod ( this->texture(), color.r, color.g, color.b ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

} // namespace nom
