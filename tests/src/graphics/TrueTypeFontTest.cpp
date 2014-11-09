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

class TrueTypeFontTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    TrueTypeFontTest()
    {
      // NOM_LOG_TRACE( NOM );

      // The frame image to compare against the reference image set
      this->append_screenshot_frame( 0 );
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~TrueTypeFontTest()
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
        "!\"#$%&'()*+,-.\n//0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n[\\]^_`\nabcdefghijklmnopqrstuvwxyz\n{|}~";

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
      this->rendered_text.set_style(this->style);
      this->rendered_text.set_text_size(this->pt_size);
      this->rendered_text.set_color(this->color);

      // FIXME: Proper multi-line alignment logic is not implemented
      this->rendered_text.set_position(this->pos);
      nom::set_alignment( &this->rendered_text,
                          this->resolution(), this->align);

      this->append_render_callback( [&] ( const RenderWindow& win ) {
        this->rendered_text.draw( this->render_window() );
      });

      return true;
    }

  protected:
    SearchPath resources;

    // nom::TrueTypeFont font;
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

TEST_F(TrueTypeFontTest, OpenSansRegular)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, OpenSansBold)
{
  std::string font =
    this->resources.path() + "OpenSans-Bold.ttf";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, LiberationSerif)
{
  std::string font =
    this->resources.path() + "LiberationSerif-Regular.ttf";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, LiberationSerifBold)
{
  std::string font =
    this->resources.path() + "LiberationSerif-Bold.ttf";

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, MagentaText)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->color = Color4i::Magenta;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, Kerning)
{
  int kerning_offset = 0;
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "WAV";
  this->pt_size = 96;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  kerning_offset = this->font->kerning(87, 65, this->pt_size);
  EXPECT_EQ(-4, kerning_offset)
  << "Are you not using a build of SDL2_ttf with the kerning pairs patch "
  << "applied? See third-party/README.md for details.";

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, NoKerning)
{
  int kerning_offset = 0;
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "WAV";
  this->pt_size = 96;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  this->font->set_font_kerning(false);

  kerning_offset = this->font->kerning(87, 65, this->pt_size);
  EXPECT_EQ(0, kerning_offset);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, TopLeftAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::TopLeft;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, TopCenterAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::TopCenter;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, TopRightAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::TopRight;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, MiddleLeftAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::MiddleLeft;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, MiddleCenterAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::MiddleCenter;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, MiddleRightAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::MiddleRight;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, BottomLeftAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::BottomLeft;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, BottomCenterAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::BottomCenter;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, BottomRightAlignment)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = 48;
  this->align = Anchor::BottomRight;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, Bold)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->style = Text::Style::Bold;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, Italic)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->style = Text::Style::Italic;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, Underline)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->style = Text::Style::Underline;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, Strikethrough)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->style = Text::Style::Strikethrough;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(TrueTypeFontTest, UseAllTextStyles)
{
  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->style =
    Text::Style::Bold | Text::Style::Italic | Text::Style::Underline |
    Text::Style::Strikethrough;

  EXPECT_EQ(true, this->load_font(font) )
  << "Could not load font file: " << font;

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

/// \brief An interactive test for variable point size rendering.
///
/// \remarks This is a test to help determine efficiency of the glyph cache
/// when rendering text with many different point sizes. Large memory
/// allocations (i.e.: generation of a glyph page) should not occur after the
/// initial caching -- the two things to look out for: a) large FPS drop;
/// b) stuttering.
///
/// \note Use your mouse wheel's Y axis -- up and down -- to zoom in and out.
TEST_F(TrueTypeFontTest, InteractiveGlyphCache)
{
  const int MIN_POINT_SIZE = 9;
  const int MAX_POINT_SIZE = 72;

  std::string font =
    this->resources.path() + "OpenSans-Regular.ttf";

  this->text = "Hello, world!";
  this->pt_size = MIN_POINT_SIZE;
  this->align = Anchor::MiddleCenter;

  ASSERT_TRUE(this->load_font(font) == true)
  << "Could not load font file: " << font;

  // Cache the glyphs of the font's point size range that we wish to test for
  for( auto idx = MIN_POINT_SIZE - 1; idx != MAX_POINT_SIZE; ++idx ) {
    this->rendered_text.set_text_size(idx + 1);
  }

  this->rendered_text.set_text_size(nom::DEFAULT_FONT_SIZE);

  // Register additional input bindings
  InputActionMapper wheel;

  EventCallback zoom_in( [&] (const Event& evt) {
    int current_size = this->rendered_text.text_size();

    if( current_size < MAX_POINT_SIZE ) {
      this->rendered_text.set_text_size(current_size += 1);

      // Reset alignment calcs
      nom::set_alignment( &this->rendered_text,
                          Size2i( this->resolution() ),
                          Anchor::None );

      nom::set_alignment( &this->rendered_text,
                          Size2i( this->resolution() ),
                          this->align );
    }
  });

  EventCallback zoom_out( [&] (const Event& evt) {
    int current_size = this->rendered_text.text_size();

    if( current_size >= MIN_POINT_SIZE ) {
      this->rendered_text.set_text_size(current_size -= 1);

      // Reset alignment calcs
      nom::set_alignment( &this->rendered_text,
                          Size2i( this->resolution() ),
                          Anchor::None );

      nom::set_alignment( &this->rendered_text,
                          Size2i( this->resolution() ),
                          this->align );
    }
  });

  wheel.insert( "zoom_in",
                 MouseWheelAction(  SDL_MOUSEWHEEL,
                                    MouseWheelAction::AXIS_Y,
                                    MouseWheelAction::UP ), zoom_in );

  wheel.insert( "zoom_out",
                 MouseWheelAction(  SDL_MOUSEWHEEL,
                                    MouseWheelAction::AXIS_Y,
                                    MouseWheelAction::DOWN ), zoom_out );

  this->input_mapper_.insert("zoom_in", wheel, true);
  this->input_mapper_.insert("zoom_out", wheel, true);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  // EXPECT_TRUE( this->compare() );
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
