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
#include "nomlib/tests/VisualUnitTest/ImageDiff.hpp"

// Forward declarations
#include "nomlib/graphics/Image.hpp"

// Private headers
#include "nomlib/system/Path.hpp"

namespace nom {

ImageDiff::ImageDiff  (
                        const std::string& dir1_path,
                        const std::string& dir2_path
                      ) :
  image1_directory_(dir1_path),
  image2_directory_(dir2_path)
{
  // NOM_LOG_TRACE( NOM );
}

ImageDiff::~ImageDiff( void )
{
  // NOM_LOG_TRACE( NOM );
}

std::string ImageDiff::directory( int pos )
{
  NOM_ASSERT( ! ( pos < 0 && pos > 1 ) );

  if( pos == 0 )
  {
    return this->image1_directory_;
  }
  else if( pos == 1 )
  {
    return this->image2_directory_;
  }

  // Err
  return "\0";
}

ImageDiffResult ImageDiff::compare( const std::string& filename )
{
  Path p;
  ImageDiffResult results;

  std::string img1_path = this->directory(0) + p.native() + filename;
  std::string img2_path = this->directory(1) + p.native() + filename;

  results = this->compare_files( img1_path, img2_path );

  results.image_filename = img1_path;

  return results;
}

ImageDiffResult ImageDiff::compare_files  (
                                            const std::string& image1_path,
                                            const std::string& image2_path
                                          )
{
  Path p;
  Image img1;
  Image img2;
  ImageDiffResult results;

  // NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "image 1 diff path: ", img1_path);
  // NOM_DUMP("image 2 diff path: ", img2_path);

  if( img1.load( image1_path ) == false || img2.load( image2_path ) == false )
  {
    NOM_LOG_ERR( NOM, "Could not compare image files: path is invalid." );

    // ImageDiffResults::passed should always be false upon ctor
    return results;
  }
  else
  {
    this->comparison_algorithm( img1, img2, results );

    // Success!
    return results;
  }
}

void ImageDiff::comparison_algorithm  (
                                        const Image& img1,
                                        const Image& img2,
                                        ImageDiffResult& results
                                      )
{
  NOM_ASSERT( img1.valid() && img2.valid() );

  // Size dimensions must be the same on both images
  NOM_ASSERT( img1.width() == img2.width() && img1.height() == img2.height() );

  results.incorrect_pixels = 0;
  float ssim = 0.0;

  Color4f disparity = Color4f::Black;
  Size2i size( img1.width(), img1.height() );

  // Invalid size dimensions
  NOM_ASSERT( size != Size2i::null );

  // Iterate through in 8x8 chunks, so we can calculate SSIM at the same time
  // as MSE
  for( auto i = 0; i < size.w / 8; ++i )
  {
    for( auto j = 0; j < size.h / 8; ++j )
    {
      // Number of pixels processed
      int n = 0;

      // Dynamic range; 0.0-1.0
      float dr = 1.f;

      // Constants
      float c1 = (0.01f * dr ) * (0.01f * dr );
      float c2 = (0.03f * dr ) * (0.03f * dr );

      // Averages
      float avg_x = 0.f;
      float avg_y = 0.f;

      // Variances
      float var_x = 0.f;
      float var_y = 0.f;

      // Covariance
      float covar = 0.f;

      for( auto k = 0; k < 8 && i * 8 + k < size.w; ++k )
      {
        for( auto l = 0; l < 8 && j * 8 + l < size.h; ++l )
        {
          ++n;

          Color4f col1 = img1.color4i_pixel( i * 8 + k, j * 8 + l );
          Color4f col2 = img2.color4i_pixel( i * 8 + k, j * 8 + l );

          if( col1 != col2 )
          {
            ++results.incorrect_pixels;
            disparity += ( col1 - col2 ) * ( col1 - col2 );
          }

          // Calculations for SSIM:
          // We'll be working with the luminosity for SSIM (computed by the
          // standard Rec. 709 definition)
          float lum1 = 0.2126f * col1.r + 0.7152f * col1.g + 0.0722f * col1.b;
          float lum2 = 0.2126f * col2.r + 0.7152f * col2.g + 0.0722f * col2.b;
          float delta_x = lum1 - avg_x;
          float delta_y = lum2 - avg_y;
          avg_x += delta_x / ( k * 8 + l + 1 );
          avg_y += delta_y / ( k * 8 + l + 1 );
          var_x += delta_x * ( lum1 - avg_x );
          var_y += delta_y * ( lum2 - avg_y );
          covar += lum1 * lum2;
        }
      }

      // More SSIM stuff:
      var_x = var_x / n;
      var_y = var_y / n;
      covar = covar / n - avg_x * avg_y;

      // Calculation based on: Z. Wang, A. C. Bovik, H. R. Sheikh and
      // E. P. Simoncelli, "Image quality assessment: From error visibility
      // to structural similarity," IEEE Transactions on Image Processing,
      // vol. 13, no. 4, pp. 600-612, Apr. 2004.
      ssim += ( ( 2 * avg_x * avg_y + c1 ) * ( 2 * covar + c2 ) ) /
              ( ( avg_x * avg_x + avg_y * avg_y + c1 ) * ( var_x + var_y + c2 ) );
    }
  }

  // Only bother with these calculations if the images aren't identical
  if( results.incorrect_pixels != 0 )
  {
    // Average and clamp to [-1,1]
    results.ssim = std::max (
                              -1.0f,
                              std::min( 1.0f, ssim / ( size.w * size.h / 64.f) )
                            );

    // Average the raw deviance value to get MSE
    Color4f d = disparity;
    d.r = disparity.r / ( size.w / size.h );
    d.g = disparity.g / ( size.w / size.h );
    d.b = disparity.b / ( size.w / size.h );
    results.mse_channels = d;

    results.mse = (
                    results.mse_channels.r +
                    results.mse_channels.g +
                    results.mse_channels.b
                  ) / 3.f;

    // PSNR = 20 * log10(range/sqrt(mse))
    results.psnr_channels.r = 20 * log10( 1.f / sqrt( results.mse_channels.r ) );
    results.psnr_channels.g = 20 * log10( 1.f / sqrt( results.mse_channels.g ) );
    results.psnr_channels.b = 20 * log10( 1.f / sqrt( results.mse_channels.b ) );
    // results.psnr_channels.a = 20 * log10( 1.f / sqrt( results.mse_channels.a ) );

    results.psnr = 20 * log10( 1.f / sqrt( results.mse ) );

    // Use the Difference color blending mode to visually show image diff

    // Input (img1, img2)
    Color4i c1, c2;

    // Output
    Image diff;
    Color4i blend_color;
    Size2i diff_size;

    // Use the larger of the two inputs for destination width and height
    diff_size.w = std::max( img1.width(), img2.width() );
    diff_size.h = std::max( img1.height(), img2.height() );

    // FIXME: We might want to query SDL for the most optimal pixel format for
    /// rendering here.
    diff.create( diff_size, SDL_PIXELFORMAT_ARGB8888 );

    // Calculate the destination color for each input's pixel...
    for( auto y = 0; y < diff_size.h; ++y )
    {
      for( auto x = 0; x < diff_size.w; ++x )
      {
        if( y < img1.height() && x < img1.width() )
        {
          c1 = img1.color4i_pixel( x, y );
        }
        else
        {
          // Overflow; this properly handles rendering the destination image
          // when the first input image is larger than the second input image.
          c1 = Color4i::Black;
        }

        if( y < img2.height() && x < img2.width() )
        {
          c2 = img2.color4i_pixel( x, y );
        }
        else
        {
          // Overflow; this properly handles rendering the destination image
          // when the second input image is larger than the first input image.
          c2 = Color4i::Black;
        }

        // r = std::abs( c1 - c2 )
        blend_color = nom::difference_color_blend( c1, c2 );

        // Output the diff color
        diff.set_pixel( x, y, blend_color );
      }
    }

    // Output image differential; this can be saved to a PNG file; see
    // Image::save_png(...)
    //
    // FIXME:
    // results.image = diff;
  }

  results.passed = results.incorrect_pixels == 0;
}

} // namespace nom
