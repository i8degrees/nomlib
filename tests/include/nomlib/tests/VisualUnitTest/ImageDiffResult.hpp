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
#ifndef NOMLIB_TESTS_COMMON_IMAGE_DIFF_RESULT_HPP
#define NOMLIB_TESTS_COMMON_IMAGE_DIFF_RESULT_HPP

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/graphics/Image.hpp"

namespace nom {

/// \brief The results from an image differential comparison
struct ImageDiffResult
{
  /// \brief Default constructor; initialize variables to zero.
  ImageDiffResult( void ) :
    passed{ false },
    frame{ 0 },
    incorrect_pixels{ 0 },
    mse{ 0 },
    psnr{ 0 },
    ssim{ 0 }
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Destructor.
  ~ImageDiffResult( void )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief The test result.
  bool passed;

  // \brief The test image.
  std::string image_filename;

  /// \brief The test description.
  std::string test_name;

  // \brief The frame number of the image.
  uint frame;

  /// \brief The number of incorrect pixels in the comparison.
  uint incorrect_pixels;

  /// \brief Mean Squared Error.
  float mse;
  Color4f mse_channels;

  /// \brief Peak signal-to-noise ratio.
  float psnr;
  Color4f psnr_channels;

  /// \brief Structural similarity index.
  float ssim;

  /// \brief The differing pixels between the image comparison, using the
  /// "Difference" blending algorithm.
  Image image;
};

typedef std::vector<ImageDiffResult> ImageDiffResultBatch;

} // namespace nom

#endif // include guard defined

/// \struct nom::ImageDiffResult
/// \ingroup tests
///
/// \see Adapted from the implementation for [Ogre3D](http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework) by Riley Adams <praetor57@gmail.com>.
///
/// \see nom::VisualUnitTest, nom::ImageDiff
///
