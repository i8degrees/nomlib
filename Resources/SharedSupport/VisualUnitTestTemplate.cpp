/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include <string>

// nom::VisualUnitTest framework
#include "nomlib/tests/VisualUnitTest.hpp"

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/serializers.hpp>
#include <nomlib/graphics.hpp>

namespace nom {

class VisualUnitTestTemplate: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    VisualUnitTestTemplate()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame(0);
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~VisualUnitTestTemplate()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );
    }

    // virtual bool init_rendering()
    // {
    //   // Initialize rendering window (and its GL context)
    //   if( this->window_.create( this->test_set(),
    //       this->resolution(), this->window_flags() ) == false )
    //   {
    //     return false;
    //   }

    //   // Allow for automatic rescaling of the output window based on aspect
    //   // ratio (i.e.: handle fullscreen resizing); this will use letterboxing
    //   // when the aspect ratio is greater than what is available, or side-bars
    //   // when the aspect ratio is less than.
    //   this->render_window().set_logical_size( this->resolution() );

    //   // Use no pixel unit scaling; this gives us one to one pixel ratio
    //   this->render_window().set_scale( nom::Point2f(1,1) );

    //   return true;
    // }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );

      std::string res_file = nom::UnitTest::test_set() + ".json";

      if( resources.load_file( res_file, "resources" ) == false )
      {
        FAIL()
        << "Could not resolve the resource path from file: " << res_file;
      }

      // VisualUnitTest environment init
      VisualUnitTest::SetUp();
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );
    }

  protected:
    SearchPath resources;
};

TEST_F(VisualUnitTestTemplate, Test)
{
  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

} // namespace nom

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init(argc, argv) == false )
  {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return NOM_EXIT_FAILURE;
  }
  atexit( nom::quit );

  // nom::UnitTest framework integration
  nom::init_test(argc, argv);

  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST, NOM_LOG_PRIORITY_CRITICAL);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST, NOM_LOG_PRIORITY_DEBUG);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_APPLICATION, nom::NOM_LOG_PRIORITY_CRITICAL);

  return RUN_ALL_TESTS();
}
