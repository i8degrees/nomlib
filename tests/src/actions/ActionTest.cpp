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

// extern initialization
ActionTestFlags anim_test_flags = {};

ActionTest::ActionTest()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);

  // We do not utilize screen dump comparison features for any of these tests,
  // so we might as well leave the logic disabled...
  NOM_TEST_FLAG(disable_comparison) = true;

  // These tests **must** always run with the interactive flag set, so that the
  // main loop does not prematurely terminate on us before we are done
  // executing the animation's loop; we explicitly terminate each test upon
  // the appropriate completion conditions.
  NOM_TEST_FLAG(interactive) = true;

  // Enable debug diagnostics for action objects
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION, nom::NOM_LOG_PRIORITY_DEBUG);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION, nom::NOM_LOG_PRIORITY_VERBOSE);

  // Enable debug diagnostics for action engine && internal queue
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_QUEUE, nom::NOM_LOG_PRIORITY_DEBUG);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_PLAYER, nom::NOM_LOG_PRIORITY_DEBUG);

  // Enable full debug diagnostics for action engine && internal queue
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_QUEUE, nom::NOM_LOG_PRIORITY_VERBOSE);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_PLAYER, nom::NOM_LOG_PRIORITY_VERBOSE);

  // Enable debug call stack diagnostics
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_ACTION, NOM_LOG_PRIORITY_VERBOSE);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_RENDER, NOM_LOG_PRIORITY_VERBOSE);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);

  // Disable NOM_LOG_CATEGORY_TEST logging for these tests
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST, NOM_LOG_PRIORITY_CRITICAL);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST, NOM_LOG_PRIORITY_DEBUG);

  if( NOM_ACTION_TEST_FLAG(enable_vsync) == true ) {
    nom::set_hint(SDL_HINT_RENDER_VSYNC, "1");
  } else {
    nom::set_hint(SDL_HINT_RENDER_VSYNC, "0");
  }

  // I get a subtle animation flicker when toggled full screen when using
  // nearest scale quality
  if( nom::set_hint(SDL_HINT_RENDER_SCALE_QUALITY, "linear") == false ) {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not set scale quality to linear;",
                  "animation flicker may result!" );
  }
}

ActionTest::~ActionTest()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);

  NOM_LOG_DEBUG( NOM_LOG_CATEGORY_ACTION,
                "Number of actions remaining in queue at the time of exit:",
                this->player.num_actions() );
}

bool ActionTest::init_rendering()
{
  uint32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

  // Initialize rendering window (and its GL context)
  if( this->window_.create( this->test_set(),
      this->resolution(), window_flags ) == false )
  {
    return false;
  }

  // Allow for automatic rescaling of the output window based on aspect
  // ratio (i.e.: handle full-screen resizing); this will use letter-boxing
  // when the aspect ratio is greater than what is available, or side-bars
  // when the aspect ratio is less than.
  this->render_window().set_logical_size( this->resolution() );

  // Use no pixel unit scaling; this gives us one to one pixel ratio
  this->render_window().set_scale( nom::Point2f(1,1) );

  // Try to set a sensible (optimal) refresh rate based on the display
  // capabilities when VSYNC is enabled.
  if( NOM_ACTION_TEST_FLAG(enable_vsync) == true ) {
    auto display_refresh_rate =
      this->render_window().refresh_rate();
    if( display_refresh_rate > 0 ) {
      NOM_ACTION_TEST_FLAG(fps) = display_refresh_rate;
    } else {
      // ...fall back to using the initialized value of the FPS test flag
      NOM_ASSERT( NOM_ACTION_TEST_FLAG(fps) > 0);
    }
  }

  return true;
}

void ActionTest::SetUp()
{
  // NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);

  std::string res_file = nom::UnitTest::test_set() + ".json";

  if( resources[0].load_file( res_file, "resources" ) == false ) {
    FAIL()
    << "Could not resolve the 'resources' path from input file: " << res_file;
  }

  if( resources[1].load_file( res_file, "fonts" ) == false ) {
    FAIL()
    << "Could not resolve the 'fonts' path from input file: " << res_file;
  }

  // VisualUnitTest environment init
  VisualUnitTest::SetUp();

  // Interactive toggling of the animation player states; pausing, stopping
  // and clearing of the animation loop
  this->append_event_callback( [=](const Event ev) {
    uint32 pstate = this->player.player_state();
    switch(ev.type) {
      default: break;

      case SDL_KEYDOWN:
      {
        switch(ev.key.sym) {
          default: break;

          case SDLK_1:
          {
            if( pstate != ActionPlayer::State::PAUSED ) {
              this->player.pause();
            } else if( pstate == ActionPlayer::State::PAUSED ) {
              this->player.resume();
            }
          } break;

          case SDLK_2:
          {
            if( pstate != ActionPlayer::State::STOPPED ) {
              this->player.stop();
            } else if( pstate == ActionPlayer::State::STOPPED ) {
              this->player.resume();
            }
          } break;

          case SDLK_3:
          {
            this->player.cancel_actions();
          } break;

        } break; // end switch ev.key.sym
      } break; // end SDL_KEYDOWN case

      case SDL_WINDOWEVENT:
      {
        switch(ev.window.event) {
          default: break;

          // This event will always be posted first
          case SDL_WINDOWEVENT_RESIZED:
          {
            this->player.pause();
          } break;

          case SDL_WINDOWEVENT_SIZE_CHANGED:
          {
            this->player.resume();
          } break;
        } break; // end switch ev.window.event
      } break; // end SDL_WINDOWEVENT case

    } // end switch ev.type
  });

  Timer anim_timer;
  anim_timer.start();
  uint32 last_delta = anim_timer.ticks();
  this->append_update_callback( [&, anim_timer, last_delta](float) mutable {

    uint32 end_delta = anim_timer.ticks();
    uint32 elapsed_delta = end_delta - last_delta;
    last_delta = end_delta;

    // NOM_DUMP(elapsed_delta);
    this->player.update(elapsed_delta);
  });
}

void ActionTest::TearDown()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void ActionTest::SetUpTestCase()
{
  // NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);
}

void ActionTest::TearDownTestCase()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);
}

void
ActionTest::expected_common_params( const IActionObject* obj,
                                    float duration, float speed,
                                    const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_common_params: " << scope_name << "\n";

  EXPECT_EQ(speed, obj->speed() )
  << "expected_common_params: " << scope_name << "\n";

  EXPECT_EQ(duration, obj->duration() )
  << "expected_common_params scoped_name: " << scope_name << "\n";
}

void ActionTest::expected_action_params(  const GroupAction* action,
                                          nom::size_type size,
                                          const std::string& scope_name )
{
  ASSERT_TRUE(action != nullptr)
  << "expected_action_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(size, action->actions_.size() )
  << "expected_action_params scoped_name: " << scope_name << "\n";
}

void ActionTest::expected_action_params(  const SequenceAction* action,
                                          nom::size_type size,
                                          const std::string& scope_name )
{
  ASSERT_TRUE(action != nullptr)
  << "expected_action_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(size, action->actions_.size() )
  << "expected_action_params scoped_name: " << scope_name << "\n";
}

void
ActionTest::expected_alpha_in_params( const FadeInAction* obj,
                                      int16 alpha, const Sprite* tex,
                                      const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_alpha_in_params scoped_name: " << scope_name << "\n";

  if( obj != nullptr ) {
    EXPECT_EQ(alpha, obj->alpha_)
    << "expected_alpha_in_params scoped_name: " << scope_name << "\n";
  }

  if( tex != nullptr ) {
    EXPECT_EQ(alpha, tex->alpha() )
    << "scoped_name: " << scope_name << "\n";
  }
}

void
ActionTest::expected_alpha_out_params(  const FadeOutAction* obj,
                                        int16 alpha, const Sprite* tex,
                                        const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_alpha_out_params scoped_name: " << scope_name << "\n";

  if( obj != nullptr ) {
    EXPECT_EQ(alpha, obj->alpha_)
    << "expected_alpha_out_params scoped_name: " << scope_name << "\n";
  }

  if( tex != nullptr ) {
    EXPECT_EQ(alpha, tex->alpha() )
    << "expected_alpha_out_params scoped_name: " << scope_name << "\n";
  }
}

void
ActionTest::expected_alpha_by_params( const FadeAlphaByAction* obj,
                                      int16 alpha, const Sprite* tex,
                                      const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_alpha_by_params scoped_name: " << scope_name << "\n";

  if( obj != nullptr ) {
    EXPECT_EQ(alpha, obj->alpha_)
    << "expected_alpha_by_params scoped_name: " << scope_name << "\n";
  }

  if( tex != nullptr ) {
    EXPECT_EQ(alpha, tex->alpha() )
    << "expected_alpha_by_params scoped_name: " << scope_name << "\n";
  }
}

void ActionTest::expected_repeat_params(  const RepeatForAction* obj,
                                          uint32 num_repeats,
                                          const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_repeat_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(num_repeats, obj->num_repeats_)
  << "expected_repeat_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(num_repeats, obj->elapsed_repeats_)
  << "expected_repeat_params scoped_name: " << scope_name << "\n";
}

void ActionTest::expected_repeat_params(  const RepeatForeverAction* obj,
                                          uint32 num_repeats,
                                          const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_repeat_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(num_repeats, obj->elapsed_repeats_)
  << "expected_repeat_params scoped_name: " << scope_name << "\n";
}

void ActionTest::
expected_sprite_textures_params(  const SpriteTexturesAction* obj,
                                  nom::size_type num_frames,
                                  real32 duration, real32 speed,
                                  const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_sprite_textures_action_params scoped_name: "
  << scope_name << "\n";

  EXPECT_EQ(num_frames, obj->frames_.size() )
  << "expected_sprite_textures_action_params scoped_name: "
  << scope_name << "\n";

  EXPECT_EQ(duration, obj->duration() )
  << "expected_sprite_textures_action_params scoped_name: "
  << scope_name << "\n";

  EXPECT_EQ(speed, obj->speed() )
  << "expected_sprite_textures_action_params scoped_name: "
  << scope_name << "\n";
}

void
ActionTest::expected_sprite_batch_action_params(  const SpriteBatchAction* obj,
                                                  nom::size_type num_frames,
                                                  real32 duration,
                                                  real32 speed,
                                                  const
                                                  std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_sprite_batch_action_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(num_frames, obj->drawable_->frames() )
  << "expected_sprite_batch_action_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(duration, obj->duration() )
  << "expected_sprite_batch_action_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(speed, obj->speed() )
  << "expected_sprite_batch_action_params scoped_name: " << scope_name << "\n";
}

// NOTE: This implementation derives from [Handmade Hero](https://www.handmadehero.org/)'s
// "Enforcing a Video Frame Rate" (Week 4). It is currently assumed that
// "granular sleep" AKA high-resolution timing is properly supported by the
// platform -- this might come back to bite us in the ass someday!
void ActionTest::set_frame_interval(uint32 interval)
{
  real32 target_seconds_per_frame =
    1.0f / (real32)interval;
  uint64 last_delta = 0;
  HighResolutionTimer anim_timer;

  // Abort our frame capping logic when explicitly requested
  if( interval == 0 ) {
    return;
  }

  anim_timer.start();

  last_delta = anim_timer.ticks();

  real32 elapsed_delta =
    HighResolutionTimer::elapsed_ticks(last_delta, anim_timer.ticks() );

  if(elapsed_delta < target_seconds_per_frame) {

    uint32 sleep_ms =
      (uint32)(1000.0f * (target_seconds_per_frame - elapsed_delta) );
    if(sleep_ms > 0) {
      nom::sleep(sleep_ms);
    }
  }
}

void
ActionTest::init_sprite_action_test(  const std::vector<const char*>&
                                      texture_filenames, texture_frames&
                                      anim_frames )
{
  for(  auto itr = texture_filenames.begin();
        itr != texture_filenames.end();
        ++itr )
  {
    ASSERT_TRUE(*itr != nullptr);

    auto tex = std::make_shared<Texture>();
    ASSERT_TRUE(tex != nullptr);

    bool ret = tex->load(this->resources[0].path() + *itr );
    if( ret == false ) {
      FAIL()  << "Could not load texture" << *itr << "from file path: "
              << this->resources[0].path() << "\n";
    }

    auto sprite = std::make_shared<Sprite>();
    ASSERT_TRUE(sprite != nullptr);
    EXPECT_EQ(true, sprite->set_texture(tex) );

    nom::set_alignment( sprite.get(), Point2i::zero, this->WINDOW_DIMS,
                        Anchor::MiddleCenter );

    anim_frames.emplace_back(sprite);
  } // end for tex filenames loop
}

bool init_cmd_line_args(int argc, char** argv)
{
  // Append additional command line arguments for this test case
  TCLAPArgs args;

  TCLAP::ValueArg<float> speed_modifier_arg(
    // Option short form (not supported)
    "",
    // Option long form
    "speed",
    // Option description
    "The speed modifier used by each test",
    // Not required
    false,
    // Option default
    NOM_ACTION_TEST_FLAG(speed),
    // Option example (part of description)
    "32-bit floating-point number"
  );

  std::stringstream timing_mode_arg_desc;
  timing_mode_arg_desc  << "One of the easing algorithms: linear (default), "
                        << "quad_ease_in, quad_ease_out, quad_ease_in_out, "
                        << "cubic_ease_in, cubic_ease_out, cubic_ease_in_out, "
                        << "quart_ease_in, quart_ease_out, quart_ease_in_out, "
                        << "quint_ease_in, quint_ease_out, quint_ease_in_out, "
                        << "back_ease_in, back_ease_out, back_ease_in_out, "
                        << "bounce_ease_in, bounce_ease_out, bounce_ease_in_out, "
                        << "circ_ease_in, circ_ease_out, circ_ease_in_out, "
                        << "elastic_ease_in, elastic_ease_out, elastic_ease_in_out, "
                        << "expo_ease_in, expo_ease_out, expo_ease_in_out, "
                        << "sine_ease_in, sine_ease_out, sine_ease_in_out";
  TCLAP::ValueArg<std::string> timing_mode_arg(
    // Option short form (not supported)
    "",
    // Option long form
    "timing-mode",
    // Option description
    "The timing mode used by every test",
    // Not required
    false,
    // Option default
    "linear_ease_in_out",
    // Option example (part of description)
    timing_mode_arg_desc.str().c_str()
  );

  TCLAP::ValueArg<uint32> fps_arg(
    // Option short form (not supported)
    "",
    // Option long form
    "fps",
    // Option description
    "The frame interval to update by for every test",
    // Not required
    false,
    // Option default
    NOM_ACTION_TEST_FLAG(fps),
    // Option example (part of description)
    "30, 60, 90, 120, ..."
  );

  std::stringstream vsync_arg_desc;
  vsync_arg_desc  << "Try to always render the action during the monitor's "
                  << "VSYNC period -- enabling this option will override any "
                  << "end-user specified FPS value when a sensible value can "
                  << "be determined from the display hardware.";
  TCLAP::SwitchArg vsync_arg(
    // Option short form (not supported)
    "",
    // Option long form
    "enable-vsync",
    // Option description
    vsync_arg_desc.str().c_str(),
    // Option default
    NOM_ACTION_TEST_FLAG(enable_vsync)
  );

  args.push_back(&speed_modifier_arg);
  args.push_back(&timing_mode_arg);
  args.push_back(&fps_arg);
  args.push_back(&vsync_arg);

  // nom::UnitTest framework integration
  nom::init_test(argc, argv, args);

  if( speed_modifier_arg.getValue() != NOM_ACTION_TEST_FLAG(speed) ) {
    NOM_ACTION_TEST_FLAG(speed) = speed_modifier_arg.getValue();
  }

  // nom::Linear is the default timing mode when none is given
  nom::IActionObject::timing_curve_func mode =
    nom::timing_curve_from_str( timing_mode_arg.getValue() );

  NOM_ACTION_TEST_FLAG(timing_curve) = mode;
  NOM_ACTION_TEST_FLAG(timing_mode_str) = timing_mode_arg.getValue();

  uint32 fps = fps_arg.getValue();
  if( fps != NOM_ACTION_TEST_FLAG(fps) ) {
    NOM_ACTION_TEST_FLAG(fps) = fps;
  }

  NOM_ACTION_TEST_FLAG(enable_vsync) = vsync_arg.getValue();

  return true;
}

IActionObject::timing_curve_func
timing_curve_from_str(const std::string& timing_mode)
{
  // Default timing mode
  IActionObject::timing_curve_func mode =
    nom::Linear::ease_in_out;

  if( timing_mode == "quad_ease_in" ) {
    mode = nom::Quad::ease_in;
  } else if( timing_mode == "quad_ease_out" ) {
    mode = nom::Quad::ease_out;
  } else if( timing_mode == "quad_ease_in_out" ) {
    mode = nom::Quad::ease_in_out;
  } else if( timing_mode == "cubic_ease_in" ) {
    mode = nom::Cubic::ease_in;
  } else if( timing_mode == "cubic_ease_out" ) {
    mode = nom::Cubic::ease_out;
  } else if( timing_mode == "cubic_ease_in_out" ) {
    mode = nom::Cubic::ease_in_out;
  } else if( timing_mode == "quart_ease_in" ) {
    mode = nom::Quart::ease_in;
  } else if( timing_mode == "quart_ease_out" ) {
    mode = nom::Quart::ease_out;
  } else if( timing_mode == "quart_ease_in_out" ) {
    mode = nom::Quart::ease_in_out;
  } else if( timing_mode == "quint_ease_in" ) {
    mode = nom::Quint::ease_in;
  } else if( timing_mode == "quint_ease_out" ) {
    mode = nom::Quint::ease_out;
  } else if( timing_mode == "quint_ease_in_out" ) {
    mode = nom::Quint::ease_in_out;
  } else if( timing_mode == "back_ease_in" ) {
    mode = nom::Back::ease_in;
  } else if( timing_mode == "back_ease_out" ) {
    mode = nom::Back::ease_out;
  } else if( timing_mode == "back_ease_in_out" ) {
    mode = nom::Back::ease_in_out;
  } else if( timing_mode == "bounce_ease_in" ) {
    mode = nom::Bounce::ease_in;
  } else if( timing_mode == "bounce_ease_out" ) {
    mode = nom::Bounce::ease_out;
  } else if( timing_mode == "bounce_ease_in_out" ) {
    mode = nom::Bounce::ease_in_out;
  } else if( timing_mode == "circ_ease_in" ) {
    mode = nom::Circ::ease_in;
  } else if( timing_mode == "circ_ease_out" ) {
    mode = nom::Circ::ease_out;
  } else if( timing_mode == "circ_ease_in_out" ) {
    mode = nom::Circ::ease_in_out;
  } else if( timing_mode == "elastic_ease_in" ) {
    mode = nom::Elastic::ease_in;
  } else if( timing_mode == "elastic_ease_out" ) {
    mode = nom::Elastic::ease_out;
  } else if( timing_mode == "elastic_ease_in_out" ) {
    mode = nom::Elastic::ease_in_out;
  } else if( timing_mode == "expo_ease_in" ) {
    mode = nom::Expo::ease_in;
  } else if( timing_mode == "expo_ease_out" ) {
    mode = nom::Expo::ease_out;
  } else if( timing_mode == "expo_ease_in_out" ) {
    mode = nom::Expo::ease_in_out;
  } else if( timing_mode == "sine_ease_in" ) {
    mode = nom::Sine::ease_in;
  } else if( timing_mode == "sine_ease_out" ) {
    mode = nom::Sine::ease_out;
  } else if( timing_mode == "sine_ease_in_out" ) {
    mode = nom::Sine::ease_in_out;
  }

  return mode;
}

/// \brief Test animation timing sanity.
TEST_F(ActionTest, WaitForDurationAction2s)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = 1.0f;
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  auto idle2s =
    nom::create_action<WaitForDurationAction>( WaitForDurationAction(DURATION) );
  ASSERT_TRUE(idle2s != nullptr);

  auto action0 =
    nom::create_action<SequenceAction>( {idle2s}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(idle2s.get(), DURATION, SPEED_MOD);
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  uint32 action_start_time = nom::ticks();
  uint32 action_done_time = 0;

  // Do not terminate loop until we have the animation object's DONE timestamp
  // recorded, so we can ensure proper operational values.
  this->append_update_callback( [=,&action_done_time](float) {

    // NOTE: We must additionally check for the state of the action queue
    // because our action will not yet be running on the first time around on
    // the main loop's update loop.
    if( this->player.idle() == true &&
        this->player.action_running("action0") == false )
    {
      action_done_time = nom::ticks();
      EXPECT_GT(action_done_time, action_start_time);
      EXPECT_GT(action_done_time, 2000);
      EXPECT_EQ(0, this->player.num_actions() );

      this->quit(); // graceful exit
    }
  });

  this->append_render_callback( [=](const RenderWindow&) {
    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks Thanks goes to Tim Jones of [sdltutorials.com](http://www.sdltutorials.com/sdl-animation)
/// for the sprite frames of Yoshi chosen for this test!
TEST_F(ActionTest, SpriteTexturesAction)
{
  // Testing parameters
  texture_frames anim_frames;
  const std::vector<const char*> texture_filenames = {{
    "yoshi_000.png", "yoshi_001.png", "yoshi_002.png", "yoshi_003.png",
    "yoshi_004.png", "yoshi_005.png", "yoshi_006.png", "yoshi_007.png"
  }};

  // fps per texture
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
    nom::create_action<GroupAction>( {tex_bg}, "action0" );
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
    this->expected_action_params(action0.get(), 1);

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

TEST_F(ActionTest, MoveByAction)
{
  // Testing parameters
  const float DURATION = 2.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const Point2i TRANSLATE_POS( Point2i(200,0) );
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Green) );
  ASSERT_TRUE(rect != nullptr);

  auto sprite = std::make_shared<Sprite>( rect->texture() );
  ASSERT_TRUE(sprite != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate}, nom::UnitTest::test_name() );
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
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=, &sprite](const RenderWindow& win) {
    // Render our animation's rectangle
    if( sprite != nullptr ) {
      sprite->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, MoveByActionNegativeXDelta)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i RECT_POS(WINDOW_DIMS.w, 0);
  const Point2i TRANSLATE_POS( Point2i(-WINDOW_DIMS.w,0) );
  const Point2i EXPECTED_TEX_POS(0,0);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Green) );
  ASSERT_TRUE(rect != nullptr);

  auto sprite = std::make_shared<Sprite>( rect->texture() );
  ASSERT_TRUE(sprite != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate}, nom::UnitTest::test_name() );
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

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
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

TEST_F(ActionTest, MoveByActionWithNegativeYDelta)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h/4);
  const Point2i RECT_POS(0, WINDOW_DIMS.h);
  const Point2i TRANSLATE_POS( Point2i(0,-WINDOW_DIMS.h) );
  const Point2i EXPECTED_TEX_POS(0,0);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Green) );
  ASSERT_TRUE(rect != nullptr);

  auto sprite = std::make_shared<Sprite>( rect->texture() );
  ASSERT_TRUE(sprite != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(sprite, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate}, nom::UnitTest::test_name() );
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

TEST_F(ActionTest, ScaleByAction)
{
  // Testing parameters
  const float DURATION = 1.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const Size2f SCALE_FACTOR(2.0f, 2.0f);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(128, 128);
  const Size2i EXPECTED_TEX_SIZE( Size2i( TEX_SIZE.w * SCALE_FACTOR.w,
                                          TEX_SIZE.h * SCALE_FACTOR.h )
  );

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex = rect->texture();

  auto sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto scale_by =
    nom::create_action<ScaleByAction>(sprite, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {scale_by}, nom::UnitTest::test_name());
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

TEST_F(ActionTest, ScaleByActionWithNegativeFactor)
{
  // Testing parameters
  const float DURATION = 1.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);
  const Size2f SCALE_FACTOR(-3.0f, -3.0f);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(128, 128);
  const Size2i EXPECTED_TEX_SIZE(
    nom::round_float<int>( (real32)(TEX_SIZE.w) / abs(SCALE_FACTOR.w) ),
    nom::round_float<int>( (real32)(TEX_SIZE.h) / abs(SCALE_FACTOR.h) )
  );

  std::string TEX_FILE_PATH = resources[0].path() + "card.png";

  auto tex = std::make_shared<Texture>( Texture() );
  ASSERT_TRUE(tex != nullptr);
  if( tex->load(TEX_FILE_PATH) == false ) {
    FAIL() << "Could not load texture from file: " << TEX_FILE_PATH;
  }

  auto sprite =
    std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto scale_by =
    nom::create_action<nom::ScaleByAction>(sprite, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by != nullptr);
  scale_by->set_name("scale_by");

  auto action0 =
    nom::create_action<GroupAction>( {scale_by}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(EXPECTED_TEX_SIZE, sprite->size() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(scale_by.get(), DURATION, SPEED_MOD);

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

TEST_F(ActionTest, ScaleByActionWithNonPowerOfTwo)
{
  // Testing parameters
  const float DURATION = 1.5f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const Size2f SCALE_FACTOR(2.25f, 1.75f);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(129, 129);
  const Size2i EXPECTED_TEX_SIZE =
    Size2i( TEX_SIZE.w * SCALE_FACTOR.w,
            nom::round_float<int>(TEX_SIZE.h * SCALE_FACTOR.h) );

  auto rect = std::make_shared<Rectangle>(
    IntRect(TEX_POS, TEX_SIZE), Color4i::Blue);
  ASSERT_TRUE(rect != nullptr);

  auto tex = rect->texture();

  auto sprite = std::make_shared<Sprite>();
  ASSERT_TRUE(sprite != nullptr);
  EXPECT_EQ(true, sprite->set_texture(tex) );

  auto scale_by =
    nom::create_action<ScaleByAction>(sprite, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by != nullptr);
  scale_by->set_name("scale_by");

  auto action0 =
    nom::create_action<GroupAction>( {scale_by}, nom::UnitTest::test_name() );
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

TEST_F(ActionTest, CallbackActionDefaultDuration)
{
  // Testing parameters
  const float DURATION = 0.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  CallbackAction::callback_type callback_func;
  callback_func = [=]() {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "Hello, there!" );
  };

  auto anim0 = nom::create_action<CallbackAction>(callback_func);

  auto action0 =
    nom::create_action<GroupAction>( {anim0}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(anim0.get(), DURATION, SPEED_MOD);

    this->quit();
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {
    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \brief Outputs "Hello, there!" to the console several times.
TEST_F(ActionTest, CallbackActionWithNonZeroDuration)
{
  // Testing parameters
  const float DURATION = 0.05f;
  // const float DURATION = 1.0f;
  const float SPEED_MOD = NOM_ACTION_TEST_FLAG(speed);
  const IActionObject::timing_curve_func TIMING_MODE =
    NOM_ACTION_TEST_FLAG(timing_curve);
  const uint32 FPS = NOM_ACTION_TEST_FLAG(fps);

  CallbackAction::callback_type callback_func;
  callback_func = [=]() {
    std::cout << "Hello, there!\n";
  };

  auto anim0 = nom::create_action<CallbackAction>(DURATION, callback_func);

  auto action0 =
    nom::create_action<GroupAction>( {anim0}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_curve(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(anim0.get(), DURATION, SPEED_MOD);

    this->quit();
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {
    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(ActionTest, ColorizeAction)
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
    TEX_END_COLOR, BLEND_MODE, DURATION);
  ASSERT_TRUE(colorize != nullptr);
  colorize->set_name("colorize");
  colorize->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(colorize, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    EXPECT_EQ(TEX_END_COLOR, sprite->color() );
    EXPECT_EQ(BLEND_MODE, sprite->color_blend_mode() );
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

} // namespace nom

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  // NOTE: The animated full-screen delay on OS X causes us to miss several key
  // frames anytime it is toggled
  nom::set_hint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0");

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init(argc, argv) == false ) {
    NOM_LOG_CRIT(NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib.");
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  if( nom::init_cmd_line_args(argc, argv) == false ) {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not initialize unit testing framework." );
    return NOM_EXIT_FAILURE;
  }

  return RUN_ALL_TESTS();
}
