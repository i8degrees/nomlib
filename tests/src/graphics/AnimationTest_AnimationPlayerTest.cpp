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

/// \brief The two group actions should finish within one frame of each other.
TEST_F(AnimationTest, GroupActionFinishEquality)
{
  // Testing parameters
  const real32 DURATION = 1.0f;
  const real32 SPEED_MOD0 = NOM_ANIM_TEST_FLAG(speed);
  const real32 SPEED_MOD1 = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

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

  auto sprite0 = std::make_shared<Sprite>( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = std::make_shared<Sprite>( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = std::make_shared<Sprite>( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = std::make_shared<Sprite>( rect3->texture() );
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
    nom::create_action<GroupAction>( {translate0, translate1}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD0);

  auto action1 =
    nom::create_action<GroupAction>( {translate2, translate3}, "action1" );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_speed(SPEED_MOD1);
  action1->set_timing_mode(TIMING_MODE);

  // action0

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD0);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  // action1

  EXPECT_EQ(1, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ( EXPECTED_TEX2_POS, sprite2->position() );
    EXPECT_EQ( EXPECTED_TEX3_POS, sprite3->position() );
    this->expected_action_params(action1.get(), 2);
    this->expected_common_params(translate2.get(), DURATION, SPEED_MOD1);
    this->expected_common_params(translate3.get(), DURATION, SPEED_MOD1);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_update_callback( [=](float) {

    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false )
    {
      this->quit();
    }
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangles
    if( sprite0 != nullptr ) {  // action0
      sprite0->draw( this->render_window() );
    }

    if( sprite1 != nullptr ) {  // action1
      sprite1->draw( this->render_window() );
    }

    if( sprite2 != nullptr ) {  // action2
      sprite2->draw( this->render_window() );
    }

    if( sprite3 != nullptr ) {  // action3
      sprite3->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, RemoveAction)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const Point2i TRANSLATE_POS(Point2i::zero);

  auto translate0 =
    nom::create_action<MoveByAction>(nullptr, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate0 != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate0}, "action0" );
  ASSERT_TRUE(action0 != nullptr);

  auto action1 =
    nom::create_action<SequenceAction>( { translate0->clone() }, "action1" );
  ASSERT_TRUE(action1 != nullptr);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0);
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(true, this->player.remove_action("action0") );
  EXPECT_EQ(false, this->player.remove_action("action1") );

  this->run_action_ret =
  this->player.run_action(action1);
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(true, this->player.remove_action("action1") );
}

// NOTE: These logs show the inaccurate results previous to bug fixes!!
// erasing action [ 1 / 1 ] [id]: ConcurrentGroupActions_action1 [type]: GROUP
// erasing from queue [id]: ConcurrentGroupActions_action1 [type]: GROUP
// erasing action [ 1 / 1 ] [id]: ConcurrentGroupActions_action2 [type]: GROUP
// erasing from queue [id]: ConcurrentGroupActions_action2 [type]: GROUP
// erasing action [ 1 / 1 ] [id]: ConcurrentGroupActions_action3 [type]: GROUP
// erasing from queue [id]: ConcurrentGroupActions_action3 [type]: GROUP
// erasing action [ 1 / 1 ] [id]: ConcurrentGroupActions_action0 [type]: GROUP
// erasing from queue [id]: ConcurrentGroupActions_action0 [type]: GROUP
TEST_F(AnimationTest, ConcurrentGroupActions)
{
  // Testing parameters
  const real32 DURATION = 1.0f;
  const real32 SPEED_MOD0 = NOM_ANIM_TEST_FLAG(speed) * 1.0f;     // action0
  const real32 SPEED_MOD1 = NOM_ANIM_TEST_FLAG(speed) * 2.0f;     // action1
  const real32 SPEED_MOD2 = NOM_ANIM_TEST_FLAG(speed) * 2.25f;    // action2
  const real32 SPEED_MOD3 = NOM_ANIM_TEST_FLAG(speed) * 1.25f;    // action3
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

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

  auto sprite0 = std::make_shared<Sprite>( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = std::make_shared<Sprite>( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = std::make_shared<Sprite>( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = std::make_shared<Sprite>( rect3->texture() );
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

  auto action0 = nom::create_action<GroupAction>( {translate0}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD0);

  auto action1 = nom::create_action<GroupAction>( {translate1}, "action1" );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_speed(SPEED_MOD1);
  action1->set_timing_mode(TIMING_MODE);

  auto action2 = nom::create_action<GroupAction>( {translate2}, "action2" );
  ASSERT_TRUE(action2 != nullptr);
  action2->set_speed(SPEED_MOD2);
  action2->set_timing_mode(TIMING_MODE);

  auto action3 = nom::create_action<GroupAction>( {translate3}, "action3" );
  ASSERT_TRUE(action3 != nullptr);
  action3->set_speed(SPEED_MOD3);
  action3->set_timing_mode(TIMING_MODE);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action1.get(), 1);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD1);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action2, [=]() {

    EXPECT_EQ( EXPECTED_TEX2_POS, sprite2->position() );
    this->expected_action_params(action2.get(), 1);
    this->expected_common_params(translate2.get(), DURATION, SPEED_MOD2);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action2";
  EXPECT_EQ(3, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action3, [=]() {

    EXPECT_EQ( EXPECTED_TEX3_POS, sprite3->position() );
    this->expected_action_params(action3.get(), 1);
    this->expected_common_params(translate3.get(), DURATION, SPEED_MOD3);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action3";
  EXPECT_EQ(4, this->player.num_actions() );

  this->append_update_callback( [=](float) {

    uint32 last_delta = nom::ticks();

    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false &&
        this->player.action_running("action2") == false &&
        this->player.action_running("action3") == false )
    {
      // Try to check for minimum test duration; the smallest speed modifier
      // should always be used for this calculation!
      if( last_delta < ( (DURATION * 1000) / SPEED_MOD0) ) {
        EXPECT_EQ( ( (DURATION * 1000) / SPEED_MOD0), last_delta);
      }

      this->quit();
    }
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangles
    if( sprite0 != nullptr ) {  // action0
      sprite0->draw( this->render_window() );
    }

    if( sprite1 != nullptr ) {  // action1
      sprite1->draw( this->render_window() );
    }

    if( sprite2 != nullptr ) {  // action2
      sprite2->draw( this->render_window() );
    }

    if( sprite3 != nullptr ) {  // action3
      sprite3->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

// erasing action [ 1 / 2 ] [id]: ConcurrentSequenceActions_action1 [type]: SEQUENCE
// erasing action [ 1 / 2 ] [id]: ConcurrentSequenceActions_action0 [type]: SEQUENCE
// erasing action [ 2 / 2 ] [id]: ConcurrentSequenceActions_action1 [type]: SEQUENCE
// erasing from queue [id]: ConcurrentSequenceActions_action1 [type]: SEQUENCE
// erasing action [ 2 / 2 ] [id]: ConcurrentSequenceActions_action0 [type]: SEQUENCE
// erasing from queue [id]: ConcurrentSequenceActions_action0 [type]: SEQUENCE
TEST_F(AnimationTest, ConcurrentSequenceActions)
{
  // Testing parameters
  const float DURATION = 1.0f;
  const real32 SPEED_MOD0 = NOM_ANIM_TEST_FLAG(speed) * 1.0f;   // action0(2/2)
  const real32 SPEED_MOD1 = NOM_ANIM_TEST_FLAG(speed) * 2.0f;   // action0(1/2)
  const real32 SPEED_MOD2 = NOM_ANIM_TEST_FLAG(speed) * 2.25f;  // action1(1/2)
  const real32 SPEED_MOD3 = NOM_ANIM_TEST_FLAG(speed) * 1.25f;  // action1(2/2)
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

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

  auto sprite0 = std::make_shared<Sprite>( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = std::make_shared<Sprite>( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = std::make_shared<Sprite>( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = std::make_shared<Sprite>( rect3->texture() );
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
    nom::create_action<SequenceAction>( {translate0, translate1}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);

  auto action1 =
    nom::create_action<SequenceAction>( {translate2, translate3}, "action1" );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_mode(TIMING_MODE);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD1);
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
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_update_callback( [=](float) {

    uint32 last_delta = nom::ticks();

    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false )
    {
      // Try to check for minimum test duration; the smallest speed modifier
      // should always be used for this calculation!
      if( last_delta < ( (DURATION * 1000) / SPEED_MOD0) ) {
        EXPECT_EQ( ( (DURATION * 1000) / SPEED_MOD0), last_delta);
      }

      this->quit();
    }
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangles
    if( sprite0 != nullptr ) {  // action0
      sprite0->draw( this->render_window() );
    }

    if( sprite1 != nullptr ) {  // action01
      sprite1->draw( this->render_window() );
    }

    if( sprite2 != nullptr ) {  // action1
      sprite2->draw( this->render_window() );
    }

    if( sprite3 != nullptr ) {  // action1
      sprite3->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

// erasing action [ 1 / 2 ] [id]: action1 [type]: SEQUENCE
// erasing action [ 1 / 2 ] [id]: action0 [type]: GROUP
// erasing action [ 2 / 2 ] [id]: action0 [type]: GROUP
// erasing from queue [id]: action0 [type]: GROUP
// erasing action [ 2 / 2 ] [id]: action1 [type]: SEQUENCE
// erasing from queue [id]: action1 [type]: SEQUENCE
TEST_F(AnimationTest, ConcurrentGroupAndSequenceActions)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const real32 SPEED_MOD0 = NOM_ANIM_TEST_FLAG(speed) * 1.0f; // action0
  const real32 SPEED_MOD1 = NOM_ANIM_TEST_FLAG(speed) * 2.0f; // action0
  const real32 SPEED_MOD2 = NOM_ANIM_TEST_FLAG(speed) * 2.25f; // action1
  const real32 SPEED_MOD3 = NOM_ANIM_TEST_FLAG(speed) * 1.25f; // action1
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

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

  auto sprite0 = std::make_shared<Sprite>( rect0->texture() );
  ASSERT_TRUE(sprite0 != nullptr);

  auto sprite1 = std::make_shared<Sprite>( rect1->texture() );
  ASSERT_TRUE(sprite1 != nullptr);

  auto sprite2 = std::make_shared<Sprite>( rect2->texture() );
  ASSERT_TRUE(sprite2 != nullptr);

  auto sprite3 = std::make_shared<Sprite>( rect3->texture() );
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
    nom::create_action<GroupAction>( {translate0, translate1}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);

  auto action1 =
    nom::create_action<SequenceAction>( {translate2, translate3}, "action1" );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_mode(TIMING_MODE);

  // EXPECT_EQ( ActionPlayer::IDLE, this->player.player_state() );
  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX0_POS, sprite0->position() );
    EXPECT_EQ( EXPECTED_TEX1_POS, sprite1->position() );
    this->expected_action_params(action0.get(), 2);
    this->expected_common_params(translate0.get(), DURATION, SPEED_MOD0);
    this->expected_common_params(translate1.get(), DURATION, SPEED_MOD1);
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
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action1";
  EXPECT_EQ(2, this->player.num_actions() );
  // EXPECT_EQ( ActionPlayer::IDLE, this->player.player_state() );

  this->append_update_callback( [=](float) {

    uint32 last_delta = nom::ticks();
    if( this->player.action_running("action0") == false &&
        this->player.action_running("action1") == false )
    {
      // EXPECT_EQ( ActionPlayer::IDLE, this->player.player_state() );

      // Try to check for minimum test duration; the smallest speed modifier
      // should always be used for this calculation!
      if( last_delta < ( (DURATION * 1000) / SPEED_MOD0) ) {
        EXPECT_EQ( ( (DURATION * 1000) / SPEED_MOD0), last_delta);
      }

      this->quit();
    }
  });

  this->append_render_callback( [=](const RenderWindow& win) {

    // action0
    if( sprite0 != nullptr ) {
      sprite0->draw( this->render_window() );
    }

    // action0
    if( sprite1 != nullptr ) {
      sprite1->draw( this->render_window() );
    }

    // action1
    if( sprite2 != nullptr ) {
      sprite2->draw( this->render_window() );
    }

    // action1
    if( sprite3 != nullptr ) {
      sprite3->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, RunActionWithName)
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

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, "action0");
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  EXPECT_EQ(true, this->player.action_running("action0") );
  EXPECT_FALSE(action0.get() == this->player.action("action0") )
  << "action0 should not be equal to the returned action; ::run_action should"
  << "be cloning the instance";

  EXPECT_EQ(1, this->player.num_actions() );
}

} // namespace nom
