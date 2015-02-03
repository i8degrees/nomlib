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
#include <string>

// nom::VisualUnitTest framework
#include "nomlib/tests/VisualUnitTest.hpp"

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/graphics.hpp>

namespace nom {

class SpriteTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    SpriteTest()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );

      this->set_window_flags(SDL_WINDOW_RESIZABLE);

      if( nom::set_hint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest") == false ) {
        NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                      "Failed to set render scale quality to", "nearest" );
      }

      // The frame image to compare against the reference image set
      this->append_screenshot_frame(0);
    }

    /// \remarks This method is called at the end of each unit test.
    virtual ~SpriteTest()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_UNIT_TEST,
                          NOM_LOG_PRIORITY_VERBOSE );

      std::string res_file = nom::UnitTest::test_set() + ".json";

      // Determine our resources path based on several possible locations;
      // this is dependent upon the build environment
      if( resources[0].load_file( res_file, "resources" ) == false ) {
        FAIL()
        << "Could not resolve the resource path from file: " << res_file;
      }

      if( resources[1].load_file( res_file, "common" ) == false ) {
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

    struct sprite_test
    {
      // input
      std::string sprite_tex_path;
      Point2i pos = Point2i::zero;
      Texture::Access tex_type;

      // output
      Texture tex;
      Sprite sprite;
    };

    void init_sprite_test(sprite_test& params)
    {
      if( params.tex.load(params.sprite_tex_path, false,
          params.tex_type) == false )
      {
        FAIL()  << "Could not load the sprite texture from: "
                << params.sprite_tex_path;
      }

      EXPECT_EQ(true, params.sprite.set_texture(params.tex) );
      ASSERT_TRUE(params.sprite.valid() == true);

      nom::set_alignment( &params.sprite, params.pos, WINDOW_DIMS,
                          Anchor::MiddleCenter );

      this->append_render_callback( [=, &params](const RenderWindow& win) {

        ASSERT_TRUE(params.sprite.valid() == true);
        if( params.sprite.valid() ) {
          params.sprite.draw( this->render_window() );
        }
      });
    }

  protected:
    const Size2i WINDOW_DIMS = Size2i( this->resolution() );
    SearchPath resources[2];
};

TEST_F(SpriteTest, SpriteInterfaceWithTextureReference)
{
  sprite_test params;
  params.sprite_tex_path =
    this->resources[0].path() + "card.png";
  params.tex_type = Texture::Access::Streaming;

  this->init_sprite_test(params);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(SpriteTest, SpriteInterfaceWithTextureRawPointer)
{
  const std::string TEX_FILE_PATH =
    this->resources[0].path() + "card.png";
  const Point2i SPRITE_POS(Point2i::zero);

  Texture* tex = new Texture();
  nom::Sprite* sprite = new Sprite();
  ASSERT_TRUE(tex != nullptr);
  ASSERT_TRUE(sprite != nullptr);

  if( tex->load(TEX_FILE_PATH, false,
      nom::Texture::Access::Streaming) == false )
  {
    FAIL() << "Could not load the sprite texture from: " << TEX_FILE_PATH;
  }
  ASSERT_TRUE(tex->valid() == true);

  EXPECT_EQ(true, sprite->set_texture(tex) );
  ASSERT_TRUE(sprite->valid() == true);

  nom::set_alignment(sprite, SPRITE_POS, WINDOW_DIMS, Anchor::MiddleCenter);

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {
    if( sprite->valid() ) {
      sprite->draw( this->render_window() );
    }
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(SpriteTest, SpriteInterfaceWithTextureRawPointerAsReference)
{
  const std::string TEX_FILE_PATH =
    this->resources[0].path() + "card.png";
  const Point2i SPRITE_POS(Point2i::zero);

  Texture* tex = new Texture();
  nom::Sprite* sprite = new Sprite();
  ASSERT_TRUE(tex != nullptr);
  ASSERT_TRUE(sprite != nullptr);

  if( tex->load(TEX_FILE_PATH, false,
      nom::Texture::Access::Streaming) == false )
  {
    FAIL() << "Could not load the sprite texture from: " << TEX_FILE_PATH;
  }
  ASSERT_TRUE(tex->valid() == true);

  EXPECT_EQ(true, sprite->set_texture(*tex) );
  ASSERT_TRUE(sprite->valid() == true);

  nom::set_alignment(sprite, SPRITE_POS, WINDOW_DIMS, Anchor::MiddleCenter);

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {
    if( sprite->valid() ) {
      sprite->draw( this->render_window() );
    }
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );

  NOM_DELETE_PTR(sprite);
  NOM_DELETE_PTR(tex);
}

TEST_F(SpriteTest, SpriteInterfaceWithTextureUniquePointer)
{
  const std::string TEX_FILE_PATH =
    this->resources[0].path() + "card.png";
  const Point2i SPRITE_POS(Point2i::zero);

  std::unique_ptr<Texture> tex;
  tex.reset( new Texture() );

  std::unique_ptr<Sprite> sprite;
  sprite.reset( new Sprite() );

  ASSERT_TRUE(tex != nullptr);
  ASSERT_TRUE(sprite != nullptr);

  if( tex->load(TEX_FILE_PATH, false,
      nom::Texture::Access::Streaming) == false )
  {
    FAIL() << "Could not load the sprite texture from: " << TEX_FILE_PATH;
  }
  ASSERT_TRUE(tex->valid() == true);

  EXPECT_EQ(true, sprite->set_texture( *tex.get() ) );
  ASSERT_TRUE(sprite->valid() == true);

  nom::set_alignment( sprite.get(), SPRITE_POS, WINDOW_DIMS,
                      Anchor::MiddleCenter );

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {
    if( sprite->valid() ) {
      sprite->draw( this->render_window() );
    }
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(SpriteTest, SpriteInterfaceWithTextureSharedPointer)
{
  const std::string TEX_FILE_PATH =
    this->resources[0].path() + "card.png";
  const Point2i SPRITE_POS(Point2i::zero);

  std::shared_ptr<Texture> tex;
  tex.reset( new Texture() );

  std::shared_ptr<Sprite> sprite;
  sprite.reset( new Sprite() );

  ASSERT_TRUE(tex != nullptr);
  ASSERT_TRUE(sprite != nullptr);

  if( tex->load(TEX_FILE_PATH, false,
      nom::Texture::Access::Streaming) == false )
  {
    FAIL() << "Could not load the sprite texture from: " << TEX_FILE_PATH;
  }
  ASSERT_TRUE(tex->valid() == true);

  EXPECT_EQ(true, sprite->set_texture(tex) );
  ASSERT_TRUE(sprite->valid() == true);

  nom::set_alignment(sprite.get(), SPRITE_POS, WINDOW_DIMS, Anchor::MiddleCenter);

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {
    if( sprite->valid() ) {
      sprite->draw( this->render_window() );
    }
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(SpriteTest, SpriteInterfaceSetAlpha)
{
  sprite_test params;
  params.sprite_tex_path = this->resources[0].path() + "card.png";
  params.tex_type = Texture::Access::Streaming;

  this->init_sprite_test(params);

  EXPECT_EQ(true, params.sprite.set_alpha(128) );

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(SpriteTest, SpriteInterfaceSetColor)
{
  sprite_test params;
  params.sprite_tex_path = this->resources[0].path() + "card.png";
  params.tex_type = Texture::Access::Streaming;

  this->init_sprite_test(params);

  params.sprite.set_color(Color4i::Magenta);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
  EXPECT_TRUE( this->compare() );
}

TEST_F(SpriteTest, SpriteBatchInterface)
{
  const std::string TEX_FILE_PATH =
    this->resources[1].path() + "cursors.png";
  const std::string SHEET_FILE_PATH =
    this->resources[1].path() + "cursors.json";
  const Point2i SPRITE_POS(Point2i::zero);

  SpriteSheet sprite_frames;
  Texture tex;
  SpriteBatch sprite;

  if( sprite_frames.load_file(SHEET_FILE_PATH) == false ) {
    FAIL() << "Could not load the sprite sheet from: " << SHEET_FILE_PATH;
  }

  if( tex.load(TEX_FILE_PATH, false,
      nom::Texture::Access::Streaming) == false )
  {
    FAIL() << "Could not load the sprite texture from: " << TEX_FILE_PATH;
  }
  ASSERT_TRUE(tex.valid() == true);

  EXPECT_EQ(true, sprite.set_texture(tex) );
  ASSERT_TRUE(sprite.valid() == true);

  sprite.set_sprite_sheet(sprite_frames);
  sprite.set_frame(3); // Right-pointing cursor hand

  nom::set_alignment(&sprite, SPRITE_POS, WINDOW_DIMS, Anchor::MiddleCenter);

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {
    if( sprite.valid() ) {
      sprite.draw( this->render_window() );
    }
  });

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
