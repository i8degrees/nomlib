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

// nom::UnitTest framework
#include "nomlib/tests/common.hpp"

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

namespace nom {

class BMFontTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    BMFontTest()
    {
      // NOM_LOG_TRACE( NOM );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~BMFontTest()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      // NOM_LOG_TRACE( NOM );

      std::string res_file = nom::UnitTest::test_set() + ".json";

      if( resources.load_file( res_file, "resources" ) == false )
      {
        FAIL()
        << "Could not resolve the resource path from file: " << res_file;
      }

      // VisualUnitTest environment init
      VisualUnitTest::SetUp();

      // Used by nom::BMFont to determine the relative directory path to prepend
      // onto the FNT file's filename field to form a complete file path.
      nom::set_file_root( resources.path() );
    }

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase()
    {
      // NOM_LOG_TRACE( NOM );
    }

  protected:
    SearchPath resources;
};

TEST_F(BMFontTest, BasicParserSanity)
{
  BMFont font;
  Glyph g;
  std::string filename = resources.path() + "scoreboard.fnt";

  std::ifstream fp(filename);

  ASSERT_TRUE( font.parse_ascii_file(fp) )
  << "Could not load input font desc: " << filename;

  // Internal interface
  EXPECT_NE(nullptr, font.clone() );
  EXPECT_EQ(IFont::FontType::BMFont, font.type() );

  // Info tag
  EXPECT_EQ(96, font.point_size() );

  // Common tag
  EXPECT_EQ(114, font.newline(0) );
  EXPECT_EQ(114, font.metrics().newline);
  EXPECT_EQ(114, font.metrics().ascent);
  EXPECT_EQ(Size2i(1024,1024), font.page_size(0) );
  // EXPECT_EQ(1, font.pages_.size() );

  // First glyph (chars tag)
  g = font.glyph(32,0);
  EXPECT_EQ(IntRect(-1,1,0,0), g.bounds)
  << "Incorrect x, y, width or height for char";

  EXPECT_EQ(12, font.spacing(0) )
  << "Incorrect xadvance for char";

  // Rendering offset
  EXPECT_EQ( Point2i(0,0), g.offset )
  << "Incorrect xoffset or yoffset for char";

  // Second glyph (chars tag)
  g = font.glyph(48,0);
  EXPECT_EQ(IntRect(205,1,204,118), g.bounds)
  << "Incorrect x, y, width or height for char";

  EXPECT_EQ(12, font.spacing(0) )
  << "Incorrect xadvance for char";

  // Rendering offset
  EXPECT_EQ( Point2i(0,0), g.offset )
  << "Incorrect xoffset or yoffset for char";

  // Last glyph (chars tag)
  g = font.glyph(57,0);
  EXPECT_EQ(IntRect(411,241,135,118), g.bounds)
  << "Incorrect x, y, width or height for char";

  EXPECT_EQ(12, font.spacing(0) )
  << "Incorrect xadvance for char";

  // Rendering offset
  EXPECT_EQ( Point2i(0,0), g.offset )
  << "Incorrect xoffset or yoffset for char";
}

TEST_F(BMFontTest, KerningParserSanity)
{
  BMFont font;
  Glyph g;
  std::string filename = resources.path() + "gameover.fnt";

  std::ifstream fp(filename);

  ASSERT_TRUE( font.parse_ascii_file(fp) )
  << "Could not load input font file: " << filename;

  // First kerning pair
  EXPECT_EQ(-4, font.kerning(86,117,0) );

  // Second kerning pair
  EXPECT_EQ(-5, font.kerning(84,97,0) );

  // Next to last kerning pair
  EXPECT_EQ(-4, font.kerning(121,46,0) );

  // Last kerning pair
  EXPECT_EQ(-9, font.kerning(86,46,0) );
}

TEST_F(BMFontTest, RenderScoreboardFont)
{
  Glyph g;
  nom::Font font;

  Text rendered_text;
  std::string filename = resources.path() + "scoreboard.fnt";

  ASSERT_TRUE( font.load(filename) )
  << "Could not load input texture source: " << filename;

  rendered_text.set_font(font);
  rendered_text.set_text("7");

  // TTcards player 2 scoreboard origins (approximation)
  rendered_text.set_position( Point2i(64,0) );
  nom::set_alignment(&rendered_text, this->resolution(), Anchor::BottomLeft);
  // player1
  // rendered_text.set_position( Point2i(-64,0) );
  // nom::set_alignment(&rendered_text, this->resolution(), Anchor::BottomRight);

  EXPECT_EQ( Size2i(13,114), rendered_text.size() );

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    rendered_text.draw( this->render_window() );
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BMFontTest, RenderGameOverFont)
{
  nom::Font font;

  Text rendered_text;
  std::string filename = resources.path() + "gameover.fnt";

  ASSERT_TRUE( font.load(filename) )
  << "Could not load input texture source: " << filename;

  rendered_text.set_font(font);
  rendered_text.set_text("You Lose...");
  rendered_text.set_position( Point2i(0,0) );
  nom::set_alignment(&rendered_text, this->resolution(), Anchor::MiddleCenter);

  EXPECT_EQ( Size2i(340,90), rendered_text.size() );
  EXPECT_EQ( 90, font->newline(0) )
  << "Text line spacing (newline) should be the same as the 'lineHeight' field.";

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    rendered_text.draw( this->render_window() );
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BMFontTest, RenderMultipleLines)
{
  nom::Font font;

  Text rendered_text;
  std::string filename = resources.path() + "gameover.fnt";

  ASSERT_TRUE( font.load(filename) )
  << "Could not load input texture source: " << filename;

  rendered_text.set_font(font);
  rendered_text.set_text("You Lose...\nYou Win!\nDraw");
  rendered_text.set_position( Point2i(0,0) );
  nom::set_alignment(&rendered_text, this->resolution(), Anchor::MiddleCenter);

  EXPECT_EQ( Size2i(340,270), rendered_text.size() );
  EXPECT_EQ( 90, font->newline(0) )
  << "Text line spacing (newline) should be the same as the 'lineHeight' field.";

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    rendered_text.draw( this->render_window() );
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BMFontTest, Kerning)
{
  int kerning_offset = 0;
  nom::Font font;

  Text rendered_text;
  std::string filename = resources.path() + "gameover.fnt";

  ASSERT_TRUE( font.load(filename) )
  << "Could not load input texture source: " << filename;

  rendered_text.set_font(font);
  rendered_text.set_text("WAV");
  rendered_text.set_position( Point2i(0,0) );
  nom::set_alignment(&rendered_text, this->resolution(), Anchor::MiddleCenter);

  kerning_offset = font->kerning(87, 65, 0);
  EXPECT_EQ(-7, kerning_offset);

  EXPECT_EQ( Size2i(159,90), rendered_text.size() );
  EXPECT_EQ( 90, font->newline(0) )
  << "Text line spacing (newline) should be the same as the 'lineHeight' field.";

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    rendered_text.draw( this->render_window() );
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BMFontTest, NoKerning)
{
  int kerning_offset = 0;
  nom::Font font;

  Text rendered_text;
  std::string filename = resources.path() + "gameover.fnt";

  ASSERT_TRUE( font.load(filename) )
  << "Could not load input texture source: " << filename;

  rendered_text.set_font(font);
  rendered_text.set_text("WAV");
  rendered_text.set_position( Point2i(0,0) );
  nom::set_alignment(&rendered_text, this->resolution(), Anchor::MiddleCenter);

  font->set_font_kerning(false);

  kerning_offset = font->kerning(87, 65, 0);
  EXPECT_EQ(0, kerning_offset);

  EXPECT_EQ( Size2i(159,90), rendered_text.size() );
  EXPECT_EQ( 90, font->newline(0) )
  << "Text line spacing (newline) should be the same as the 'lineHeight' field.";

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    rendered_text.draw( this->render_window() );
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

/// \remarks The default settings of [BMFont v1.14b](http://www.angelcode.com/products/bmfont/)
/// were used to export the files used in this test.
TEST_F(BMFontTest, RenderAngelCodeBMFontExport)
{
  nom::Font font;
  Text rendered_text;

  std::string filename = resources.path() + "arial.fnt";

  ASSERT_TRUE( font.load(filename) )
  << "Could not load input texture source: " << filename;

  rendered_text.set_font(font);
  rendered_text.set_text("Hello, World!");
  rendered_text.set_position( Point2i(0,0) );
  nom::set_alignment(&rendered_text, this->resolution(), Anchor::MiddleCenter);

  EXPECT_EQ( Size2i(169,32), rendered_text.size() );
  EXPECT_EQ( 32, font->newline(0) )
  << "Text line spacing (newline) should be the same as the 'lineHeight' field.";

  this->append_render_callback( [&] ( const RenderWindow& win ) {
    rendered_text.draw( this->render_window() );
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init( argc, argv ) == false )
  {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  // nom::UnitTest framework integration
  nom::init_test( argc, argv );

  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_RENDER, nom::NOM_LOG_PRIORITY_VERBOSE );
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
