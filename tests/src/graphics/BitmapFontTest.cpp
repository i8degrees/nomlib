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

// nom::VisualUnitTest framework
#include "nomlib/tests/VisualUnitTest.hpp"

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

namespace nom {

class BitmapFontTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    BitmapFontTest()
    {
      // NOM_LOG_TRACE( NOM );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame(0);
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~BitmapFontTest()
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

      // Text rendering defaults
      this->text =
        "!\"#$%&'()*+,-.\n/0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~";

      this->pt_size = nom::DEFAULT_FONT_SIZE;
      this->pos = Point2i(0,0);
      this->align = Anchor::MiddleCenter;
      this->color = Color4i::White;
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

    bool load_font( const std::string filename )
    {
      if( this->font.load(filename) == false ) {
        return false;
      }

      this->rendered_text.set_font(this->font);
      this->rendered_text.set_text(this->text);
      this->rendered_text.set_color(this->color);

      // Verify no ill effects from non-implemented features; text style and
      // text size is **not** implemented for bitmap fonts.
      this->rendered_text.set_style(this->style);
      this->rendered_text.set_text_size(this->pt_size);
      this->rendered_text.set_position(this->pos);

      this->append_update_callback( [=] (float) {
        nom::set_alignment( &this->rendered_text, Point2i(0, 0),
                            this->resolution(), this->align );
      });

      this->append_render_callback( [&] ( const RenderWindow& win ) {
        this->rendered_text.draw( this->render_window() );
      });

      return true;
    }

  protected:
    SearchPath resources;

    nom::Font font;
    nom::Text rendered_text;

    /// \brief The text to be rendered
    std::string text;

    /// \brief The point size of the text.
    int pt_size;

    /// \brief The text rendering position.
    Point2i pos;

    /// \brief The text alignment.
    uint32 align = Anchor::None;

    uint32 style = Text::Style::Normal;

    /// \brief The text color.
    Color4i color;
};

TEST_F(BitmapFontTest, VIII)
{
  std::string font =
    this->resources.path() + "VIII.png";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, VIII_small)
{
  std::string font =
    this->resources.path() + "VIII_small.png";

  this->text = "INFO.";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, VIII_cards)
{
  std::string font =
    this->resources.path() + "VIII_cards.png";

  this->text = "0123456789A";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, MagentaText)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->color = Color4i::Magenta;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, Kerning)
{
  int kerning_offset = 0;
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "WAV";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  kerning_offset = this->font->kerning(87, 65, this->pt_size);
  EXPECT_EQ(0, kerning_offset)
  << "nom::BitmapFont does not implemented kerning pair offsets???";

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, NoKerning)
{
  int kerning_offset = 0;
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "WAV";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  this->rendered_text.set_text_kerning(false);

  kerning_offset = this->font->kerning(87, 65, this->pt_size);
  EXPECT_EQ(0, kerning_offset)
  << "nom::BitmapFont does not implement kerning pair offsets!";

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, TopLeftAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::TopLeft;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, TopCenterAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::TopCenter;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, TopRightAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::TopRight;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, MiddleLeftAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::MiddleLeft;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, MiddleCenterAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::MiddleCenter;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, MiddleRightAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::MiddleRight;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, BottomLeftAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::BottomLeft;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, BottomCenterAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::BottomCenter;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(BitmapFontTest, BottomRightAlignment)
{
  std::string font =
    this->resources.path() + "VIII.png";

  this->text = "Hello, world!";
  this->align = Anchor::BottomRight;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

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
  // nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_TRACE_FONTS, nom::NOM_LOG_PRIORITY_VERBOSE );

  return RUN_ALL_TESTS();
}
