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
#include "nomlib/tests/actions/ActionTest.hpp"

namespace nom {

TEST_F(ActionTest, FadeInAction)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_TRANSPARENT);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto fade_in =
    nom::create_action<FadeInAction>(sprite, DURATION);
  ASSERT_TRUE(fade_in != nullptr);

  fade_in->set_name("fade_in");

  auto action0 =
    nom::create_action<GroupAction>( {fade_in}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_in_params( fade_in.get(), Color4i::ALPHA_OPAQUE,
                                    sprite.get() );

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_in.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {

    // Render our animation's texture
    if( sprite != nullptr && sprite->valid() == true ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeInActionFromNonTransparentOpacity)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE/2);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto fade_in =
    nom::create_action<FadeInAction>(sprite, DURATION);
  ASSERT_TRUE(fade_in != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_in}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_in_params( fade_in.get(), Color4i::ALPHA_OPAQUE,
                                    sprite.get() );

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_in.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {

    // Render our animation's texture
    if( sprite != nullptr && sprite->valid() == true ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeOutAction)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto fade_out =
    nom::create_action<FadeOutAction>(sprite, DURATION);
  ASSERT_TRUE(fade_out != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_out}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_alpha_out_params(  fade_out.get(), Color4i::ALPHA_TRANSPARENT,
                                      sprite.get() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_out.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";

  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {

    // Render our animation's texture
    if( sprite != nullptr && sprite->valid() == true ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeOutActionFromNonOpaqueOpacity)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto rect =
    std::make_shared<Rectangle>(IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE/2);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto fade_out =
    nom::create_action<FadeOutAction>(sprite, DURATION);
  ASSERT_TRUE(fade_out != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_out}, "action0");
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_out_params(  fade_out.get(), Color4i::ALPHA_OPAQUE,
                                      sprite.get() );

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_out.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {

    // Render our animation's texture
    if( sprite != nullptr && sprite->valid() == true ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \brief Fade in, from an alpha value of zero to an alpha value of 129-1 a
/// texture of a blue block.
TEST_F(ActionTest, FadeAlphaByAction)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 FADE_BY = 129;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_TRANSPARENT);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(sprite, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_by_params( fade_by.get(), FADE_BY, sprite.get() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_by.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {

    // Render our animation's texture
    if( sprite != nullptr && sprite->valid() == true ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeAlphaByActionFromNonOpaqueOpacity)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 FADE_BY = 129;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE/4);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(sprite, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_by_params( fade_by.get(), FADE_BY, sprite.get() );

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_by.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {

    // Render our animation's texture
    if( sprite != nullptr && sprite->valid() == true ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeAlphaByActionWithNegativeValue)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const int16 FADE_BY = -129;
  const int16 EXPECTED_ALPHA = 126;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(sprite, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_alpha_by_params( fade_by.get(), EXPECTED_ALPHA,
                                    sprite.get() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_by.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {

    // Render our animation's texture
    if( sprite != nullptr && sprite->valid() == true ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, AlphaBlendingDemo)
{
  // Testing parameters
  const float DURATION = 2.5f;  // 5s total duration due to x2 sequences
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(256, 256);

  auto bg_tex = std::make_shared<Texture>();
  ASSERT_TRUE(bg_tex != nullptr);
  if( bg_tex->load( resources[0].path() + "backdrop.png" ) == false ) {
    FAIL()  << "Could not load 'backdrop.png' input file from "
            << resources[0].path();
  }

  bg_tex->set_alpha(Color4i::ALPHA_OPAQUE);

  // Stretched dimensions to cover entire window
  bg_tex->set_size(WINDOW_DIMS);

  auto bg_sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(bg_sprite != nullptr);
  EXPECT_EQ(true, bg_sprite->set_texture(bg_tex) );

  auto rect0 =
    std::make_shared<Rectangle>(  IntRect(RECT_POS, RECT_SIZE),
                                  Color4i::Magenta );
  ASSERT_TRUE(rect0 != nullptr);

  auto rect1 =
    std::make_shared<Rectangle>(  IntRect(RECT_POS, RECT_SIZE),
                                  Color4i::Blue );
  ASSERT_TRUE(rect1 != nullptr);

  auto rect_tex0 =
    std::shared_ptr<Texture>( rect0->texture() );
  ASSERT_TRUE(rect_tex0 != nullptr);

  auto rect_tex1 =
    std::shared_ptr<Texture>( rect1->texture() );
  ASSERT_TRUE(rect_tex1 != nullptr);

  // Initialize texture with its initial alpha value for blending test
  rect_tex0->set_alpha(Color4i::ALPHA_TRANSPARENT);
  rect_tex0->set_blend_mode(SDL_BLENDMODE_BLEND);

  rect_tex1->set_alpha(Color4i::ALPHA_OPAQUE);
  rect_tex1->set_blend_mode(SDL_BLENDMODE_BLEND);

  // magenta rect
  auto sprite0 =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite0 != nullptr);
  EXPECT_EQ(true, sprite0->set_texture(rect_tex0) );

  // blue rect
  auto sprite1 =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite1 != nullptr);
  EXPECT_EQ(true, sprite1->set_texture(rect_tex1) );

  auto fade_bg_tex_out =
    nom::create_action<FadeOutAction>(bg_sprite, DURATION);
  ASSERT_TRUE(fade_bg_tex_out != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_bg_tex_out}, "bg_layer");
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  auto fade_rect0_in =
    nom::create_action<FadeInAction>(sprite0, DURATION);
  ASSERT_TRUE(fade_rect0_in != nullptr);

  auto fade_rect1_out =
    nom::create_action<FadeOutAction>(sprite1, DURATION);
  ASSERT_TRUE(fade_rect1_out != nullptr);

  auto action1 =
    nom::create_action<SequenceAction>( {
      fade_rect0_in, fade_rect1_out}, "fade_rect0_in, fade_rect1_out" );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_curve(TIMING_MODE);
  action1->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(2, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(fade_bg_tex_out.get(), DURATION, SPEED_MOD);

    this->expected_alpha_out_params(  fade_bg_tex_out.get(),
                                      Color4i::ALPHA_TRANSPARENT,
                                      bg_sprite.get(), "fade_bg_sprite_out" );
    this->expected_alpha_in_params( fade_rect0_in.get(),
                                    Color4i::ALPHA_OPAQUE,
                                    sprite0.get(), "fade_sprite0_in" );
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to enqueue fade_blue_rect!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action1.get(), 2);
    this->expected_common_params(fade_rect0_in.get(), DURATION, SPEED_MOD);
    this->expected_common_params(fade_rect1_out.get(), DURATION, SPEED_MOD);

    this->expected_alpha_out_params(  fade_rect1_out.get(),
                                      Color4i::ALPHA_TRANSPARENT,
                                      sprite1.get(), "fade_sprite1_out" );
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to enqueue fade_blue_rect!";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite0.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
    nom::set_alignment( sprite1.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render a pretty backdrop for our demo
    if( bg_sprite != nullptr ) {
      bg_sprite->draw( this->render_window() );
    }

    // Render our animation's blue rectangle
    if( sprite0 != nullptr ) {
      sprite0->draw( this->render_window() );
    }

    if( sprite1 != nullptr ) {
      sprite1->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom
