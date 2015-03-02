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

TEST_F(ActionTest, FadeInActionReversed)
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

  auto fade_in_reversed =
    nom::create_action<ReversedAction>(fade_in);
  ASSERT_TRUE(fade_in_reversed != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_in_reversed}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_in_params( fade_in.get(), Color4i::ALPHA_TRANSPARENT,
                                    sprite.get() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeOutActionReversed)
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

  auto fade_out =
    nom::create_action<FadeOutAction>(sprite, DURATION);
  ASSERT_TRUE(fade_out != nullptr);

  auto fade_out_reversed =
    nom::create_action<ReversedAction>(fade_out);
  ASSERT_TRUE(fade_out_reversed != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_out_reversed}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_alpha_out_params(  fade_out.get(), Color4i::ALPHA_OPAQUE,
                                      sprite.get() );
    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, FadeAlphaByActionReversed)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const uint8 FADE_BY = 129;
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

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(sprite, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto fade_by_reversed =
    nom::create_action<ReversedAction>(fade_by);
  ASSERT_TRUE(fade_by_reversed != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by_reversed}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_by_params(fade_by.get(), FADE_BY);
    EXPECT_EQ( FADE_BY, sprite->alpha() );

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1, nom::UnitTest::test_name() );

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, MoveByActionReversed)
{
  // Testing parameters
  const float DURATION = 2.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const Point2i TRANSLATE_POS( Point2i(200,0) );
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(TRANSLATE_POS);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(0, 0);

  auto rect =
    std::make_shared<Rectangle>(IntRect(RECT_POS, RECT_SIZE), Color4i::Green);
  ASSERT_TRUE(rect != nullptr);

  auto sprite = std::make_shared<Sprite>( rect->texture() );
  ASSERT_TRUE(sprite != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto translate_rev =
    nom::create_action<ReversedAction>(translate);
  ASSERT_TRUE(translate_rev != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate_rev}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangle
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ScaleByActionReversed)
{
  // Testing parameters
  const float DURATION = 1.5f;  // 90 frames @ 60 FPS
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const Size2f SCALE_FACTOR(2.0f, 2.0f);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(128, 128);

  // Resulting animation frame effect to test for
  const Size2i EXPECTED_TEX_SIZE( Size2i( TEX_SIZE.w / SCALE_FACTOR.w,
                                          TEX_SIZE.h / SCALE_FACTOR.h )
  );

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex =
    std::shared_ptr<Texture>( rect->texture() );
  ASSERT_TRUE(tex != nullptr);

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto scale_by =
    nom::create_action<nom::ScaleByAction>(sprite, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by != nullptr);

  auto scale_by_reversed =
    nom::create_action<nom::ReversedAction>(scale_by);
  ASSERT_TRUE(scale_by_reversed != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {scale_by_reversed}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_SIZE, sprite->size() );
    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(scale_by.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, MoveByActionRepeatForReversed)
{
  // Testing parameters
  const float DURATION = 0.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const uint NUM_REPEATS = 4;
  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w,0) );
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(TRANSLATE_POS);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS-TRANSLATE_POS);

  auto rect =
    std::make_shared<Rectangle>(IntRect(RECT_POS, RECT_SIZE), Color4i::Red);
  ASSERT_TRUE(rect != nullptr);

  auto sprite = std::make_shared<Sprite>( rect->texture() );
  ASSERT_TRUE(sprite != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto repeat =
    nom::create_action<RepeatForAction>(translate, NUM_REPEATS);
  ASSERT_TRUE(repeat != nullptr);

  auto repeat_reversed =
    nom::create_action<ReversedAction>(repeat);
  ASSERT_TRUE(repeat_reversed != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {repeat_reversed}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(EXPECTED_TEX_POS, sprite->position() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);
    this->expected_repeat_params(repeat.get(), NUM_REPEATS);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";

  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangle
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks This test does not honor custom speed modifiers passed via command
/// line.
TEST_F(ActionTest, RepeatForeverActionReversed)
{
  // Testing parameters
  const real32 TEST_DURATION = 2.5f;  // when to stop testing "forever"
  const nom::size_type NUM_REPEATS = 4;
  const real32 DURATION = 0.5f;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w,0) );
  // Initial texture position and size
  const Point2i RECT_POS(TRANSLATE_POS);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);

  auto rect =
    std::make_shared<Rectangle>(IntRect(RECT_POS, RECT_SIZE), Color4i::Red);
  ASSERT_TRUE(rect != nullptr);

  auto sprite = std::make_shared<Sprite>( rect->texture() );
  ASSERT_TRUE(sprite != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto repeat_forever =
    nom::create_action<RepeatForeverAction>(translate);
  ASSERT_TRUE(repeat_forever != nullptr);

  auto repeat_forever_rev =
    nom::create_action<ReversedAction>(repeat_forever);
  ASSERT_TRUE(repeat_forever_rev != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {repeat_forever_rev}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  auto kill_timer =
    nom::create_action<WaitForDurationAction>(TEST_DURATION);
  ASSERT_TRUE(kill_timer != nullptr);

  auto action1 =
    nom::create_action<SequenceAction>( {kill_timer}, "action1" );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_curve(TIMING_MODE);
  action1->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {
    FAIL() << "action0 should never complete!";
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ(2, this->player.num_actions() );
    this->expected_repeat_params( repeat_forever.get(), NUM_REPEATS,
                                  "repeat_params" );

    this->expected_common_params( kill_timer.get(), TEST_DURATION, SPEED_MOD,
                                  "common_params" );
    this->expected_common_params( translate.get(), DURATION, SPEED_MOD,
                                  "common_params" );

    this->expected_action_params(action0.get(), 1, "action_params" );
    this->expected_action_params(action1.get(), 1, "action_params");

    this->quit();
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangle
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks Thanks goes to Tim Jones of [sdltutorials.com](http://www.sdltutorials.com/sdl-animation)
/// for the sprite frames of Yoshi chosen for this test!
TEST_F(ActionTest, SpriteActionReversed)
{
  // Testing parameters
  texture_frames anim_frames;
  const std::vector<const char*> texture_filenames = {{
    "yoshi_000.png", "yoshi_001.png", "yoshi_002.png", "yoshi_003.png",
    "yoshi_004.png", "yoshi_005.png", "yoshi_006.png", "yoshi_007.png"
  }};

  // fps per shown texture
  const real32 FRAME_DURATION = 0.5f;

  // total test duration
  const real32 DURATION = FRAME_DURATION * texture_filenames.size();

  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  this->init_sprite_action_test(texture_filenames, anim_frames);

  EXPECT_EQ( anim_frames.size(), texture_filenames.size() );

  auto sprite0 =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite0 != nullptr);

  auto tex_bg =
    nom::create_action<SpriteTexturesAction>(sprite0, anim_frames, FRAME_DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<ReversedAction>(tex_bg, "action0_rev");
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_sprite_textures_params(  tex_bg.get(), anim_frames.size(),
                                            DURATION, SPEED_MOD,
                                            "sprite_textures_params" );
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=, &sprite0](const RenderWindow& win) {

    if( sprite0 != nullptr && sprite0->valid() == true ) {
      sprite0->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

// FIXME
#if 0
TEST_F(ActionTest, ColorizeActionReversed)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  const Color4i TEX_START_COLOR(Color4i::Black);
  const Color4i TEX_END_COLOR(Color4i::White);
  const BlendMode BLEND_MODE = BLEND_MODE_ADD;
  const std::string TEX_FILE_PATH = resources[0].path() + "card.png";

  auto tex = std::make_shared<Texture>( Texture() );
  ASSERT_TRUE(tex != nullptr);
  if( tex->load(TEX_FILE_PATH) == false ) {
    FAIL() << "Could not load texture from file: " << TEX_FILE_PATH;
  }

  auto sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );
  sprite->set_color(TEX_START_COLOR);

  auto colorize = nom::create_action<nom::ColorizeAction>(sprite,
    TEX_START_COLOR, BLEND_MODE, DURATION);
  ASSERT_TRUE(colorize != nullptr);
  colorize->set_name("colorize");
  colorize->set_speed(SPEED_MOD);

  auto colorize_rev =
    nom::create_action<nom::ReversedAction>(colorize, "colorize_rev");
  ASSERT_TRUE(colorize_rev != nullptr);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(colorize_rev, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    EXPECT_EQ(TEX_END_COLOR, sprite->color() );
    EXPECT_EQ(BLEND_MODE, sprite->blend_mode() );
    this->expected_common_params(colorize.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( sprite.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {

    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}
#endif

} // namespace nom
