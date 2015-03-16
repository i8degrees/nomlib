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

/// \brief The two group actions should finish within one frame of each other.
TEST_F(ActionTest, GroupActionFinishEquality)
{
  // Testing parameters
  const real32 DURATION = 1.0f;
  const real32 SPEED_MOD0 = NOM_ACTION_TEST_FLAG(speed);
  const real32 SPEED_MOD1 = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h/4);
  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w-RECT_SIZE.w,0) );
  const Point2i RECT0_POS(0, RECT_SIZE.h*0);
  const Point2i RECT1_POS(0, RECT_SIZE.h*1);
  const Point2i RECT2_POS(0, RECT_SIZE.h*2);
  const Point2i RECT3_POS(0, RECT_SIZE.h*3);
  const Point2i EXPECTED_TEX0_POS(TRANSLATE_POS);
  const Point2i EXPECTED_TEX1_POS(TRANSLATE_POS.x, RECT_SIZE.h*1);
  const Point2i EXPECTED_TEX2_POS(TRANSLATE_POS.x, RECT_SIZE.h*2);
  const Point2i EXPECTED_TEX3_POS(TRANSLATE_POS.x, RECT_SIZE.h*3);

  auto rect0 =
    std::make_shared<Rectangle>(IntRect(RECT0_POS, RECT_SIZE), Color4i::Green);
  ASSERT_TRUE(rect0 != nullptr);

  auto rect1 =
    std::make_shared<Rectangle>(IntRect(RECT1_POS, RECT_SIZE), Color4i::Red);
  ASSERT_TRUE(rect1 != nullptr);

  auto rect2 =
    std::make_shared<Rectangle>(IntRect(RECT2_POS, RECT_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect2 != nullptr);

  auto rect3 =
    std::make_shared<Rectangle>(IntRect(RECT3_POS, RECT_SIZE), Color4i::Yellow);
  ASSERT_TRUE(rect3 != nullptr);

  auto sprite0 = nom::make_shared_sprite( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = nom::make_shared_sprite( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = nom::make_shared_sprite( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = nom::make_shared_sprite( rect3->texture() );
  ASSERT_TRUE(sprite3 != nullptr);

  auto translate0 =
    nom::create_action<MoveByAction>(sprite0, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate0 != nullptr);

  auto translate1 =
    nom::create_action<MoveByAction>(sprite1, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate1 != nullptr);

  auto translate2 =
    nom::create_action<MoveByAction>(sprite2, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate2 != nullptr);

  auto translate3 =
    nom::create_action<MoveByAction>(sprite3, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate3 != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate0, translate1} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD0);
  action0->set_name("action0");

  auto action1 =
    nom::create_action<GroupAction>( {translate2, translate3} );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_speed(SPEED_MOD1);
  action1->set_timing_curve(TIMING_MODE);
  action1->set_name("action1");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  auto remove_action1 =
    nom::create_action<RemoveAction>(action1);
  ASSERT_TRUE(remove_action1 != nullptr);
  remove_action1->set_name("remove_action1");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD0);

    this->player.run_action(remove_action0, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite0 != nullptr);
      EXPECT_FALSE( sprite0->valid() );
      EXPECT_TRUE(sprite1 != nullptr);
      EXPECT_FALSE( sprite1->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(1, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ( EXPECTED_TEX2_POS, sprite2->position() );
    EXPECT_EQ( EXPECTED_TEX3_POS, sprite3->position() );
    this->expected_action_params(action1.get(), 2);
    this->expected_common_params(translate2.get(), DURATION, SPEED_MOD1);
    this->expected_common_params(translate3.get(), DURATION, SPEED_MOD1);

    this->player.run_action(remove_action1, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite2 != nullptr);
      EXPECT_FALSE( sprite2->valid() );
      EXPECT_TRUE(sprite2 != nullptr);
      EXPECT_FALSE( sprite2->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_update_callback( [=](float) {

    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false &&
        // The smallest speed modifier should always be used here
        this->expected_min_duration(DURATION, SPEED_MOD0) == true )
    {
      this->quit();
    }
  });

  this->append_render_queue( sprite0.get() ); // action0
  this->append_render_queue( sprite1.get() ); // action1
  this->append_render_queue( sprite2.get() ); // action2
  this->append_render_queue( sprite3.get() ); // action3
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, CancelAction)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const Point2i TRANSLATE_POS(Point2i::zero);

  auto translate0 =
    nom::create_action<MoveByAction>(nullptr, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate0 != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate0} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_name("action0");

  auto action1 =
    nom::create_action<SequenceAction>( { translate0->clone() } );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_name("action1");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0);
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(true, this->player.cancel_action("action0") );
  EXPECT_EQ(false, this->player.cancel_action("action1") );

  this->run_action_ret =
  this->player.run_action(action1);
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(true, this->player.cancel_action("action1") );
}

TEST_F(ActionTest, ConcurrentGroupActions)
{
  // Testing parameters
  const real32 DURATION = 1.0f;
  const real32 SPEED_MOD0 = NOM_ACTION_TEST_FLAG(speed) * 1.0f;     // action0
  const real32 SPEED_MOD1 = NOM_ACTION_TEST_FLAG(speed) * 2.0f;     // action1
  const real32 SPEED_MOD2 = NOM_ACTION_TEST_FLAG(speed) * 2.25f;    // action2
  const real32 SPEED_MOD3 = NOM_ACTION_TEST_FLAG(speed) * 1.25f;    // action3
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h/4);
  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w-RECT_SIZE.w,0) );
  const Point2i RECT0_POS(0, RECT_SIZE.h*0);
  const Point2i RECT1_POS(0, RECT_SIZE.h*1);
  const Point2i RECT2_POS(0, RECT_SIZE.h*2);
  const Point2i RECT3_POS(0, RECT_SIZE.h*3);
  const Point2i EXPECTED_TEX0_POS(TRANSLATE_POS);
  const Point2i EXPECTED_TEX1_POS(TRANSLATE_POS.x, RECT_SIZE.h*1);
  const Point2i EXPECTED_TEX2_POS(TRANSLATE_POS.x, RECT_SIZE.h*2);
  const Point2i EXPECTED_TEX3_POS(TRANSLATE_POS.x, RECT_SIZE.h*3);

  auto rect0 =
    std::make_shared<Rectangle>(IntRect(RECT0_POS, RECT_SIZE), Color4i::Green);
  ASSERT_TRUE(rect0 != nullptr);

  auto rect1 =
    std::make_shared<Rectangle>(IntRect(RECT1_POS, RECT_SIZE), Color4i::Red);
  ASSERT_TRUE(rect1 != nullptr);

  auto rect2 =
    std::make_shared<Rectangle>(IntRect(RECT2_POS, RECT_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect2 != nullptr);

  auto rect3 =
    std::make_shared<Rectangle>(IntRect(RECT3_POS, RECT_SIZE), Color4i::Yellow);
  ASSERT_TRUE(rect3 != nullptr);

  auto sprite0 = nom::make_shared_sprite( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = nom::make_shared_sprite( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = nom::make_shared_sprite( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = nom::make_shared_sprite( rect3->texture() );
  ASSERT_TRUE(sprite3 != nullptr);

  auto translate0 =
    nom::create_action<MoveByAction>(sprite0, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate0 != nullptr);

  auto translate1 =
    nom::create_action<MoveByAction>(sprite1, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate1 != nullptr);

  auto translate2 =
    nom::create_action<MoveByAction>(sprite2, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate2 != nullptr);

  auto translate3 =
    nom::create_action<MoveByAction>(sprite3, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate3 != nullptr);

  translate0->set_name("translate0");
  translate1->set_name("translate1");
  translate2->set_name("translate2");
  translate3->set_name("translate3");

  auto action0 = nom::create_action<GroupAction>( {translate0} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD0);
  action0->set_name("action0");

  auto action1 = nom::create_action<GroupAction>( {translate1} );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_speed(SPEED_MOD1);
  action1->set_timing_curve(TIMING_MODE);
  action1->set_name("action1");

  auto action2 = nom::create_action<GroupAction>( {translate2} );
  ASSERT_TRUE(action2 != nullptr);
  action2->set_speed(SPEED_MOD2);
  action2->set_timing_curve(TIMING_MODE);
  action2->set_name("action2");

  auto action3 = nom::create_action<GroupAction>( {translate3} );
  ASSERT_TRUE(action3 != nullptr);
  action3->set_speed(SPEED_MOD3);
  action3->set_timing_curve(TIMING_MODE);
  action3->set_name("action3");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  auto remove_action1 =
    nom::create_action<RemoveAction>(action1);
  ASSERT_TRUE(remove_action1 != nullptr);
  remove_action1->set_name("remove_action1");

  auto remove_action2 =
    nom::create_action<RemoveAction>(action2);
  ASSERT_TRUE(remove_action2 != nullptr);
  remove_action2->set_name("remove_action2");

  auto remove_action3 =
    nom::create_action<RemoveAction>(action3);
  ASSERT_TRUE(remove_action3 != nullptr);
  remove_action3->set_name("remove_action3");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);

    this->player.run_action(remove_action0, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite0 != nullptr);
      EXPECT_FALSE( sprite0->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action1.get(), 1);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD1);

    this->player.run_action(remove_action1, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite1 != nullptr);
      EXPECT_FALSE( sprite1->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action2, [=]() {

    EXPECT_EQ( EXPECTED_TEX2_POS, sprite2->position() );
    this->expected_action_params(action2.get(), 1);
    this->expected_common_params(translate2.get(), DURATION, SPEED_MOD2);

    this->player.run_action(remove_action2, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite2 != nullptr);
      EXPECT_FALSE( sprite2->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action2";
  EXPECT_EQ(3, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action3, [=]() {

    EXPECT_EQ( EXPECTED_TEX3_POS, sprite3->position() );
    this->expected_action_params(action3.get(), 1);
    this->expected_common_params(translate3.get(), DURATION, SPEED_MOD3);

    this->player.run_action(remove_action3, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite3 != nullptr);
      EXPECT_FALSE( sprite3->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action3";
  EXPECT_EQ(4, this->player.num_actions() );

  this->append_update_callback( [=](float) {

    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false &&
        this->player.action_running("action2") == false &&
        this->player.action_running("action3") == false &&
        this->expected_min_duration(DURATION, SPEED_MOD2) == true )
    {
      this->quit();
    }
  });

  this->append_render_queue( sprite0.get() ); // action0
  this->append_render_queue( sprite1.get() ); // action1
  this->append_render_queue( sprite2.get() ); // action2
  this->append_render_queue( sprite3.get() ); // action3
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ConcurrentSequenceActions)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const real32 SPEED_MOD0 = NOM_ACTION_TEST_FLAG(speed) * 1.0f;   // action0(2/2)
  const real32 SPEED_MOD1 = NOM_ACTION_TEST_FLAG(speed) * 2.0f;   // action0(1/2)
  const real32 SPEED_MOD2 = NOM_ACTION_TEST_FLAG(speed) * 2.25f;  // action1(1/2)
  const real32 SPEED_MOD3 = NOM_ACTION_TEST_FLAG(speed) * 1.25f;  // action1(2/2)
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h/4);
  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w-RECT_SIZE.w,0) );
  const Point2i RECT0_POS(0, RECT_SIZE.h*0);
  const Point2i RECT1_POS(0, RECT_SIZE.h*1);
  const Point2i RECT2_POS(0, RECT_SIZE.h*2);
  const Point2i RECT3_POS(0, RECT_SIZE.h*3);
  const Point2i EXPECTED_TEX0_POS(TRANSLATE_POS);
  const Point2i EXPECTED_TEX1_POS(TRANSLATE_POS.x, RECT_SIZE.h*1);
  const Point2i EXPECTED_TEX2_POS(TRANSLATE_POS.x, RECT_SIZE.h*2);
  const Point2i EXPECTED_TEX3_POS(TRANSLATE_POS.x, RECT_SIZE.h*3);

  auto rect0 =
    std::make_shared<Rectangle>(IntRect(RECT0_POS, RECT_SIZE), Color4i::Green);
  ASSERT_TRUE(rect0 != nullptr);

  auto rect1 =
    std::make_shared<Rectangle>(IntRect(RECT1_POS, RECT_SIZE), Color4i::Red);
  ASSERT_TRUE(rect1 != nullptr);

  auto rect2 =
    std::make_shared<Rectangle>(IntRect(RECT2_POS, RECT_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect2 != nullptr);

  auto rect3 =
    std::make_shared<Rectangle>(IntRect(RECT3_POS, RECT_SIZE), Color4i::Yellow);
  ASSERT_TRUE(rect3 != nullptr);

  auto sprite0 = nom::make_shared_sprite( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = nom::make_shared_sprite( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = nom::make_shared_sprite( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = nom::make_shared_sprite( rect3->texture() );
  ASSERT_TRUE(sprite3 != nullptr);

  auto translate0 =
    nom::create_action<MoveByAction>(sprite0, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate0 != nullptr);

  auto translate1 =
    nom::create_action<MoveByAction>(sprite1, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate1 != nullptr);

  auto translate2 =
    nom::create_action<MoveByAction>(sprite2, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate2 != nullptr);

  auto translate3 =
    nom::create_action<MoveByAction>(sprite3, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate3 != nullptr);

  translate0->set_name("translate0");
  translate1->set_name("translate1");
  translate2->set_name("translate2");
  translate3->set_name("translate3");
  translate0->set_speed(SPEED_MOD0);
  translate1->set_speed(SPEED_MOD1);
  translate2->set_speed(SPEED_MOD2);
  translate3->set_speed(SPEED_MOD3);

  auto action0 =
    nom::create_action<SequenceAction>( {translate0, translate1} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_name("action0");

  auto action1 =
    nom::create_action<SequenceAction>( {translate2, translate3} );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_curve(TIMING_MODE);
  action1->set_name("action1");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  auto remove_action1 =
    nom::create_action<RemoveAction>(action1);
  ASSERT_TRUE(remove_action1 != nullptr);
  remove_action1->set_name("remove_action1");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD1);

    this->player.run_action(remove_action0, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite0 != nullptr);
      EXPECT_FALSE( sprite0->valid() );
      EXPECT_TRUE(sprite1 != nullptr);
      EXPECT_FALSE( sprite1->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ( EXPECTED_TEX2_POS, sprite2->position() );
    EXPECT_EQ( EXPECTED_TEX3_POS, sprite3->position() );
    this->expected_action_params(action1.get(), 2);
    this->expected_common_params(translate2.get(), DURATION, SPEED_MOD2);
    this->expected_common_params(translate3.get(), DURATION, SPEED_MOD3);

    this->player.run_action(remove_action1, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite2 != nullptr);
      EXPECT_FALSE( sprite2->valid() );
      EXPECT_TRUE(sprite3 != nullptr);
      EXPECT_FALSE( sprite3->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_update_callback( [=](float) {

    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false &&
        this->expected_min_duration(DURATION, SPEED_MOD2) == true )
    {
      this->quit();
    }
  });

  this->append_render_queue( sprite0.get() ); // action0
  this->append_render_queue( sprite1.get() ); // action0
  this->append_render_queue( sprite2.get() ); // action1
  this->append_render_queue( sprite3.get() ); // action1
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ConcurrentGroupAndSequenceActions)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const real32 SPEED_MOD0 = NOM_ACTION_TEST_FLAG(speed) * 1.0f; // action0
  const real32 SPEED_MOD1 = NOM_ACTION_TEST_FLAG(speed) * 2.0f; // action0
  const real32 SPEED_MOD2 = NOM_ACTION_TEST_FLAG(speed) * 2.25f; // action1
  const real32 SPEED_MOD3 = NOM_ACTION_TEST_FLAG(speed) * 1.25f; // action1
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h/4);
  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w-RECT_SIZE.w,0) );
  const Point2i RECT0_POS(0, RECT_SIZE.h*0);
  const Point2i RECT1_POS(0, RECT_SIZE.h*1);
  const Point2i RECT2_POS(0, RECT_SIZE.h*2);
  const Point2i RECT3_POS(0, RECT_SIZE.h*3);
  const Point2i EXPECTED_TEX0_POS(TRANSLATE_POS);
  const Point2i EXPECTED_TEX1_POS(TRANSLATE_POS.x, RECT_SIZE.h*1);
  const Point2i EXPECTED_TEX2_POS(TRANSLATE_POS.x, RECT_SIZE.h*2);
  const Point2i EXPECTED_TEX3_POS(TRANSLATE_POS.x, RECT_SIZE.h*3);

  auto rect0 =
    std::make_shared<Rectangle>(IntRect(RECT0_POS, RECT_SIZE), Color4i::Green);
  ASSERT_TRUE(rect0 != nullptr);

  auto rect1 =
    std::make_shared<Rectangle>(IntRect(RECT1_POS, RECT_SIZE), Color4i::Red);
  ASSERT_TRUE(rect1 != nullptr);

  auto rect2 =
    std::make_shared<Rectangle>(IntRect(RECT2_POS, RECT_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect2 != nullptr);

  auto rect3 =
    std::make_shared<Rectangle>(IntRect(RECT3_POS, RECT_SIZE), Color4i::Yellow);
  ASSERT_TRUE(rect3 != nullptr);

  auto sprite0 = nom::make_shared_sprite( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = nom::make_shared_sprite( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = nom::make_shared_sprite( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = nom::make_shared_sprite( rect3->texture() );
  ASSERT_TRUE(sprite3 != nullptr);

  auto translate0 =
    nom::create_action<MoveByAction>(sprite0, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate0 != nullptr);

  auto translate1 =
    nom::create_action<MoveByAction>(sprite1, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate1 != nullptr);

  auto translate2 =
    nom::create_action<MoveByAction>(sprite2, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate2 != nullptr);

  auto translate3 =
    nom::create_action<MoveByAction>(sprite3, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate3 != nullptr);

  translate0->set_name("translate0");
  translate1->set_name("translate1");
  translate2->set_name("translate2");
  translate3->set_name("translate3");
  translate0->set_speed(SPEED_MOD0);
  translate1->set_speed(SPEED_MOD1);
  translate2->set_speed(SPEED_MOD2);
  translate3->set_speed(SPEED_MOD3);

  auto action0 =
    nom::create_action<GroupAction>( {translate0, translate1} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_name("action0");

  auto action1 =
    nom::create_action<SequenceAction>( {translate2, translate3} );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_curve(TIMING_MODE);
  action1->set_name("action1");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  auto remove_action1 =
    nom::create_action<RemoveAction>(action1);
  ASSERT_TRUE(remove_action1 != nullptr);
  remove_action1->set_name("remove_action1");

  // EXPECT_EQ( ActionPlayer::IDLE, this->player.player_state() );
  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD1);

    this->player.run_action(remove_action0, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite0 != nullptr);
      EXPECT_FALSE( sprite0->valid() );
      EXPECT_TRUE(sprite1 != nullptr);
      EXPECT_FALSE( sprite1->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  // EXPECT_EQ( ActionPlayer::IDLE, this->player.player_state() );
  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ( EXPECTED_TEX2_POS, sprite2->position() );
    EXPECT_EQ( EXPECTED_TEX3_POS, sprite3->position() );
    this->expected_action_params(action1.get(), 2);
    this->expected_common_params(translate2.get(), DURATION, SPEED_MOD2);
    this->expected_common_params(translate3.get(), DURATION, SPEED_MOD3);

    this->player.run_action(remove_action1, [=]() {
      // NOTE: The removal of the action should free the stored texture of each
      // sprite, preventing it from being rendered by invalidating the texture.
      EXPECT_TRUE(sprite2 != nullptr);
      EXPECT_FALSE( sprite2->valid() );
      EXPECT_TRUE(sprite3 != nullptr);
      EXPECT_FALSE( sprite3->valid() );
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_update_callback( [=](float) {

    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false &&
        this->expected_min_duration(DURATION, SPEED_MOD2) == true )
    {
      this->quit();
    }
  });

  this->append_render_queue( sprite0.get() ); // action0
  this->append_render_queue( sprite1.get() ); // action0
  this->append_render_queue( sprite2.get() ); // action1
  this->append_render_queue( sprite3.get() ); // action1
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, RunActionWithName)
{
  // Testing parameters
  const float DURATION = 0.0f;
  const Point2i TRANSLATE_POS(Point2i::zero);

  auto translate0 =
    nom::create_action<MoveByAction>(nullptr, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate0 != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate0} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_name("action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0);
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(true, this->player.action_running("action0") );
  EXPECT_EQ(1, this->player.num_actions() );
}

TEST_F(ActionTest, ClonedGroupAction)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const Point2i TRANSLATE_POS( Point2i(200,0) );
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS);

  auto sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  sprite->init_with_color(Color4i::Green, RECT_SIZE);
  sprite->set_position(RECT_POS);
  EXPECT_EQ( true, sprite->valid() );

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

    sprite->set_position(RECT_POS);
    this->player.run_action(action0->clone(), [=]() {

      EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
      EXPECT_EQ(1, this->player.num_actions() );

      this->expected_action_params(action0.get(), 1);
      this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

      this->player.run_action(remove_action0, [=]() {
        ASSERT_TRUE(sprite != nullptr);
        EXPECT_FALSE( sprite->valid() );
      });
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION*2, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ClonedSequenceAction)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const Point2i TRANSLATE_POS( Point2i(200,0) );
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS);

  auto sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  sprite->init_with_color(Color4i::Green, RECT_SIZE);
  sprite->set_position(RECT_POS);
  EXPECT_EQ( true, sprite->valid() );

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<SequenceAction>( {translate} );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

    sprite->set_position(RECT_POS);
    this->player.run_action(action0->clone(), [=]() {

      EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
      EXPECT_EQ(1, this->player.num_actions() );

      this->expected_action_params(action0.get(), 1);
      this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

      this->player.run_action(remove_action0, [=]() {
        ASSERT_TRUE(sprite != nullptr);
        EXPECT_FALSE( sprite->valid() );
      });
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION*2, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ClonedReversedAction)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const Point2i TRANSLATE_POS( Point2i(200,0) );
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i RECT_POS(WINDOW_DIMS.w-RECT_SIZE.w, 0);
  const Point2i EXPECTED_TEX_POS(RECT_POS-TRANSLATE_POS);

  auto sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  sprite->init_with_color(Color4i::Green, RECT_SIZE);
  sprite->set_position(RECT_POS);
  EXPECT_EQ( true, sprite->valid() );

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<ReversedAction>(translate);
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

    sprite->set_position(RECT_POS);
    this->player.run_action(action0->clone(), [=]() {

      EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
      EXPECT_EQ(1, this->player.num_actions() );
      this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

      this->player.run_action(remove_action0, [=]() {
        ASSERT_TRUE(sprite != nullptr);
        EXPECT_FALSE( sprite->valid() );
      });
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION*2, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ClonedRepeatForAction)
{
  // Testing parameters
  const real32 DURATION = 2.0f;
  const real32 SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const Point2i TRANSLATE_POS( Point2i(200,0) );
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const nom::size_type NUM_REPEATS = 1;

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS);

  auto sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  sprite->init_with_color(Color4i::Green, RECT_SIZE);
  sprite->set_position(RECT_POS);
  EXPECT_EQ( true, sprite->valid() );

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<RepeatForAction>(translate, NUM_REPEATS);
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);
  action0->set_name("action0");

  auto remove_action0 =
    nom::create_action<RemoveAction>(action0);
  ASSERT_TRUE(remove_action0 != nullptr);
  remove_action0->set_name("remove_action0");

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_repeat_params(action0.get(), NUM_REPEATS, "num_repeats" );
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

    sprite->set_position(RECT_POS);
    this->player.run_action(action0->clone(), [=]() {

      EXPECT_EQ( EXPECTED_TEX_POS, sprite->position() );
      EXPECT_EQ(1, this->player.num_actions() );

      this->expected_repeat_params(action0.get(), NUM_REPEATS, "num_repeats" );
      this->expected_common_params(translate.get(), DURATION, SPEED_MOD);

      this->player.run_action(remove_action0, [=]() {
        ASSERT_TRUE(sprite != nullptr);
        EXPECT_FALSE( sprite->valid() );
      });
    });
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](real32) {
    if( this->expected_min_duration(DURATION*2, SPEED_MOD) == true ) {
      this->quit();
    }
  });

  this->append_render_queue( sprite.get() );
  this->append_frame_interval(FPS);

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

} // namespace nom
