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
#include "nomlib/graphics/Image.hpp"

namespace nom {

Image::Image ( void ) : image_ ( nullptr, priv::FreeSurface )
{
NOM_LOG_TRACE ( NOM );

  if ( IMG_Init ( IMG_INIT_PNG ) != IMG_INIT_PNG )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

Image::~Image ( void )
{
NOM_LOG_TRACE ( NOM );
}

Image::Image ( uint32 flags ) : image_ ( nullptr, priv::FreeSurface )
{
NOM_LOG_TRACE ( NOM );

  if ( IMG_Init ( flags ) != flags )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
  }

  atexit ( IMG_Quit );
}

Image::Image ( const Image& other )  : image_ { other.image(), priv::FreeSurface }
{
NOM_LOG_TRACE ( NOM );
}

Image& Image::operator = ( const Image& other )
{
  this->image_ = other.image_;

  return *this;
}

bool Image::initialize( void* pixels, int32 width, int32 height,
                        int32 bits_per_pixel, uint16 pitch,
                        uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask
                      )
{
NOM_LOG_TRACE ( NOM );

  this->image_.reset ( SDL_CreateRGBSurfaceFrom ( pixels, width, height, bits_per_pixel, pitch, Rmask, Gmask, Bmask, Amask ), priv::FreeSurface );
  //this->set_bounds ( Coords ( 0, 0, width, height ) );

  if ( this->valid() == false )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Image::initialize ( int32 width, int32 height, uint8 bits_per_pixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask, bool colorkey )
{
NOM_LOG_TRACE ( NOM );

  this->image_.reset ( SDL_CreateRGBSurface ( 0, width, height, bits_per_pixel, Rmask, Gmask, Bmask, Amask ), priv::FreeSurface );
  //this->set_bounds ( Coords ( 0, 0, width, height ) );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  // If the video surface is marked for color keying transparency, we need to
  // set it up here and now!
  if ( colorkey )
  {
    if ( this->set_colorkey ( this->colorkey(), true ) == false )
    {
      NOM_LOG_ERR ( NOM, "Could not create the video surface with color key transparency." );
      return false;
    }
  }

  return true;
}

SDL_SURFACE::RawPtr Image::image ( void ) const
{
  return this->image_.get();
}

bool Image::valid ( void ) const
{
  if ( this->image() != nullptr ) return true;

  return false;
}

int32 Image::width ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->w;
}

int32 Image::height ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->h;
}

void* Image::pixels ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->pixels;
}

uint16 Image::pitch ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->pitch;
}

uint8 Image::bits_per_pixel ( void ) const
{
  SDL_Surface* buffer = this->image();

  return buffer->format->BitsPerPixel;
}

const SDL_PIXELFORMAT::RawPtr Image::pixel_format ( void ) const
{
  SDL_Surface* buffer = this->image();
  return buffer->format;
}

const uint32 Image::red_mask ( void ) const
{
  return this->pixel_format()->Rmask;
}

const uint32 Image::green_mask ( void ) const
{
  return this->pixel_format()->Gmask;
}

const uint32 Image::blue_mask ( void ) const
{
  return this->pixel_format()->Bmask;
}

const uint32 Image::alpha_mask ( void ) const
{
  return this->pixel_format()->Amask;
}

const Coords Image::bounds ( void ) const
{
  SDL_Rect clip_buffer; // temporary storage struct
  Coords clip_bounds; // transferred values from SDL_Rect clip_buffer

  // Return values are put into the clip_buffer SDL_Rect after executing:
  SDL_GetClipRect ( this->image(), &clip_buffer );

  // Now transfer the values into our preferred data container type
  clip_bounds = Coords ( clip_buffer.x, clip_buffer.y, clip_buffer.w, clip_buffer.h );

  return clip_bounds;
}

bool Image::must_lock ( void ) const
{
  if ( SDL_MUSTLOCK ( this->image() ) ) return true;

  return false;
}

bool Image::locked ( void ) const
{
  return this->image()->locked;
}

const uint8 Image::alpha ( void ) const
{
  uint8 alpha;

  if ( SDL_GetSurfaceAlphaMod ( this->image(), &alpha ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return 0;
  }

  return alpha;
}

void Image::set_bounds ( const Coords& clip_bounds )
{
  SDL_Rect clip = SDL_RECT ( clip_bounds );

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( this->image(), &clip );
}

bool Image::load ( const std::string& filename )
{
  SDL_Surface *buffer = IMG_Load ( filename.c_str() );

  if ( buffer == nullptr )
  {
NOM_LOG_ERR ( NOM, IMG_GetError() );
    priv::FreeSurface ( buffer );
    return false;
  }

  this->image_.reset ( SDL_ConvertSurfaceFormat ( buffer, SDL_PIXELFORMAT_ARGB8888, 0 ), priv::FreeSurface );
  priv::FreeSurface ( buffer );

  return true;
}

bool Image::load_bmp ( const std::string& filename )
{
  SDL_Surface *buffer = IMG_Load ( filename.c_str() );
  if ( buffer == nullptr )
  {
    NOM_LOG_ERR ( NOM, IMG_GetError() );
    priv::FreeSurface ( buffer );
    return false;
  }

  this->image_.reset ( SDL_ConvertSurfaceFormat ( buffer, SDL_PIXELFORMAT_ARGB8888, 0 ), priv::FreeSurface );
  priv::FreeSurface ( buffer );

  return true;
}

bool Image::save ( const std::string& filename, SDL_SURFACE::RawPtr video_buffer ) const
{
  if ( SDL_SaveBMP ( video_buffer, filename.c_str() ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Image::save ( const std::string& filename ) const
{
  if ( this->save( filename, this->image() ) == false ) return false;

  // Success!
  return true;
}

bool Image::save_png ( const std::string& filename ) const
{
  // Undocumented method for writing out a PNG file with SDL2_image
  if ( IMG_SavePNG ( this->image(), filename.c_str() ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

const Point2i Image::size ( void ) const
{
  SDL_Surface* buffer = this->image();
  Point2i image_pos ( buffer->w, buffer->h );

  return image_pos;
}

const Color4u Image::colorkey ( void ) const
{
  SDL_Surface* buffer = this->image();
  uint32 transparent_color = 0;
  Color4u key;

  if ( SDL_GetColorKey ( this->image(), &transparent_color ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    priv::FreeSurface ( buffer );
    return NOM_COLOR4U_BLACK;
  }

  return nom::pixel ( transparent_color, buffer->format ); // SDL_helper function
}

const SDL_BlendMode Image::blend_mode ( void ) const
{
  SDL_BlendMode blend;

  if ( SDL_GetSurfaceBlendMode ( this->image(), &blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return blend;
  }

  return blend;
}

bool Image::set_colorkey ( const Color4u& colorkey, bool flag )
{
  SDL_Surface* buffer = this->image();
  uint32 transparent_color = RGB ( colorkey, buffer->format );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not set color key: invalid image buffer." );
    priv::FreeSurface ( buffer );
    return false;
  }

  if ( SDL_SetColorKey ( buffer, SDL_BOOL(flag), transparent_color ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    priv::FreeSurface ( buffer );
    return false;
  }

  return true;
}

bool Image::RLE ( bool flag )
{
  if ( SDL_SetSurfaceRLE ( this->image(), SDL_BOOL(flag) ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

uint32 Image::pixel ( int32 x, int32 y )
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

bool Image::set_blend_mode ( const SDL_BlendMode blend )
{
  if ( SDL_SetSurfaceBlendMode ( this->image(), blend ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }

  return true;
}

bool Image::lock ( void ) const
{
  if ( SDL_BOOL ( this->must_lock() ) )
  {
    if ( SDL_LockSurface ( this->image() ) != 0 )
    {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
      return false;
    }
  }

  return true;
}

void Image::unlock ( void ) const
{
  SDL_UnlockSurface ( this->image() );
}


} // namespace nom
