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
      // ...
    }

    ~RenderWindowTest( void )
    {
      // ...
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
      // Nothing to manually destruct
    }

  protected:
    nom::RenderWindow window;
};

TEST_F( RenderWindowTest, CreateWindow )
{
  // Only requirement is that we pass ::SetUp.
}

/// \fixme Unit test fails if we do not execute from the path of the program
/// executable (where icon.png is expected to be found).
TEST_F( RenderWindowTest, SetIcon )
{
  Path p;
  File fp;
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

  return RUN_ALL_TESTS();
}
