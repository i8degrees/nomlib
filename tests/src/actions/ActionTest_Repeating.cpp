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

void
ActionTest::setup_repeating_cursor_test(  real32 duration, real32 speed,
                                          real32 fps, uint32 type,
                                          nom::size_type num_repeats,
                                          const std::string& scope_name )
{
  // Testing parameters
  const nom::size_type NUM_REPEATS = num_repeats;
  const std::string TEXTURE_FILENAME = "cursors.png";
  const std::string SPRITE_SHEET_FILENAME = "cursors.json";

  // fps per texture
  const real32 FRAME_DURATION = 0.100f;

  const nom::size_type NUM_FRAMES = 3;

  // total duration of a single repeat
  const real32 ACTION_DURATION = (FRAME_DURATION * NUM_FRAMES);

  // maximum duration before test termination occurs
  const real32 DURATION = duration;

  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const real32 SPEED_MOD = speed;
  const uint32 FPS = fps;
  nom::SpriteSheet anim_frames;

  auto tex = std::make_shared<Texture>();
  ASSERT_TRUE(tex != nullptr);

  bool ret = tex->load(this->resources[0].path() + TEXTURE_FILENAME);
  if( ret == false ) {
    FAIL()  << "Could not load texture " << TEXTURE_FILENAME
            << " from file path: " << this->resources[0].path() << "\n";
  }

  if( anim_frames.load_file(  this->resources[0].path() +
      SPRITE_SHEET_FILENAME ) == false )
  {
    FAIL()  << "Could not load sprite sheet " << TEXTURE_FILENAME
            << " from file path: " << this->resources[0].path() << "\n";
  }

  EXPECT_EQ(true, anim_frames.remove_frame(3) );
  EXPECT_EQ(true, anim_frames.remove_frame(4) );

  auto sprite = std::make_shared<SpriteBatch>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );
  sprite->set_sprite_sheet(anim_frames);
  sprite->set_frame(0);

  nom::set_alignment( sprite.get(), Point2i::zero, this->WINDOW_DIMS,
                      Anchor::MiddleCenter );

  EXPECT_EQ( NUM_FRAMES, sprite->frames() );

  auto sprite_action =
    nom::create_action<SpriteBatchAction>(sprite, FRAME_DURATION);
  ASSERT_TRUE(sprite_action != nullptr);
  sprite_action->set_speed(SPEED_MOD);

  std::shared_ptr<IActionObject> action0;
  if( type & ActionType::GROUP ) {
    action0 = nom::create_action<GroupAction>( {sprite_action}, "action0" );
  } else if( type & ActionType::SEQUENCE ) {
    action0 = nom::create_action<SequenceAction>( {sprite_action}, "action0" );
  } else if( type & ActionType::REVERSED ) {
    action0 = nom::create_action<ReversedAction>(sprite_action, "action0");
  } else if( type & ActionType::ACTION ) {
    action0 = sprite_action;
  } else {
    ASSERT_TRUE("ActionType must be either one of the enumeration values")
    << scope_name;
  }

  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);

  std::shared_ptr<IActionObject> repeat;
  if( type & ActionType::REPEAT_FOR ) {
    repeat =
      nom::create_action<RepeatForAction>(action0, NUM_REPEATS, "action0_RepeatFor");
  } else if( type & ActionType::REPEAT_FOREVER ) {
    repeat =
      nom::create_action<RepeatForeverAction>(action0, "action0_RepeatForever");
  }
  ASSERT_TRUE(repeat != nullptr);

  EXPECT_EQ(0, this->player.num_actions() )
  << scope_name;
  this->run_action_ret =
  this->player.run_action(repeat, [=]() {

    this->expected_sprite_batch_action_params(  sprite_action.get(), NUM_FRAMES,
                                                ACTION_DURATION, SPEED_MOD,
                                                "sprite_batch_params" );
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to enqueue repeating action in " << scope_name;
  EXPECT_EQ(1, this->player.num_actions() )
  << scope_name;

  auto kill_timer =
    nom::create_action<WaitForDurationAction>(DURATION);
  ASSERT_TRUE(kill_timer != nullptr);
  kill_timer->set_name("kill_timer");

  EXPECT_EQ(1, this->player.num_actions() )
  << scope_name;
  this->run_action_ret =
  this->player.run_action(kill_timer, [=]() {

    this->expected_common_params( kill_timer.get(), DURATION, SPEED_MOD,
                                  "common_params" );
    if( type & ActionType::REPEAT_FOR ) {
      RepeatForAction* repeat_obj =
        NOM_DYN_PTR_CAST(RepeatForAction*, repeat.get() );

      this->expected_repeat_params( repeat_obj, NUM_REPEATS, "repeat_params" );
    } else if( type & ActionType::REPEAT_FOREVER ) {
      RepeatForeverAction* repeat_obj =
        NOM_DYN_PTR_CAST(RepeatForeverAction*, repeat.get() );

      this->expected_repeat_params( repeat_obj, NUM_REPEATS, "repeat_params" );
    }

    this->quit();
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the kill_timer in " << scope_name;
  EXPECT_EQ(2, this->player.num_actions() )
  << scope_name;

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, RepeatForAction)
{
  // Testing parameters
  const float DURATION = 0.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const uint NUM_REPEATS = 4;
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w,0) );

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Red) );
  ASSERT_TRUE(rect != nullptr);

  auto sprite = std::make_shared<Sprite>( rect->texture() );
  ASSERT_TRUE(sprite != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto repeat =
    nom::create_action<RepeatForAction>(translate, NUM_REPEATS);
  ASSERT_TRUE(repeat != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {repeat}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);
    this->expected_repeat_params(repeat.get(), NUM_REPEATS);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) {
    if( this->expected_min_duration(DURATION, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks This test does not honor custom speed modifiers passed via command
/// line.
TEST_F(ActionTest, RepeatForeverAction)
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
  const Point2i RECT_POS(Point2i::zero);
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

  auto action0 =
    nom::create_action<GroupAction>( {repeat_forever}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  auto kill_timer =
    nom::create_action<WaitForDurationAction>(TEST_DURATION);
  ASSERT_TRUE(kill_timer != nullptr);
  kill_timer->set_speed(SPEED_MOD);
  kill_timer->set_name("kill_timer");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {
    FAIL() << "action0 should never complete!";
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(kill_timer, [=]() {

    EXPECT_EQ(2, this->player.num_actions() );
    this->expected_repeat_params(repeat_forever.get(), NUM_REPEATS);
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);
    this->expected_common_params(kill_timer.get(), TEST_DURATION, SPEED_MOD);

    this->quit();
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue kill_timer!";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, SpriteBatchActionRepeatForever)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::ACTION | ActionType::REPEAT_FOREVER;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

TEST_F(ActionTest, SpriteBatchActionRepeatForeverReversed)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::REVERSED | ActionType::REPEAT_FOREVER;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

TEST_F(ActionTest, SpriteBatchActionGroup)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::GROUP | ActionType::REPEAT_FOR;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

TEST_F(ActionTest, SpriteBatchActionSequence)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::SEQUENCE | ActionType::REPEAT_FOR;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

TEST_F(ActionTest, SpriteBatchActionReversed)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::REVERSED | ActionType::REPEAT_FOR;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

/// \remarks This test does not honor custom speed modifiers passed via command
/// line.
TEST_F(ActionTest, SpriteBatchActionGroupRepeatingForever)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::GROUP | ActionType::REPEAT_FOREVER;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

/// \remarks This test does not honor custom speed modifiers and frame
/// intervals passed via command line.
TEST_F(ActionTest, SpriteBatchActionSequenceRepeatingForever)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::SEQUENCE | ActionType::REPEAT_FOREVER;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

/// \remarks This test does not honor custom speed modifiers and frame
/// intervals passed via command line.
TEST_F(ActionTest, SpriteBatchActionRepeatingForeverReversed)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 1.5f;
  // this value correlates with TEST_DURATION
  const nom::size_type NUM_REPEATS = 4;
  const uint32 action_type =
    ActionType::REVERSED | ActionType::REPEAT_FOREVER;
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

/// \remarks Thanks goes to Tim Jones of [sdltutorials.com](http://www.sdltutorials.com/sdl-animation)
/// for the sprite frames of Yoshi chosen for this test!
TEST_F(ActionTest, AnimateTexturesActionActionWithRepeatForAction)
{
  // Testing parameters
  texture_frames anim_frames;
  const std::vector<const char*> texture_filenames = {{
    "yoshi_000.png", "yoshi_001.png", "yoshi_002.png", "yoshi_003.png",
    "yoshi_004.png", "yoshi_005.png", "yoshi_006.png", "yoshi_007.png"
  }};

  // fps per texture
  const real32 FRAME_DURATION = 0.100f;

  // total duration of a single repeat
  const real32 ACTION_DURATION =
    (FRAME_DURATION * texture_filenames.size() );

  const nom::size_type NUM_REPEATS = 4;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  this->init_animate_textures_test(texture_filenames, anim_frames);

  EXPECT_EQ( anim_frames.size(), texture_filenames.size() );

  auto sprite0 =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite0 != nullptr);

  auto tex_bg =
    nom::create_action<AnimateTexturesAction>(  sprite0, anim_frames,
                                                FRAME_DURATION );
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<RepeatForAction>(tex_bg, NUM_REPEATS);
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0_repeat");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_animate_textures_params( tex_bg.get(), anim_frames.size(),
                                            ACTION_DURATION, SPEED_MOD,
                                            "animate_textures_params" );
    this->expected_repeat_params(action0.get(), NUM_REPEATS);

    this->player.run_action(remove_action0, [=]() {
      ASSERT_TRUE(sprite0 != nullptr);
      EXPECT_FALSE( sprite0->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(  (ACTION_DURATION * NUM_REPEATS),
                                      SPEED_MOD ) == true )
    {
      this->quit();
    }
  });

  this->append_render_queue( sprite0.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom
