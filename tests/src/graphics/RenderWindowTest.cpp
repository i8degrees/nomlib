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
#include "gtest/gtest.h"

#include <nomlib/graphics.hpp>
#include <nomlib/system.hpp>

namespace nom {

class RenderWindowTest: public ::testing::Test
{
  public:
    RenderWindowTest( void )
    {
      // FIXME: I get "Invalid renderer" err logging from SDL2 when we enable
      // its logging.
      // This could possibly be the cause of the issue that causes test failures
      // under WindowsOS within RenderWindowTest::SetIcon?
      // nom::SDL2Logger::set_logging_priorities( SDL_LOG_PRIORITY_DEBUG );
    }

    virtual ~RenderWindowTest( void )
    {
      // Nothing to clean up
    }

    virtual void SetUp( void )
    {
      const int WINDOW_WIDTH = 320;
      const int WINDOW_HEIGHT = 240;
      // uint32 wflags = SDL_WINDOW_SHOWN;
      uint32 wflags = SDL_WINDOW_HIDDEN;

      ASSERT_TRUE( this->window.create( "RenderWindowTest", WINDOW_WIDTH, WINDOW_HEIGHT, wflags ) == true );
    }

    virtual void TearDown( void )
    {
      // Nothing to clean up
    }

  protected:
    nom::RenderWindow window;
};

TEST_F( RenderWindowTest, CreateWindow )
{
  // The render window is created during ::SetUp at the start of each test
}

TEST_F( RenderWindowTest, SetIcon )
{
  Path p;
  File fp;

  NOM_DUMP_VAR( NOM_LOG_CATEGORY_TEST, "current directory path:", fp.currentPath() );

  std::string icon_path = fp.currentPath() + p.native() + "icon.png";
  // p = fp.resource_path( "org.i8degrees.nomlib" );
  // std::string icon_path = p.prepend("icon.png");

  ASSERT_TRUE( this->window.set_window_icon( icon_path ) == true );
}

TEST_F( RenderWindowTest, SetWindowTitle )
{
  this->window.set_window_title( "SetWindowTitle" );
  EXPECT_EQ( "SetWindowTitle", this->window.window_title() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  NOM_ASSERT( nom::init( argc, argv ) == true );

  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
