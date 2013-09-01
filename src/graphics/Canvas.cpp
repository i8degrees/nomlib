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
#include "nomlib/graphics/Canvas.hpp"
#include "nomlib/graphics/hqx/hqx.hpp"

// C Macros used in scale2x method
#define SCALE2x_READINT24(x) \
  ((x)[0]<<16 | (x)[1]<<8 | (x)[2])

#define SCALE2x_WRITEINT24(x, i) \
  {(x)[0]=i>>16; (x)[1]=(i>>8)&0xff; x[2]=i&0xff; }

namespace nom {
  namespace priv {

void Canvas_FreeSurface ( SDL_Surface* video_buffer )
{
  SDL_FreeSurface ( video_buffer );
}

  } // namespace priv
} // namespace nom

namespace nom {

Canvas::Canvas ( void )  : canvas_buffer ( nullptr, nom::priv::Canvas_FreeSurface ),
                                        coords ( 0, 0, -1, -1 ), // only x, y position is used in blitting
                                        offsets ( 0, 0, -1, -1 ) // only the width, height is used in source blitting
{
NOM_LOG_CLASSINFO;
}

Canvas::Canvas ( void* video_buffer )  : canvas_buffer ( static_cast<SDL_Surface*> ( video_buffer ), nom::priv::Canvas_FreeSurface )
{
NOM_LOG_CLASSINFO;

  SDL_Surface* buffer = static_cast<SDL_Surface*> ( video_buffer );

  this->offsets.setSize ( buffer->w, buffer->h );
}

Canvas::Canvas ( const Canvas& other ) : canvas_buffer ( static_cast<SDL_Surface*> ( other.canvas_buffer.get() ), nom::priv::Canvas_FreeSurface ),
                                                          coords ( other.coords.x, other.coords.y ), offsets ( other.offsets.width, other.offsets.height )
{
NOM_LOG_CLASSINFO;
}

Canvas::Canvas ( int32 width, int32 height, uint8 bitsPerPixel, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask, uint32 flags )
{
NOM_LOG_CLASSINFO;

  this->canvas_buffer = std::shared_ptr<void> ( SDL_CreateRGBSurface ( flags, width, height, bitsPerPixel, Rmask, Gmask, Bmask, Amask ), nom::priv::Canvas_FreeSurface );
  this->offsets.setSize ( width, height );
}

Canvas::Canvas ( Pixels pixels, int32 width, int32 height, int32 depth, uint16 pitch, uint32 Rmask, uint32 Gmask, uint32 Bmask, uint32 Amask )
{
NOM_LOG_CLASSINFO;

  this->canvas_buffer = std::shared_ptr<void> ( SDL_CreateRGBSurfaceFrom ( pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask ), nom::priv::Canvas_FreeSurface );
  this->offsets.setSize ( width, height );
}

Canvas::~Canvas ( void )
{
NOM_LOG_CLASSINFO;
}

Canvas& Canvas::operator = ( const Canvas& other )
{
  this->canvas_buffer = other.canvas_buffer;

  return *this;
}

bool Canvas::valid ( void ) const
{
  if ( this->canvas_buffer.get() != nullptr )
    return true;
  else
    return false;
}

void Canvas::setCanvas ( const Canvas& surface )
{
  this->canvas_buffer.reset ( SDL_ConvertSurface ( static_cast<SDL_Surface*> ( surface.canvas_buffer.get() ), static_cast<SDL_PixelFormat*> ( surface.getCanvasPixelsFormat() ), surface.getCanvasFlags() ), nom::priv::Canvas_FreeSurface );

  this->offsets.setSize ( surface.getCanvasWidth(), surface.getCanvasHeight() );
}

void Canvas::setPosition ( const Coords& coords_ )
{
  this->coords.x = coords_.x;
  this->coords.y = coords_.y;
}

void Canvas::setOffsets ( const Coords& offsets_ )
{
  this->offsets = offsets_;
}

const int32_t Canvas::getCanvasWidth ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->w;
}

const int32_t Canvas::getCanvasHeight ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->h;
}

uint32_t Canvas::getCanvasFlags ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->flags;
}

uint16 Canvas::getCanvasPitch ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->pitch;
}

const Pixels Canvas::getCanvasPixels ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->pixels;
}

const uint8 Canvas::getCanvasBitsPerPixel ( void ) const
{
   SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format->BitsPerPixel;
}

const Pixels Canvas::getCanvasPixelsFormat ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format;
}

const Color Canvas::getCanvasColorKey ( void ) const
{
  uint32 transparent_color = 0; // holds me color for conversion
  Color colorkey; // native container

  transparent_color = getColorAsInt ( this->getCanvasPixelsFormat(), colorkey );

  return colorkey;
}

const uint8 Canvas::getCanvasAlphaValue ( void ) const
{
  SDL_PixelFormat* fmt = static_cast<SDL_PixelFormat*> ( this->getCanvasPixelsFormat() );
  return fmt->alpha;
}

const uint32 Canvas::getCanvasRedMask ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format->Rmask;
}

const uint32 Canvas::getCanvasGreenMask ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format->Gmask;
}

const uint32 Canvas::getCanvasBlueMask ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format->Bmask;
}

const uint32 Canvas::getCanvasAlphaMask ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->format->Amask;
}

const Coords Canvas::getCanvasBounds ( void ) const
{
  SDL_Rect clip_buffer; // temporary storage struct
  Coords clip_bounds; // transferred values from SDL_Rect clip_buffer

  // Return values are put into the clip_buffer SDL_Rect after executing:
  SDL_GetClipRect ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) , &clip_buffer );

  // Now transfer the values into our preferred data container type
  clip_bounds = Coords ( clip_buffer.x, clip_buffer.y, clip_buffer.w, clip_buffer.h );

  return clip_bounds;
}

void Canvas::setCanvasBounds ( const Coords& clip_bounds )
{
  SDL_Rect clip = getSDL_Rect ( clip_bounds ); // temporary storage struct for setting

  // As per libSDL docs, if SDL_Rect is nullptr, the clipping rectangle is set
  // to the full size of the surface
  SDL_SetClipRect ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), &clip );
}

int32 Canvas::getCanvasColorDepth ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );

  switch ( buffer->format->BytesPerPixel )
  {
    default: return -1; break; // Unsupported color depth

    case 1: return 8; break; // 8-bit
    case 2: return 16; break; // 16-bit
    case 3: return 24; break; // 24-bit
    case 4: return 32; break; // 32-bit
  }
}

bool Canvas::getCanvasLock ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );
  return buffer->locked;
}

bool Canvas::mustLock ( void ) const
{
  if ( SDL_MUSTLOCK ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ) )
    return true;
  else
    return false;
}

bool Canvas::lock ( void ) const
{
  if ( this->mustLock() == true )
  {
    if ( SDL_LockSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ) == -1 )
    {
NOM_LOG_ERR ( "Could not lock video surface memory." );
      return false;
    }
  }
  return true;
}

void Canvas::unlock ( void ) const
{
  SDL_UnlockSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) );
}

bool Canvas::load ( const std::string& filename, const Color& colorkey,
                    bool use_cache, uint32 flags
                  )
{
  Image image;

  // By default -- for peace of mind above all else -- we have caching turned
  // off
  if ( use_cache )
  {
    priv::ObjectCache cache;

    this->canvas_buffer = cache.getObject ( filename );

    if ( this->canvas_buffer == nullptr )
    {
      this->canvas_buffer = cache.addObject ( filename, image.load ( filename ) );
    }
  }
  else // Do not use the object cache
  {
    this->canvas_buffer = std::shared_ptr<void> ( image.load ( filename ) );
  }

  // Validate our obtained data is good before further processing
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "Could not load canvas image file: " + filename );
    return false;
  }

  // We produce a segmentation fault here if we do not have SDL's video
  // subsystem initialized before making the following calls -- transparency
  // and display format conversion.
NOM_ASSERT ( SDL_WasInit ( SDL_INIT_VIDEO) );

  if ( flags & SDL_SRCCOLORKEY )
    this->setTransparent ( colorkey, flags );

  if ( flags & SDL_SRCALPHA )
  {
    this->displayFormatAlpha(); // Optimized video surface with an alpha channel
  }
  else
  {
    this->displayFormat(); // Optimized video surface without an alpha channel
  }

  // Update our canvas clipping bounds with the new source; not sure if we still
  // need to be doing this.
  this->offsets.setSize ( this->getCanvasWidth(), this->getCanvasHeight() );

  return true;
}

void Canvas::Draw ( void* video_buffer ) const
{
  // temporary vars to store our wrapped Coords
  SDL_Rect blit_coords = getSDL_Rect ( this->coords );
  SDL_Rect blit_offsets = getSDL_Rect ( this->offsets );

  // Perhaps also check to see if video_buffer is nullptr?
  if ( this->valid() )
  {
    if ( blit_offsets.w != -1 && blit_offsets.h != -1 )
    {
      if ( SDL_BlitSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), &blit_offsets, static_cast<SDL_Surface*> ( video_buffer ), &blit_coords ) != 0 )
NOM_LOG_ERR ( SDL_GetError() );
        return;
    }
    else
    {
      if ( SDL_BlitSurface ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), nullptr, (SDL_Surface*) video_buffer, &blit_coords ) != 0 )
NOM_LOG_ERR ( SDL_GetError() );
        return;
    }
  }
}

bool Canvas::Update ( void* video_buffer )
{
  if ( SDL_Flip ( (SDL_Surface*) video_buffer ) != 0 )
  {
NOM_LOG_ERR ( SDL_GetError() );
    return false;
  }
  return true;
}

bool Canvas::setAlpha ( uint8_t opacity, uint32_t flags )
{
NOM_ASSERT ( ! ( opacity > SDL_ALPHA_OPAQUE ) || ( opacity < SDL_ALPHA_TRANSPARENT ) );

  if ( SDL_SetAlpha ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), flags, static_cast<uint32_t>( opacity ) ) == -1 )
  {
NOM_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

bool Canvas::setTransparent ( const Color& color, uint32_t flags )
{
  uint32_t transparent_color = 0;

  // TODO: Alpha value needs testing
  transparent_color = getColorAsInt ( this->getCanvasPixelsFormat(), color );

  if ( SDL_SetColorKey ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ), flags, transparent_color ) != 0 )
  {
NOM_LOG_ERR ( SDL_GetError() );
    return false;
  }

  return true;
}

bool Canvas::displayFormat ( void )
{
  this->canvas_buffer.reset ( SDL_DisplayFormat ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ), nom::priv::Canvas_FreeSurface );

NOM_ASSERT ( this->valid() );

  return true;
}

bool Canvas::displayFormatAlpha ( void )
{
  this->canvas_buffer.reset ( SDL_DisplayFormatAlpha ( static_cast<SDL_Surface*> ( this->canvas_buffer.get() ) ), nom::priv::Canvas_FreeSurface );

NOM_ASSERT ( this->valid() );

  return true;
}

void Canvas::clear ( const Color& color ) const
{
  Rectangle rect ( Coords ( 0, 0, this->getCanvasWidth(), this->getCanvasHeight() ), color );

  rect.Draw ( this->canvas_buffer.get() );
}

int32 Canvas::getPixel ( int32 x, int32 y )
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->canvas_buffer.get() );

  switch ( this->getCanvasColorDepth() )
  {
    default: return -1; break; // Unsupported

    case 8:
    {
      uint8* pixels = static_cast<uint8*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch ) + x ];
    }
    break;

    case 16:
    {
      uint16* pixels = static_cast<uint16*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch/2 ) + x ];
    }
    break;

    case 24:
    {
      uint8* pixels = static_cast<uint8*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch ) + x ];
    }
    break;

    case 32:
    {
      uint32* pixels = static_cast<uint32*> ( buffer->pixels );

      return pixels[ ( y * buffer->pitch/4 ) + x ];
    }
    break;
  } // end switch
}

bool Canvas::resize ( enum ResizeAlgorithm scaling_algorithm )
{
  switch ( scaling_algorithm )
  {
    default: // No resizing is applied
    case ResizeAlgorithm::NearestNeighbor: // Not implemented
    case ResizeAlgorithm::scale3x: // Not implemented
    case ResizeAlgorithm::scale4x: // Not implemented
    case ResizeAlgorithm::hq3x: // Not implemented
    case ResizeAlgorithm::hq4x: // Not implemented
      return false;
    break;

    case ResizeAlgorithm::scale2x:
    {
      if ( this->scale2x() == false ) return false;
    }
    break;

    case ResizeAlgorithm::hq2x:
    {
      if ( this->hq2x() == false ) return false;
    }
    break;
  }

  return true;
}

bool Canvas::scale2x ( void )
{
  // Ensure that our existing video surface is OK first
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "The existing video surface is not valid." );
    return false;
  }

  // Current video surface flags state (primarily for handling setting the
  // color keys and/or alpha values upon our resulting video surface.
  uint32 flags = this->getCanvasFlags();

  // Save a temporary copy of the existing width & height for scaling
  // calculation.
  const int32 width = this->getCanvasWidth();
  const int32 height = this->getCanvasHeight();

  // This is the target video surface object that is created from the existing
  // video surface (but by a scale factor of two 2).
  //
  // We derive our new video surface parameters from the existing video state at
  // the time of this call with the one exception of the video surface width and
  // height (these obviously need to be sized up by the scaling factor).
  Canvas destination_buffer;

  // We must not set an alpha mask value if our existing video surface is color
  // keyed (or bad things ensue -- like many hours spent reading up on this
  // surprisingly confusing subject).
  if ( flags & SDL_SRCCOLORKEY )
  {
    destination_buffer = Canvas (
                                  this->getCanvasWidth() * 2, // x2 width
                                  this->getCanvasHeight() * 2, // x2 height
                                  this->getCanvasBitsPerPixel(),
                                  this->getCanvasRedMask(),
                                  this->getCanvasGreenMask(),
                                  this->getCanvasBlueMask(),
                                  0 // No alpha mask value
                                );
  }
  else // Throw all the alpha you can eat if surface has alpha blending enabled!
  {
    destination_buffer = Canvas (
                                  this->getCanvasWidth() * 2, // x2 width
                                  this->getCanvasHeight() * 2, // x2 height
                                  this->getCanvasBitsPerPixel(),
                                  this->getCanvasRedMask(),
                                  this->getCanvasGreenMask(),
                                  this->getCanvasBlueMask(),
                                  // FIXME
                                  0//this->getCanvasAlphaMask(),
                                  //SDL_SRCALPHA
                                );
  }

  // Ensure that our new video surface is sane
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "The destination video surface is not valid." );
    return false;
  }

  // The video surface pitch (width) is saved for scaling calculations.
  const uint16 srcpitch = this->getCanvasPitch();

  // We must use the new video surface configuration for computing the pitch as
  // this is dependent upon width & height parameters.
  const uint16 dstpitch = destination_buffer.getCanvasPitch();

  // Existing & resulting pixel arrays
  uint8* srcpix = static_cast<uint8*> ( this->getCanvasPixels() );
  uint8* dstpix = static_cast<uint8*> ( destination_buffer.getCanvasPixels() );

  // Lock pixels buffer for writing to
  if ( this->lock() == false )
  {
NOM_LOG_ERR ( "Could not lock video surface memory." );
    return false;
  }

  // Use the existing video surface BPP for choosing scaling algorithm.
  switch ( this->getCanvasColorDepth() )
  {
    default: // ERR -- could not determine a valid color depth
    {
NOM_LOG_ERR ( "Could not determine color depth -- aborting." );
      this->unlock(); // Relinquish our write lock
      return false;
    break;
    } // end unsupported color depth

    case 8:
    {
      uint8 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = *(uint8*)(srcpix + (std::max ( 0, looph - 1 ) * srcpitch ) + ( 1 * loopw ) );
          D = *(uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * std::max ( 0,loopw - 1 ) ) );
          E = *(uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * loopw ) );
          F = *(uint8*)(srcpix + ( looph * srcpitch ) + ( 1 * std::min ( width - 1,loopw + 1 ) ) );
          H = *(uint8*)(srcpix + ( std::min ( height - 1, looph + 1 ) * srcpitch ) + ( 1 * loopw ) );

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(uint8*)(dstpix + looph*2*dstpitch + loopw*2*1) = E0;
          *(uint8*)(dstpix + looph*2*dstpitch + (loopw*2+1)*1) = E1;
          *(uint8*)(dstpix + (looph*2+1)*dstpitch + loopw*2*1) = E2;
          *(uint8*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*1) = E3;
        } // for width loop
      } // for height loop
    break;
    } // end case 8

    case 16:
    {
      uint16 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = *(uint16*)(srcpix + ( std::max ( 0, looph - 1 ) * srcpitch ) + ( 2 * loopw ) );
          D = *(uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * std::max ( 0, loopw - 1 ) ) );
          E = *(uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * loopw ) );
          F = *(uint16*)(srcpix + ( looph * srcpitch ) + ( 2 * std::min ( width - 1, loopw + 1 ) ) );
          H = *(uint16*)(srcpix + ( std::min ( height - 1, looph + 1 ) * srcpitch ) + ( 2 * loopw ) );

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(uint16*)(dstpix + looph*2*dstpitch + loopw*2*2) = E0;
          *(uint16*)(dstpix + looph*2*dstpitch + (loopw*2+1)*2) = E1;
          *(uint16*)(dstpix + (looph*2+1)*dstpitch + loopw*2*2) = E2;
          *(uint16*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*2) = E3;
        } // for width loop
      } // for height loop
    break;
    } // end case 16

    case 24:
    {
      int32 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = SCALE2x_READINT24(srcpix + (std::max(0,looph-1)*srcpitch) + (3*loopw));
          D = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*std::max(0,loopw-1)));
          E = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*loopw));
          F = SCALE2x_READINT24(srcpix + (looph*srcpitch) + (3*std::min(width-1,loopw+1)));
          H = SCALE2x_READINT24(srcpix + (std::min(height-1,looph+1)*srcpitch) + (3*loopw));

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          SCALE2x_WRITEINT24((dstpix + looph*2*dstpitch + loopw*2*3), E0);
          SCALE2x_WRITEINT24((dstpix + looph*2*dstpitch + (loopw*2+1)*3), E1);
          SCALE2x_WRITEINT24((dstpix + (looph*2+1)*dstpitch + loopw*2*3), E2);
          SCALE2x_WRITEINT24((dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*3), E3);
        } // for width loop
      } // for height loop
    break;
    } // end case 24

    case 32:
    {
      uint32 E0, E1, E2, E3, B, D, E, F, H;

      for ( int32 looph = 0; looph < height; ++looph)
      {
        for ( int32 loopw = 0; loopw < width; ++loopw)
        {
          B = *(uint32*)(srcpix + (std::max(0,looph-1)*srcpitch) + (4*loopw));
          D = *(uint32*)(srcpix + (looph*srcpitch) + (4*std::max(0,loopw-1)));
          E = *(uint32*)(srcpix + (looph*srcpitch) + (4*loopw));
          F = *(uint32*)(srcpix + (looph*srcpitch) + (4*std::min(width-1,loopw+1)));
          H = *(uint32*)(srcpix + (std::min(height-1,looph+1)*srcpitch) + (4*loopw));

          E0 = D == B && B != F && D != H ? D : E;
          E1 = B == F && B != D && F != H ? F : E;
          E2 = D == H && D != B && H != F ? D : E;
          E3 = H == F && D != H && B != F ? F : E;

          *(uint32*)(dstpix + looph*2*dstpitch + loopw*2*4) = E0;
          *(uint32*)(dstpix + looph*2*dstpitch + (loopw*2+1)*4) = E1;
          *(uint32*)(dstpix + (looph*2+1)*dstpitch + loopw*2*4) = E2;
          *(uint32*)(dstpix + (looph*2+1)*dstpitch + (loopw*2+1)*4) = E3;
        } // for width loop
      } // for height loop
    } // end case 32
    break;
  } // end switch (BytesPerPixel)

  this->unlock(); // Relinquish our write lock

  // If we have gotten this far, we assume success has been made in video
  // surface scaling and thus proceed to reset the video surface object's video
  // memory to the resulting pixel data.
  this->setCanvas ( destination_buffer );

  // Last, but not least, we copy over transparency info onto our new video surface
  // if the appropriate flag is set.
  if ( flags & SDL_SRCCOLORKEY )
  {
    if ( this->setTransparent ( this->getCanvasColorKey(), SDL_RLEACCEL | SDL_SRCCOLORKEY ) == false )
    {
      return false;
    }
  }

  // Do one more sanity check on our new video surface
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "The rescaled video surface is not valid." );
    return false;
  }

  return true;
}

bool Canvas::hq2x ( void )
{
  // Ensure that our existing video surface is OK first
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "The existing video surface is not valid." );
    return false;
  }

  // Save a temporary copy of the existing width & height for scaling
  // calculation.
  const int32 width = this->getCanvasWidth();
  const int32 height = this->getCanvasHeight();

  // Current video surface flags state (primarily for handling setting the
  // color keys and/or alpha values upon our resulting video surface.
  uint32 flags = this->getCanvasFlags();

  // This is the target video surface object that is created from the existing
  // video surface (but by a scale factor of two 2).
  //
  // We derive our new video surface parameters from the existing video state at
  // the time of this call with the one exception of the video surface width and
  // height (these obviously need to be sized up by the scaling factor).
  Canvas destination_buffer;

  // We must not set an alpha mask value if our existing video surface is color
  // keyed (or bad things ensue -- like many hours spent reading up on this
  // surprisingly confusing subject).
  if ( flags & SDL_SRCCOLORKEY )
  {
    destination_buffer = Canvas (
                                  width * 2, // x2 width
                                  height * 2, // x2 height
                                  this->getCanvasBitsPerPixel(),
                                  this->getCanvasRedMask(),
                                  this->getCanvasGreenMask(),
                                  this->getCanvasBlueMask(),
                                  0
                                );
  }
  else // Throw all the alpha you can eat if surface has alpha blending enabled!
  {
    destination_buffer = Canvas (
                                  width * 2, // x2 width
                                  height * 2, // x2 height
                                  this->getCanvasBitsPerPixel(),
                                  this->getCanvasRedMask(),
                                  this->getCanvasGreenMask(),
                                  this->getCanvasBlueMask(),
                                  0//this->getCanvasAlphaMask(),
                                  //SDL_SRCALPHA
                                );
  }

  // Ensure that our new video surface is sane
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "The destination video surface is not valid." );
    return false;
  }

  hqxInit();

  // Lock pixels buffer for writing to
  if ( this->lock() == false )
  {
NOM_LOG_ERR ( "Could not lock video surface memory." );
    return false;
  }

  // Note that we must pass the *original* width and height here
  hq2x_32 ( static_cast<uint32*> ( this->getCanvasPixels() ), static_cast<uint32*> ( destination_buffer.getCanvasPixels() ), width, height );

  this->unlock(); // Relinquish our write lock

  // If we have gotten this far, we assume success has been made in video
  // surface scaling and thus proceed to reset the video surface object's video
  // memory to the resulting pixel data.
  this->setCanvas ( destination_buffer );

  // Last, but not least, we copy over transparency info onto our new video surface
  // if the appropriate flag is set.
  if ( flags & SDL_SRCCOLORKEY )
  {
    if ( this->setTransparent ( this->getCanvasColorKey(), SDL_RLEACCEL | SDL_SRCCOLORKEY ) == false )
    {
      return false;
    }
  }

  // Do one more sanity check on our new video surface
  if ( this->valid() == false )
  {
NOM_LOG_ERR ( "The rescaled video surface is not valid." );
    return false;
  }

  return true;
}


} // namespace nom
