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
#ifndef NOMLIB_TESTS_COMMON_VISUAL_UNIT_TEST_RESULT_WRITER_HPP
#define NOMLIB_TESTS_COMMON_VISUAL_UNIT_TEST_RESULT_WRITER_HPP

#include <string>

#include "gtest/gtest.h"

namespace nom {

class VisualUnitTestResultWriter: public ::testing::EmptyTestEventListener
{
  public:
    static void initialize  (
                              const std::string& ref_path,
                              const std::string& output_path
                            );

  private:
    /// \brief Ensure one-time initialization of the input & output directories.
    ///
    /// \remarks Ensure that the directory paths stay in sync across unit test
    /// runs.
    static bool initialized_;

    static std::string ref_dir_;
    static std::string output_dir_;

    /// \remarks This method is called after the test case ends.
    virtual void OnTestCaseEnd( const ::testing::TestCase& test_info );
};

} // namespace nom

#endif // include guard defined

/// \class nom::VisualUnitTestResultWriter
/// \ingroup tests
///
/// HTML test result output for nom::VisualUnitTest using Google Test's
/// event listener framework.
///
/// \see Adapted from the implementation for [Ogre3D](http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework) by Riley Adams <praetor57@gmail.com>.
///
