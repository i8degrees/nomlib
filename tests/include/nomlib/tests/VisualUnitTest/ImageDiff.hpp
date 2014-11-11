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
#ifndef NOMLIB_TESTS_COMMON_IMAGE_DIFF_HPP
#define NOMLIB_TESTS_COMMON_IMAGE_DIFF_HPP

#include <iostream>
#include <string>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/tests/VisualUnitTest/ImageDiffResult.hpp"

namespace nom {

// Forward declarations
class Image;

/// \brief Interface for image differential comparisons
class ImageDiff
{
  public:
    typedef ImageDiff self_type;

    typedef self_type* raw_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;

    ImageDiff (
                const std::string& dir1_path,
                const std::string& dir2_path
              );

    virtual ~ImageDiff( void );

    /// \brief Get the absolute directory path prefix for the image comparison.
    ///
    /// \param pos The image file path index for the first or second image;
    /// this value **must** be zero (0) or one (1).
    ///
    /// \returns An absolute directory path prefix on success. A null-terminated
    /// string on err.
    std::string directory( int pos );

    /// \brief Compare the differentials for two image files.
    ///
    /// \param filename The relative file path (filename and extension only)
    /// used for comparison. The prefix directory path set in the construction
    /// of the object is used on both image paths.
    ///
    /// \see ImageDiff::comparison_algorithm.
    ImageDiffResult compare( const std::string& filename );

    /// \brief Compare the differentials for two image files.
    ///
    /// \param image1_path The absolute file path to the first image.
    /// \param image2_path The absolute file path to the second image.
    ///
    /// \see ImageDiff::comparison_algorithm.
    ImageDiffResult compare_files (
                                    const std::string& image1_path,
                                    const std::string& image2_path
                                  );

  protected:
    /// \brief The differential algorithm used for differential metrics on two
    /// images.
    ///
    /// \param img1     The first nom::Image input object to compare.
    /// \param img2     The second nom::Image input object to compare.
    /// \param results  The output object to fill.
    ///
    /// \remarks The nom::Image objects must be loaded into memory using
    /// nom::Image::load.
    ///
    /// \note The default implementation computes the MSE, PSNR and SSIM for
    /// two images.
    ///
    /// \fixme The resulting image diff (ImageDiffResult::image) is broken, due
    /// to a memory violation err.
    virtual void comparison_algorithm (
                                        const Image& img1,
                                        const Image& img2,
                                        ImageDiffResult& results
                                      );

  private:
    std::string image1_directory_;
    std::string image2_directory_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::ImageDiff
/// \ingroup tests
///
/// \see nom::VisualUnitTest, nom::ImageDiffResult
///
/// \see Adapted from the implementation for [Ogre3D](http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework) by Riley Adams <praetor57@gmail.com>.
///
/// \fixme Verify why MSE, PSNR, SSIM values are different on
/// ImageDiffTest::ComplexInequalityComparison from Ogre's implementation.
///
