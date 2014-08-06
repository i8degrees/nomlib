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
#include <vector>
#include <array>
#include <fstream>

#include "gtest/gtest.h"

#include "nomlib/core.hpp"

using namespace nom;

static const int NUM_OUTPUT_LINES = 6;

static const std::string out_texts[NUM_OUTPUT_LINES] =
{
  "...A hello, world in BLUE colored text!\n\n",
  "Another (YELLOW) colored output string -- oh my!\n\n",
  "Bitchin' ",
  "BOLD & UNDERLINED styled text,",
  " if your term settings supports it :]\n"
};

static const std::string output_file = "FileOutputStreamCtor.txt";

void output_texts( ConsoleOutput& cons, std::ostream& os )
{
  cons.set_color( ConsoleOutput::FG_BLUE );
  os << out_texts[0];

  cons.set_color( ConsoleOutput::FG_YELLOW );
  os << out_texts[1];

  cons.set_color( ConsoleOutput::Color::FG_RED );
  os << out_texts[2];

  cons.set_style( ConsoleOutput::STYLE_BOLD | ConsoleOutput::STYLE_UNDERLINED );
  cons.set_color( ConsoleOutput::Color::FG_YELLOW );
  os << out_texts[3];

  // cons.endl();
  cons.set_style( 0 );

  cons.set_color( ConsoleOutput::Color::FG_BLUE );
  os << out_texts[4];

  // return os;
}

TEST( ConsoleOutputTest, DefaultCtor )
{
  nom::ConsoleOutput cons;
  // nom::ConsoleOutput cons( std::cerr );

 output_texts( cons, std::cerr );

FAIL()  << "FIXME: The default constructed object displays non-colored texts,"
        << "whereas when we explicitly pass a std::cerr or std::cout"
        << "object to it, the colored texts begin working...";
}

TEST( ConsoleOutputTest, StandardOutputStreamCtor )
{
  nom::ConsoleOutput cons( std::cout );

  output_texts( cons, std::cout );
}

/// \remarks The resulting output file should *not* have the terminal color
/// codes in it; it should be three lines of plain-text.
TEST( ConsoleOutputTest, FileOutputStreamCtor )
{
  std::stringstream buffer;
  std::string append_buffer;

  std::ofstream fp_out( output_file );
  nom::ConsoleOutput cons( fp_out );

  output_texts( cons, fp_out );

  fp_out.close();

  cons.endl();

  std::ifstream fp_in( output_file );

  buffer << fp_in.rdbuf();

  for( auto idx = 0; idx != NUM_OUTPUT_LINES; ++idx )
  {
    append_buffer.append( out_texts[idx] );
  }

  EXPECT_EQ( buffer.str(), append_buffer );

  fp_in.close();
}

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Note that SDL2 does not need to be initialized before using the logging
  // facilities!

  return RUN_ALL_TESTS();
}
