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
#ifndef NOMLIB_SDL2_TEXTURE_HEADERS
#define NOMLIB_SDL2_TEXTURE_HEADERS

#include <iostream>
#include <string>
#include <memory>

#include <SDL.h>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Rect.hpp"

// Dump the rescaled Texture as a PNG file
//#define NOM_DEBUG_SDL2_RESIZE_PNG

namespace nom {

// Forward declarations
class Image;
class RenderWindow;

class Texture
{
  public:
    typedef Texture self_type;
    typedef std::shared_ptr<Texture> SharedPtr;

    /// \brief Available pixel rescaling algorithms
    enum ResizeAlgorithm
    {
      None = 0, // No resizing is applied
      scale2x,
      scale3x, // Reserved for future implementation
      scale4x, // Reserved for future implementation
      hq2x,
      hq3x,
      hq4x
    };

    enum Access
    {
      Invalid = 0,
      Static = SDL_TEXTUREACCESS_STATIC,        // Changes rarely; not lockable
      Streaming = SDL_TEXTUREACCESS_STREAMING,  // Frequent changes; lockable, RW
      RenderTarget = SDL_TEXTUREACCESS_TARGET   // Render (texture) to target
    };

    /// \brief Default constructor.
    ///
    /// \fixme position_ should be initialized to Point2i::null.
    /// \fixme bounds_ should be initialized to IntRect::null.
    /// \fixme colorkey_ should be initialized to Color4i::null.
    Texture( void );

    /// \brief Destructor.
    ///
    /// \remarks Frees any pixel data associated with the Texture
    ///
    /// \remarks The video surface -- if locked -- will be unlocked at this
    /// time before freeing its memory.
    ~Texture( void );

    /// \brief Destroy the texture.
    ///
    /// \remarks The pixel data and pitch value associated with the Texture is
    /// freed as well. The other attributes, such as position and color key are
    /// *not* reset to their respective defaults.
    void free_texture( void );

    /// Copy constructor
    Texture ( const Texture& copy );

    /// Copy assignment operator
    Texture& operator = ( const Texture& other );

    /// \brief Get a shallow-copy of the underlying stored texture.
    ///
    /// \returns A pointer to a new nom::Texture instance from the stored
    /// data of this object's instance. The returned pointer is owned by the
    /// caller.
    ///
    /// \remarks The cloned instance shares the same internal texture memory.
    /// If a deep-copy clone is required, you should either keep the nom::Image
    /// source used to create the texture's pixel buffer and clone from it
    /// instead. Alternatives may include using a Render To Texture target or
    /// nom::Renderer::pixels.
    ///
    /// \todo Change return type to std::unique_ptr<Texture> to better express
    /// intent?
    Texture* clone() const;

    /// Initialize an object with specified parameters
    ///
    /// \param width    Width -- in pixels -- of the new texture
    /// \param height   Height -- in pixels -- of the new texture
    /// \param format   A pixel format; one of the enumerated values in
    ///                 SDL_PixelFormatEnum
    /// \param flags    Texture access type; one of the enumerated values in
    ///                 SDL_TextureAccess
    bool initialize ( uint32 format, uint32 flags, int width, int height );

    /// \see ::intialize(uint32 format, uint32 flags, int width, int height)
    bool initialize(uint32 format, uint32 flags, const Size2i& dims);

    /// \remarks Texture::Access::Static type
    bool create ( const Image& source );

    /// \remarks For creation of textures of access types: Access::Streaming and
    /// Access::RenderTarget.
    bool create( const Image& source, uint32 pixel_format, enum Texture::Access type );

    const Point2i& position( void ) const;

    /// \brief Get the width & height dimensions of the texture.
    ///
    /// \remarks This method return invalid results if the bounds has not
    /// been set.
    ///
    /// \fixme We need to decide if we ought to return the dimensions as per
    /// what SDL has on record, or if we should use cached values.
    const Size2i size( void ) const;

    const IntRect& bounds( void ) const;

    /// \brief Get the underlying texture stored.
    SDL_Texture* texture() const;

    /// Is this object initialized -- not nullptr?
    bool valid ( void ) const;

    /// \brief Query texture access type
    ///
    /// \returns Texture::Access enumeration
    enum Texture::Access access ( void ) const;

    void set_position( const Point2i& pos );

    /// \brief Set the width & height dimensions of the texture.
    ///
    /// \remarks If the width or height dimensions are greater than the
    /// original source dimensions, they will automatically be rescaled.
    void set_size( const Size2i& size );

    /// Set bounding coordinates of the Texture
    void set_bounds( const IntRect& bounds );

    /// Obtain width, in pixels, of texture
    ///
    /// \return -1 on error; non-zero width on success
    int32 width ( void ) const;

    /// Obtain height, in pixels, of texture
    ///
    /// \return -1 on error; non-zero height on success
    int32 height ( void ) const;

    int pitch ( void ) const;

    void* pixels ( void ) const;

    uint8 bytes_per_pixel ( void ) const;

    uint32 pixel_format ( void ) const;

    /// Calculate this object's bits per pixel (color depth)
    ///
    /// \returns  Integer value of 8, 16, 24 or 32 on success; 32-bit if the
    ///           color depth is unknown.
    uint8 bits_per_pixel ( void ) const;

    /// Obtain the blending mode used for texture copies
    const SDL_BlendMode blend_mode ( void ) const;

    /// Obtain the set color key for this image
    ///
    /// \return   Returns non-negative nom::Color on success;
    ///           nom::Color::null on failure
    const Color4i& colorkey ( void ) const;

    /// \brief Obtain the set color key for this image
    ///
    /// \returns  Value between Color4i::ALPHA_OPAQUE and
    ///           Color4i::ALPHA_TRANSPARENT.
    uint8 alpha ( void ) const;

    /// \brief  Obtain the additional color value multiplied into render copy
    ///         operations.
    ///
    /// \returns  nom::Color4i on success; nom::Color4i::null on err
    const Color4i color_modulation ( void ) const;

    /// \brief Obtain the largest nom::Texture size allowable by your graphics
    /// hardware.
    ///
    /// \return A new Point2i object filled in with width (x) and height (y)
    /// values, in pixel units.
    ///
    /// \note The values obtained here are dependent upon the nom::Renderer
    /// context used at the time of creation of the nom::Texture -- in other
    /// words, whichever nom::RenderWindow was active at the time of the nom::Texture
    /// creation.
    static const Point2i maximum_size ( void );

    /// \brief Query lock status of texture
    bool locked ( void ) const;

    /// \brief Lock the entire bounds of the texture for write access to the
    /// pixel buffer.
    ///
    /// \remarks Texture must have been created as the Access::Streaming type.
    bool lock ( void );

    /// \brief Lock a portion of the texture for write access to the pixel
    /// buffer.
    ///
    /// \param bounds The area encompassing the area to lock; pass IntRect::null
    /// to lock the entire surface of the texture.
    ///
    /// \remarks Texture must have been created as the Access::Streaming type.

    bool lock ( const IntRect& bounds );

    /// Unlock the texture; signals the OK to upload the pixel buffer to the GPU
    ///
    /// \remarks The storage for the pixel buffer will be deleted from memory.
    void unlock ( void );

    /// Load an image into memory from a file
    ///
    /// \param filename         File path you wish to load into memory as a
    ///                         valid bitmap / texture
    ///                         (think: supported image file types).
    ///
    /// \param type             nom::Texture::Access enumeration type
    ///
    /// \param use_cache        Whether or not to use an internal object cache
    ///                         feature of nomlib. Defaults to off.
    ///
    /// \todo Test/Research Texture caching -- nom::ObjectCache worked
    /// beautifully with SDL_Surface, is the same true of SDL_Texture?
    ///
    /// \todo merge 'use_cache' in with 'flags'
    bool load ( const std::string& filename,
                bool use_cache = false,
                enum Texture::Access type = Access::Static
              );

    /// \brief Upload pixels to texture
    ///
    /// \param source   Pixels to upload to the texture
    /// \param pitch    Pitch of the source pixels
    /// \param bounds   Rectangle bounds area to update; IntRect::null to update
    ///                 the entire texture
    ///
    /// \remarks This is intended for use with Texture::Access::Static texture
    /// types. When used with Texture::Access::Streaming texture types, you may
    /// not get the pixels back if you lock the texture afterwards.
    bool update_pixels(const void* source, uint16 pitch, const IntRect& bounds);

    /// Draw a nom::Texture to a SDL_Renderer target
    ///
    /// \param  SDL_Renderer
    ///
    void draw ( SDL_Renderer* target ) const;

    /// Draw a nom::Texture to a nom::RenderWindow target
    ///
    /// \param  nom::RenderWindow
    ///
    /// \note This is an alias for nom::Texture::draw ( SDL_Renderer* )
    void draw( const RenderWindow& target ) const;

    /// Draw a rotated nom::Texture to a rendering target
    ///
    /// \param  target  Pointer to SDL_Renderer context
    /// \param  angle   Rotation angle in degrees
    ///
    /// \todo Implement pivot point & make use of SDL_RendererFlip enum
    void draw ( SDL_Renderer* target, const double angle ) const;

    /// Draw a rotated nom::Texture on a nom::RenderWindow
    ///
    /// \param  target  Reference to an active nom::RenderWindow
    /// \param  angle   Rotation angle in degrees
    void draw( const RenderWindow& target, const double angle ) const;

    // FIXME: This is a TEMPORARY function just to get AnimationScaleBy working
    bool
    draw_scaled(const Size2i& dims, real64 angle, const RenderWindow& target) const;

    /// \brief  Set an additional alpha value multiplied into render copy
    ///         operations.
    ///
    /// \remarks Upon rendering, the following formula is used for the alpha
    /// modulation: srcA = srcA * ( alpha / 255 )
    bool set_alpha ( uint8 opacity );

    /// \brief Read a RGBA pixel from the video surface
    ///
    /// \returns  A 32-bit encoded color value, dependent upon the pixel format.
    ///
    /// \remarks You are responsible for any necessary video surface locking
    /// and unlocking.
    ///
    /// \note The color depth is determined by the existing color depth of the
    ///       nom::Image -- 8-bit, 15/16-bit, 24-bit or 32-bit bits per pixel
    ///       depths are supported. When a color depth cannot be determined, it
    ///       is assumed 32-bit.
    ///
    /// \todo Test 8-bit, 15/16-bit & 24-bit pixel blits
    uint32 pixel ( int x, int y );

    /// \brief Rescale the Texture with the chosen rescaling algorithm.
    ///
    /// \note See the ResizeAlgorithm enum for available rescaling algorithms.
    ///
    /// \remarks  This method call requires that the existing Texture is of the
    ///           Texture::Access::Streaming type. Once resized, the rescaled
    ///           texture is of the Texture::Access::Static type and therefore
    ///           cannot be modified again without the complete re-initialization
    ///           of the original buffer (image file).
    ///
    /// \todo Improve err handling of this method when ScaleX & HQX algorithms
    /// are not built. (Perhaps log a message warning the user that since the
    /// algorithms are not built in, that SDL2's bilinear rescaling will be used
    /// instead???)
    bool resize( enum ResizeAlgorithm scaling_algorithm );

    /// \brief Return the scaling factor of the chosen algorithm
    int scale_factor( enum ResizeAlgorithm scaling_algorithm ) const;
    int scale_factor() const;

    /// Set a new blending mode for this texture
    bool set_blend_mode ( const SDL_BlendMode blend );

    /// Set a new color key
    ///
    /// \param colorkey     nom::Color object representing the RGB pixel to be
    ///                     marked transparent.
    ///
    /// \todo ERR check on locking call, etc.
    ///
    /// (This method requires locking the texture; use wisely!).
    ///
    /// \remarks This method converts a a color key to a fully transparent
    /// alpha channel.
    bool set_colorkey(const Color4i& colorkey);

    /// \brief    Set an additional color value multiplied into render copy
    ///           operations
    ///
    /// \param    nom::Color4i  red, green & blue values multiplied into
    ///                         color operations
    ///
    /// \returns  TRUE on success; FALSE on failure
    ///
    /// \remarks  SDL2 color modulation formula:
    ///           srcC = srcC * ( color / 255 )
    bool set_color_modulation ( const Color4i& color );

    bool copy_pixels ( const void* source, int pitch );

    /// \brief    Set the nom::Texture as the current renderer target.
    ///
    /// \remarks  nom::Texture access type must be Texture::Access::RenderTarget
    ///
    /// \note     Not all graphics hardware supports this request
    ///
    /// \see nom::Renderer::reset_render_target.
    bool set_render_target(RenderWindow& target);

  private:
    void set_scale_factor(int factor);

    std::shared_ptr<SDL_Texture> texture_;

    /// Texture's pixels; these are only available when a Texture is locked.
    void* pixels_;

    /// Texture's pixel pitch; these are only available when a Texture is locked.
    int pitch_;

    /// Rendering position & size -- X, Y, width & height -- in pixels.
    ///
    /// \remarks  Rescaling of the rendered pixels can be done by modifying the
    ///           width & height members.
    Point2i position_; // This should probably be an IntRect. (Global bounds).

    /// \todo This needs to be used ASAP; we've been using our texture source
    /// bounds with our texture size coordinates, and this makes a huge,
    /// confusing mess of things. See Texture.cpp for additional comments.
    // Size2i size_;

    /// Position & size of texture within memory; X, Y, width & height in pixels.
    ///
    /// \remarks  These coordinates are generally not used unless we are reading
    ///           pixels from a larger texture than what is rendered --
    ///           in other words: these are used for sprite sheets, and are also
    ///           known as clipping bounds.
    IntRect bounds_; // Local bounds

    /// Cached upon use of the set_colorkey method for use by external classes
    Color4i colorkey_;

    int scale_factor_;
};


} // namespace nom

#endif // include guard defined
