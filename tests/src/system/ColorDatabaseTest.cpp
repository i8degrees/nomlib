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
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/system.hpp>

namespace nom {

class ColorDatabaseTest: public ::testing::Test
{
  public:
    ColorDatabaseTest( void )
    {
      // ...
    }

    ~ColorDatabaseTest( void )
    {
      // ...
    }

  protected:
    ColorDatabase colors;
};

TEST_F( ColorDatabaseTest, CoreAPI )
{
  this->colors.append_color( "Black", Color4i( 0, 0, 0, 255 ) );
  this->colors.append_color( "White", Color4i( 255, 255, 255, 255 ) );
  this->colors.append_color( "Red", Color4i( 255, 0, 0, 255 ) );
  this->colors.append_color( "Green", Color4i( 0, 255, 0, 255 ) );
  this->colors.append_color( "Blue", Color4i( 0, 0, 255, 255 ) );
  this->colors.append_color( "Yellow", Color4i( 255, 255, 0 ) );
  this->colors.append_color( "Magenta", Color4i( 255, 0, 255 ) );
  this->colors.append_color( "Cyan", Color4i( 0, 255, 255, 255 ) );
  this->colors.append_color( "Silver", Color4i( 192, 192, 192, 255 ) );
  this->colors.append_color( "Purple", Color4i( 128, 0, 128, 255 ) );
  this->colors.append_color( "Orange", Color4i( 255, 165, 0, 255 ) );
  this->colors.append_color( "LightGray", Color4i( 99, 99, 99, 255 ) );
  this->colors.append_color( "Gray", Color4i( 67, 67, 67, 255 ) );
  this->colors.append_color( "SkyBlue", Color4i( 110, 144, 190, 255 ) );

  ASSERT_TRUE( this->colors.find_color( "Black" ) == Color4i( 0, 0, 0, 255 ) );
  ASSERT_TRUE( this->colors.find_color( "White" ) == Color4i( 255, 255, 255, 255 ) );
  ASSERT_TRUE( this->colors.find_color( "SkyBlue" ) == Color4i( 110, 144, 190, 255 ) );

  this->colors.erase_color( "Black" );
  this->colors.erase_color( "White" );
  this->colors.erase_color( "SkyBlue" );

  ASSERT_FALSE( this->colors.find_color( "Black" ) == Color4i( 0, 0, 0, 255 ) );
  ASSERT_FALSE( this->colors.find_color( "White" ) == Color4i( 255, 255, 255, 255 ) );
  ASSERT_FALSE( this->colors.find_color( "SkyBlue" ) == Color4i( 110, 144, 190, 255 ) );
}

TEST_F( ColorDatabaseTest, SystemColorsAPI )
{
  // SystemColors::initialize();

  ASSERT_TRUE( SystemColors::colors().find_color( "Black" ) == Color4i( 0, 0, 0, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "White" ) == Color4i( 255, 255, 255, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Red" ) == Color4i( 255, 0, 0, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Green" ) == Color4i( 0, 255, 0, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Blue" ) == Color4i( 0, 0, 255, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Yellow" ) == Color4i( 255, 255, 0 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Magenta" ) == Color4i( 255, 0, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Cyan" ) == Color4i( 0, 255, 255, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Silver" ) == Color4i( 192, 192, 192, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Purple" ) == Color4i( 128, 0, 128, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Orange" ) == Color4i( 255, 165, 0, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "LightGray" ) == Color4i( 99, 99, 99, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "Gray" ) == Color4i( 67, 67, 67, 255 ) );
  ASSERT_TRUE( SystemColors::colors().find_color( "SkyBlue" ) == Color4i( 110, 144, 190, 255 ) );

  SystemColors::colors().erase_color( "Black" );
  SystemColors::colors().erase_color( "White" );
  SystemColors::colors().erase_color( "SkyBlue" );

  ASSERT_TRUE( SystemColors::colors().find_color( "Black" ) == Color4i::null );
  ASSERT_TRUE( SystemColors::colors().find_color( "White" ) == Color4i::null );
  ASSERT_TRUE( SystemColors::colors().find_color( "SkyBlue" ) == Color4i::null );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
