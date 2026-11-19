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
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 INITIAL_ALPHA = Color4i::ALPHA_TRANSPARENT;
  const uint8 EXPECTED_ALPHA = Color4i::ALPHA_OPAQUE;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->init_with_color(Color4i::Blue, TEX_SIZE) );
  sprite->set_position(TEX_POS);
  sprite->set_alpha(INITIAL_ALPHA);
  sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_in =
    nom::create_action<FadeInAction>(sprite, DURATION);
  ASSERT_TRUE(fade_in != nullptr);
  fade_in->set_name("fade_in");

  auto action0 =
    nom::create_action<GroupAction>( {fade_in} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_in_params( fade_in.get(), EXPECTED_ALPHA,
                                    sprite.get() );

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_in.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeInActionFromNonTransparentAlpha)
{
  // Testing parameters
  const real32 DURATION = 1.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 INITIAL_ALPHA = (Color4i::ALPHA_OPAQUE / 2);
  const uint8 EXPECTED_ALPHA = Color4i::ALPHA_OPAQUE;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->init_with_color(Color4i::Blue, TEX_SIZE) );
  sprite->set_position(TEX_POS);
  sprite->set_alpha(INITIAL_ALPHA);
  sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_in =
    nom::create_action<FadeInAction>(sprite, DURATION);
  ASSERT_TRUE(fade_in != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_in} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_in_params( fade_in.get(), EXPECTED_ALPHA,
                                    sprite.get() );

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_in.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeOutAction)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 INITIAL_ALPHA = Color4i::ALPHA_OPAQUE;
  const uint8 EXPECTED_ALPHA = Color4i::ALPHA_TRANSPARENT;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->init_with_color(Color4i::Blue, TEX_SIZE) );
  sprite->set_position(TEX_POS);
  sprite->set_alpha(INITIAL_ALPHA);
  sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_out =
    nom::create_action<FadeOutAction>(sprite, DURATION);
  ASSERT_TRUE(fade_out != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_out} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  EXPECT_EQ(0, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_alpha_out_params(  fade_out.get(), EXPECTED_ALPHA,
                                      sprite.get() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_out.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";

  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeOutActionFromNonOpaqueAlpha)
{
  // Testing parameters
  const real32 DURATION = 1.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 INITIAL_ALPHA = (Color4i::ALPHA_OPAQUE / 2);
  const uint8 EXPECTED_ALPHA = Color4i::ALPHA_TRANSPARENT;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->init_with_color(Color4i::Blue, TEX_SIZE) );
  sprite->set_position(TEX_POS);
  sprite->set_alpha(INITIAL_ALPHA);
  sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_out =
    nom::create_action<FadeOutAction>(sprite, DURATION);
  ASSERT_TRUE(fade_out != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_out} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  EXPECT_EQ(0, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_out_params(  fade_out.get(), EXPECTED_ALPHA,
                                      sprite.get() );

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_out.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeAlphaByAction)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 INITIAL_ALPHA = Color4i::ALPHA_TRANSPARENT;
  const real32 FADE_BY = 129;
  const uint8 EXPECTED_ALPHA = (INITIAL_ALPHA + fabs(FADE_BY) );

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->init_with_color(Color4i::Blue, TEX_SIZE) );
  sprite->set_position(TEX_POS);
  sprite->set_alpha(INITIAL_ALPHA);
  sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(sprite, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_by_params( fade_by.get(), EXPECTED_ALPHA,
                                    sprite.get() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );
    this->expected_common_params(fade_by.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeAlphaByActionFromNonOpaqueAlpha)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 INITIAL_ALPHA = (Color4i::ALPHA_OPAQUE / 4);
  const real32 FADE_BY = 129;
  const uint8 EXPECTED_ALPHA = (INITIAL_ALPHA + FADE_BY);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->init_with_color(Color4i::Blue, TEX_SIZE) );
  sprite->set_position(TEX_POS);
  sprite->set_alpha(INITIAL_ALPHA);
  sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(sprite, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

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

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeAlphaByActionWithNegativeValue)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const uint8 INITIAL_ALPHA = Color4i::ALPHA_OPAQUE;
  const real32 FADE_BY = -129;
  const uint8 EXPECTED_ALPHA = (INITIAL_ALPHA - fabs(FADE_BY) );

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->init_with_color(Color4i::Blue, TEX_SIZE) );
  sprite->set_position(TEX_POS);
  sprite->set_alpha(INITIAL_ALPHA);
  sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(sprite, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

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

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, AlphaBlendingDemo)
{
  // Testing parameters
  const real32 DURATION = 2.5f;  // 5s total duration due to x2 sequences
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(256, 256);

  // NOTE: action0 init

  auto bg_sprite_tex = std::make_shared<Texture>();
  ASSERT_TRUE(bg_sprite_tex != nullptr);
  if( bg_sprite_tex->load( resources[0].path() + "backdrop.png" ) == false ) {
    FAIL()  << "Could not load 'backdrop.png' input file from "
            << resources[0].path();
  }

  auto bg_sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(bg_sprite != nullptr);
  EXPECT_EQ(true, bg_sprite->set_texture(bg_sprite_tex) );
  bg_sprite->set_alpha(Color4i::ALPHA_OPAQUE);
  // Stretched dimensions to cover entire window
  bg_sprite->set_size(WINDOW_DIMS);

  auto fade_bg_sprite_out_action =
    nom::create_action<FadeOutAction>(bg_sprite, DURATION);
  ASSERT_TRUE(fade_bg_sprite_out_action != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_bg_sprite_out_action} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("fade_bg_sprite_out");

  // NOTE: action1 init

  auto magenta_rect =
    std::make_shared<Rectangle>(  IntRect(RECT_POS, RECT_SIZE),
                                  Color4i::Magenta );
  ASSERT_TRUE(magenta_rect != nullptr);

  auto blue_rect =
    std::make_shared<Rectangle>(  IntRect(RECT_POS, RECT_SIZE),
                                  Color4i::Blue );
  ASSERT_TRUE(blue_rect != nullptr);

  auto magenta_sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(magenta_sprite != nullptr);
  EXPECT_EQ(true, magenta_sprite->set_texture( magenta_rect->texture() ) );
  magenta_sprite->set_alpha(Color4i::ALPHA_TRANSPARENT);
  magenta_sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto blue_sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(blue_sprite != nullptr);
  EXPECT_EQ(true, blue_sprite->set_texture( blue_rect->texture() ) );
  blue_sprite->set_alpha(Color4i::ALPHA_OPAQUE);
  blue_sprite->set_color_blend_mode(BLEND_MODE_BLEND);

  auto fade_magenta_sprite_in_action =
    nom::create_action<FadeInAction>(magenta_sprite, DURATION);
  ASSERT_TRUE(fade_magenta_sprite_in_action != nullptr);

  auto fade_blue_sprite_out =
    nom::create_action<FadeOutAction>(blue_sprite, DURATION);
  ASSERT_TRUE(fade_blue_sprite_out != nullptr);

  auto action1 =
    nom::create_action<SequenceAction>( {
      fade_magenta_sprite_in_action, fade_blue_sprite_out} );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_curve(TIMING_MODE);
  action1->set_speed(SPEED_MOD);
  action1->set_name("action1");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(2, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params( fade_bg_sprite_out_action.get(), DURATION,
                                  SPEED_MOD );
    this->expected_alpha_out_params(  fade_bg_sprite_out_action.get(),
                                      Color4i::ALPHA_TRANSPARENT,
                                      bg_sprite.get(), "fade_bg_sprite_out" );

    // action1
    this->expected_alpha_in_params( fade_magenta_sprite_in_action.get(),
                                    Color4i::ALPHA_OPAQUE,
                                    magenta_sprite.get(),
                                    "fade_magenta_sprite_in" );
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to enqueue action0!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action1.get(), 2);
    this->expected_common_params( fade_magenta_sprite_in_action.get(),
                                  DURATION, SPEED_MOD );
    this->expected_common_params( fade_blue_sprite_out.get(), DURATION,
                                  SPEED_MOD );
    this->expected_alpha_out_params(  fade_blue_sprite_out.get(),
                                      Color4i::ALPHA_TRANSPARENT,
                                      blue_sprite.get(),
                                      "fade_blue_sprite_out" );
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to enqueue fade_blue_rect!";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_update_callback( [=](real32) mutable {
    nom::set_alignment( magenta_sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
    nom::set_alignment( blue_sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( bg_sprite.get() );
  this->append_render_queue( magenta_sprite.get() );
  this->append_render_queue( blue_sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom
