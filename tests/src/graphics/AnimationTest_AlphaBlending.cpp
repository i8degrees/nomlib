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
#include "nomlib/tests/graphics/AnimationTest.hpp"

namespace nom {

TEST_F(AnimationTest, FadeInAction)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_TRANSPARENT);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto fade_in =
    nom::create_action<FadeInAction>(tex, DURATION);
  ASSERT_TRUE(fade_in != nullptr);

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  fade_in->set_name("fade_in");
  tex_bg->set_name("tex_bg");

  auto action0 =
    nom::create_action<GroupAction>( {fade_in, tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_in_params( fade_in.get(), Color4i::ALPHA_OPAQUE,
                                    tex.get() );

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(fade_in.get(), DURATION, SPEED_MOD);
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, FadeInActionFromNonTransparentOpacity)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE/2);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto fade_in =
    nom::create_action<FadeInAction>(tex, DURATION);
  ASSERT_TRUE(fade_in != nullptr);

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_in, tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_in_params( fade_in.get(), Color4i::ALPHA_OPAQUE,
                                    tex.get() );

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(fade_in.get(), DURATION, SPEED_MOD);
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, FadeOutAction)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto fade_out =
    nom::create_action<FadeOutAction>(tex, DURATION);
  ASSERT_TRUE(fade_out != nullptr);

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_out, tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_alpha_out_params(  fade_out.get(), Color4i::ALPHA_TRANSPARENT,
                                      tex.get() );

    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(fade_out.get(), DURATION, SPEED_MOD);
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";

  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, FadeOutActionFromNonOpaqueOpacity)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE/2);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto fade_out =
    nom::create_action<FadeOutAction>(tex, DURATION);
  ASSERT_TRUE(fade_out != nullptr);

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_out, tex_bg}, "action0");
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_out_params(  fade_out.get(), Color4i::ALPHA_OPAQUE,
                                      tex.get() );

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(fade_out.get(), DURATION, SPEED_MOD);
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \brief Fade in, from an alpha value of zero to an alpha value of 129-1 a
/// texture of a blue block.
TEST_F(AnimationTest, FadeAlphaByAction)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);
  const uint8 FADE_BY = 129;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_TRANSPARENT);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(tex, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by, tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_by_params( fade_by.get(), FADE_BY, tex.get() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(fade_by.get(), DURATION, SPEED_MOD);
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, FadeAlphaByActionFromNonOpaqueOpacity)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);
  const uint8 FADE_BY = 129;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE/4);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(tex, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by, tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    this->expected_alpha_by_params( fade_by.get(), FADE_BY, tex.get() );

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(fade_by.get(), DURATION, SPEED_MOD);
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, FadeAlphaByActionWithNegativeValue)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);
  const int16 FADE_BY = -129;
  const int16 EXPECTED_ALPHA = 126;

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(256, 256);

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  // Initialize texture with its starting alpha value for blending test
  tex->set_alpha(Color4i::ALPHA_OPAQUE);
  tex->set_blend_mode(SDL_BLENDMODE_BLEND);

  auto fade_by =
    nom::create_action<FadeAlphaByAction>(tex, FADE_BY, DURATION);
  ASSERT_TRUE(fade_by != nullptr);

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {fade_by, tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_alpha_by_params( fade_by.get(), EXPECTED_ALPHA, tex.get() );

    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(fade_by.get(), DURATION, SPEED_MOD);
    this->expected_common_params(tex_bg.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom
