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

// extern initialization
AnimTestFlags anim_test_flags = {};

AnimationTest::AnimationTest()
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

  // Enable debug diagnostics for action animation objects
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ANIMATION, nom::NOM_LOG_PRIORITY_DEBUG);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ANIMATION, nom::NOM_LOG_PRIORITY_VERBOSE);

  // Enable debug diagnostics
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_QUEUE, nom::NOM_LOG_PRIORITY_DEBUG);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_PLAYER, nom::NOM_LOG_PRIORITY_DEBUG);

  // Enable full debug diagnostics
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_QUEUE, nom::NOM_LOG_PRIORITY_VERBOSE);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_ACTION_PLAYER, nom::NOM_LOG_PRIORITY_VERBOSE);

  // Enable debug call stack diagnostics
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_ANIMATION, NOM_LOG_PRIORITY_VERBOSE);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_RENDER, NOM_LOG_PRIORITY_VERBOSE);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);

  // Disable NOM_LOG_CATEGORY_TEST logging for these tests
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST, NOM_LOG_PRIORITY_CRITICAL);
  // nom::SDL2Logger::set_logging_priority(NOM_LOG_CATEGORY_TEST, NOM_LOG_PRIORITY_DEBUG);

  if( NOM_ANIM_TEST_FLAG(enable_vsync) == true ) {
    nom::set_hint(SDL_HINT_RENDER_VSYNC, "1");
  } else {
    nom::set_hint(SDL_HINT_RENDER_VSYNC, "0");
  }

  // TODO: Change this back to linear?
  // I get a subtle animation flicker when toggled full screen when using
  // nearest scale quality
  if( nom::set_hint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest") == false ) {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not set scale quality to linear;",
                  "animation flicker may result!" );
  }
}

AnimationTest::~AnimationTest()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);

  NOM_LOG_DEBUG( NOM_LOG_CATEGORY_TEST,
                "Number of actions remaining in queue at time of exit:",
                this->player.num_actions() );
}

bool AnimationTest::init_rendering()
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

  return true;
}

void AnimationTest::SetUp()
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
            this->player.remove_actions();
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

  // Do not use the default update callbacks provided by nom::VisualUnitTest
  // this->clear_update_callbacks();

  Timer anim_timer;
  anim_timer.start();
  uint32 last_delta = anim_timer.ticks();
  this->append_update_callback( [&, anim_timer, last_delta](float) mutable {

    uint32 end_delta = anim_timer.ticks();
    uint32 elapsed_delta = end_delta - last_delta;
    last_delta = end_delta;

    // NOM_DUMP(elapsed_delta);
    this->player.update(elapsed_delta);

    // TODO: We need to determine if doing the frame update logic is best done
    // as-is -- at the end of each render callback per test -- or after the
    // action player has updated its loop for the frame (below).
    // ...I'm thinking that the latter is the most proper solution, but I
    // hesitate because it involves removing ::set_frame_interval from each
    // test individually!
    // const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);
    // this->set_frame_interval(FPS);
    // this->render_window().update();
  });
}

void AnimationTest::TearDown()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, nom::NOM_LOG_PRIORITY_VERBOSE);
}

void AnimationTest::SetUpTestCase()
{
  // NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);
}

void AnimationTest::TearDownTestCase()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_UNIT_TEST, NOM_LOG_PRIORITY_VERBOSE);
}

void
AnimationTest::expected_common_params(  const IActionObject* obj,
                                        float duration, float speed,
                                        const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_common_params: " << scope_name << "\n";

  EXPECT_EQ(speed, obj->speed() )
  << "expected_common_params: " << scope_name << "\n";

  EXPECT_EQ(duration*1000, obj->duration() )
  << "expected_common_params scoped_name: " << scope_name << "\n";
}

void AnimationTest::expected_action_params( const GroupAction* action,
                                            nom::size_type size,
                                            const std::string& scope_name )
{
  ASSERT_TRUE(action != nullptr)
  << "expected_action_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(size, action->actions_.size() )
  << "expected_action_params scoped_name: " << scope_name << "\n";
}

void AnimationTest::expected_action_params( const SequenceAction* action,
                                            nom::size_type size,
                                            const std::string& scope_name )
{
  ASSERT_TRUE(action != nullptr)
  << "expected_action_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(size, action->actions_.size() )
  << "expected_action_params scoped_name: " << scope_name << "\n";
}

void
AnimationTest::expected_alpha_in_params(  const FadeInAction* obj,
                                          int16 alpha, const Texture* tex,
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
AnimationTest::expected_alpha_out_params( const FadeOutAction* obj,
                                          int16 alpha, const Texture* tex,
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
AnimationTest::expected_alpha_by_params(  const FadeAlphaByAction* obj,
                                          int16 alpha, const Texture* tex,
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

void AnimationTest::expected_repeat_params( const RepeatForAction* obj,
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

void AnimationTest::expected_repeat_params( const RepeatForeverAction* obj,
                                            uint32 num_repeats,
                                            const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_repeat_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(num_repeats, obj->elapsed_repeats_)
  << "expected_repeat_params scoped_name: " << scope_name << "\n";
}

void
AnimationTest::expected_animation_texture_params( const AnimationTexture* obj,
                                                  nom::size_type num_frames,
                                                  real32 duration, real32 speed,
                                                  const std::string& scope_name )
{
  ASSERT_TRUE(obj != nullptr)
  << "expected_animation_texture_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(num_frames, obj->frames_.size() )
  << "expected_animation_texture_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(duration*1000.0f, obj->duration() )
  << "expected_animation_texture_params scoped_name: " << scope_name << "\n";

  EXPECT_EQ(speed, obj->speed() )
  << "expected_animation_texture_params scoped_name: " << scope_name << "\n";
}

// TODO: Clean up function
NOM_IGNORED_VARS();
void AnimationTest::set_frame_interval(uint32 interval)
{
  const bool SleepIsGranular = true;
  real32 TargetSecondsPerFrame = 1.0f / (real32)interval;

  HighResolutionTimer anim_timer;
  anim_timer.start();

  uint64 last_delta = anim_timer.ticks();

  // Choose the update looping strategy based on whether or not we have a
  // display that is synced to the vertical refresh rate; this determines the
  // effective frame interval we advance by

  // Setup a update callback based on a fixed time step; the display is
  // synced to the vertical refresh rate -- so our target frame interval rate
  // should be 16.66ms (on a 60Hz vertical refresh rate display)

  // Disable frame cap if requested
  if( interval == 0 ) {
    return;
  }

  uint64 end_delta = anim_timer.ticks();
  uint64 elapsed_delta = end_delta - last_delta;
  last_delta = end_delta;

  uint64 WorkCounter = anim_timer.ticks();
  real32 WorkSecondsElapsed =
    HighResolutionTimer::elapsed_ticks(last_delta, WorkCounter);

  real32 SecondsElapsedForFrame = WorkSecondsElapsed;
  if(SecondsElapsedForFrame < TargetSecondsPerFrame) {

    if(SleepIsGranular) {
      uint32 SleepMS = (uint32)(1000.0f * ( TargetSecondsPerFrame -
                                            SecondsElapsedForFrame) );
      if(SleepMS > 0) {
        nom::sleep(SleepMS);
      }
    } else {
      // Not granular; use CPU melter
      // real32 TestSecondsElapsedForFrame =
        // HighResolutionTimer::elapsed_ticks(last_delta, anim_timer.ticks() );
      // NOM_ASSERT(TestSecondsElapsedForFrame < TargetSecondsPerFrame);

      while(SecondsElapsedForFrame < TargetSecondsPerFrame) {
        SecondsElapsedForFrame =
          HighResolutionTimer::elapsed_ticks(last_delta, anim_timer.ticks() );
      }
    } // end if SleepIsGranular
  } else {
    // MISSED FRAME RATE!
  }
}
NOM_IGNORED_VARS_ENDL();

void
AnimationTest::init_animation_texture_test( const std::vector<const char*>& texture_filenames,
                                            texture_frames& anim_frames )
{
  for(  auto itr = texture_filenames.begin();
        itr != texture_filenames.end();
        ++itr )
  {
    auto tex = std::make_shared<Texture>( Texture() );
    ASSERT_TRUE(tex != nullptr);
    ASSERT_TRUE(*itr != nullptr);
    bool ret = tex->load(this->resources[0].path() + *itr );
    if( ret == false ) {
      FAIL()  << "Could not load texture" << *itr << "from file path: "
              << this->resources[0].path() << "\n";
    }

    if( tex->valid() == false ) {
      FAIL()  << "Initialized nom::Texture instance was invalid!\n";
    }

    nom::set_alignment( tex.get(), Point2i::zero, this->WINDOW_DIMS,
                        Anchor::MiddleCenter );

    anim_frames.push_back(tex);
  } // end for tex filenames loop
}

void
AnimationTest::setup_repeating_cursor_test( real32 duration, real32 speed,
                                            real32 fps, uint32 type,
                                            nom::size_type num_repeats,
                                            const std::string& scope_name )
{
  NOM_LOG_DEBUG(NOM_LOG_CATEGORY_ANIMATION, "Marker:", scope_name);

  // Testing parameters
  const nom::size_type NUM_REPEATS = num_repeats;
  texture_frames anim_frames;
  const std::vector<const char*> texture_filenames = {{
    "game-cursor_0.png", "game-cursor_1.png", "game-cursor_2.png",
    "game-cursor_3.png"
  }};

  // maximum test duration before test termination occurs
  const real32 TEST_DURATION = duration;

  // fps per texture
  const real32 FRAME_DURATION = 0.100f;

  // total action duration
  const real32 DURATION = FRAME_DURATION * texture_filenames.size();

  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const real32 SPEED_MOD = speed;
  const uint32 FPS = fps;

  this->init_animation_texture_test(texture_filenames, anim_frames);

  EXPECT_EQ( anim_frames.size(), texture_filenames.size() );

  auto tex_bg =
    nom::create_action<AnimationTexture>(anim_frames, FRAME_DURATION);
  ASSERT_TRUE(tex_bg != nullptr);
  tex_bg->set_speed(SPEED_MOD);

  std::shared_ptr<IActionObject> action0;
  if( type & GROUP ) {
    action0 = nom::create_action<GroupAction>( {tex_bg}, "action0" );
  } else if( type & SEQUENCE ) {
    action0 = nom::create_action<SequenceAction>( {tex_bg}, "action0" );
  } else if( type & REVERSED ) {
    action0 = nom::create_action<ReversedAction>(tex_bg, "action0");
  } else {
    ASSERT_TRUE("ActionType must be either one of: GROUP, SEQUENCE, REVERSED")
    << scope_name;
  }
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);

  std::shared_ptr<IActionObject> repeat;
  if( type & REPEAT_FOR ) {
    repeat =
      nom::create_action<RepeatForAction>(action0, NUM_REPEATS, "action0_RepeatFor");
  } else if( type & REPEAT_FOREVER ) {
    repeat =
      nom::create_action<RepeatForeverAction>(action0, "action0_RepeatForever");
  }
  ASSERT_TRUE(repeat != nullptr);

  EXPECT_EQ(0, this->player.num_actions() )
  << scope_name;
  this->run_action_ret =
  this->player.run_action(repeat, [=]() {

    this->expected_animation_texture_params(  tex_bg.get(), anim_frames.size(),
                                              DURATION, SPEED_MOD,
                                              "texture_params" );
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue repeating action in " << scope_name;
  EXPECT_EQ(1, this->player.num_actions() )
  << scope_name;

  auto kill_timer =
    nom::create_action<WaitForDurationAction>(TEST_DURATION);
  ASSERT_TRUE(kill_timer != nullptr);
  kill_timer->set_name("kill_timer");

  EXPECT_EQ(1, this->player.num_actions() )
  << scope_name;
  this->run_action_ret =
  this->player.run_action( kill_timer, [=]() {

    if( type & REPEAT_FOR ) {
      RepeatForAction* repeat_obj =
        NOM_DYN_PTR_CAST(RepeatForAction*, repeat.get() );

      this->expected_repeat_params( repeat_obj, NUM_REPEATS, "repeat_params" );
    } else if( type & REPEAT_FOREVER ) {
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

  this->append_render_callback( [=](const RenderWindow& win) {

    // Render our animation's texture
    if( tex_bg != nullptr ) {
      tex_bg->render(0);
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
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
    NOM_ANIM_TEST_FLAG(speed),
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
    NOM_ANIM_TEST_FLAG(fps),
    // Option example (part of description)
    "30, 60, 90, 120, ..."
  );

  TCLAP::SwitchArg vsync_arg(
    // Option short form (not supported)
    "",
    // Option long form
    "enable-vsync",
    // Option description
    "Enable updating animation logic during the display's VSYNC period",
    // Option default
    NOM_ANIM_TEST_FLAG(enable_vsync)
  );

  args.push_back(&speed_modifier_arg);
  args.push_back(&timing_mode_arg);
  args.push_back(&fps_arg);
  args.push_back(&vsync_arg);

  // nom::UnitTest framework integration
  nom::init_test(argc, argv, args);

  if( speed_modifier_arg.getValue() != NOM_ANIM_TEST_FLAG(speed) ) {
    NOM_ANIM_TEST_FLAG(speed) = speed_modifier_arg.getValue();
  }

  // nom::Linear is the default timing mode when none is given
  nom::IActionObject::timing_mode_func mode =
    nom::timing_mode_func_from_str( timing_mode_arg.getValue() );

  NOM_ANIM_TEST_FLAG(timing_mode) = mode;
  NOM_ANIM_TEST_FLAG(timing_mode_str) = timing_mode_arg.getValue();

  uint32 fps = fps_arg.getValue();
  if( fps != NOM_ANIM_TEST_FLAG(fps) ) {
    NOM_ANIM_TEST_FLAG(fps) = fps;
  }

  NOM_ANIM_TEST_FLAG(enable_vsync) = vsync_arg.getValue();

  return true;
}

std::shared_ptr<Texture>
create_rect_texture(  RenderWindow& window, const Point2i& pos,
                      const Size2i& dims, const Color4i& color )
{
  std::shared_ptr<Texture> tex =
    std::make_shared<Texture>( Texture() );

  if( tex == nullptr ) {
    // Out of memory???
    return nullptr; // NULL
  }

  // Obtain the optimal pixel format for the platform
  RendererInfo caps = window.caps();

  if( tex->initialize(caps.optimal_texture_format(),
      SDL_TEXTUREACCESS_TARGET, dims) == false )
  {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not initialize the texture source." );
    return nullptr; // Err
  }

  tex->set_position(pos);

  // Set the rendering target to the texture (uses FBO)
  if( tex->set_render_target(window) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not set the rendering target to the texture;",
                  "does your video card support FBO?" );
    return nullptr; // Err
  }

  if( window.fill(color) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not set texture's color" );
    return nullptr; // Err
  }

  if( window.reset_render_target() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not reset the rendering target." );
    return nullptr; // Err
  }

  // Success!
  return tex;
}

IActionObject::timing_mode_func
timing_mode_func_from_str(const std::string& timing_mode)
{
  // Default timing mode
  IActionObject::timing_mode_func mode = nom::Linear::ease_in_out;

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

// FIXME:
ActionTimeComparisonResult
compare_action_timestamps(  const ActionPlayer* player,
                            const std::string& action1,
                            const std::string& action2 )
{
  static uint32 action1_done_time = 0;
  static uint32 action2_done_time = 0;

  bool action1_done = player->action_running(action1);
  bool action2_done = player->action_running(action2);

  if( action1_done == true && action1_done_time == 0 ) {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "action1 is done!");
    action1_done_time = nom::ticks();
  }

  if( action2_done == true && action2_done_time == 0 ) {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "action2 is done!");
    action2_done_time = nom::ticks();
  }

  // process final results
  if( action1_done == true && action2_done == true ) {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "action1 && action2 are done!");


    if( action1_done_time > action2_done_time ) {
      // action 1 took longer to complete than action 2
      return ActionTimeComparisonResult::ACTION2;
    } else if( action2_done_time > action1_done_time ) {
      // action 2 took longer to complete than action 1
      return ActionTimeComparisonResult::ACTION1;
    } else {
      // action 1 took the same time to complete as action 2
      return ActionTimeComparisonResult::EQUAL;
    }
  }

  // Not done comparing
  return ActionTimeComparisonResult::WAITING;
}

/// \brief Test animation timing sanity.
TEST_F(AnimationTest, WaitForDurationAction2s)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = 1.0f;
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
    const uint32 FPS = 0;

  auto idle2s =
    nom::create_action<WaitForDurationAction>( WaitForDurationAction(DURATION) );
  ASSERT_TRUE(idle2s != nullptr);

  auto action0 =
    nom::create_action<SequenceAction>( {idle2s}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
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
  this->append_update_callback( [&](float) {

    // We cannot use nom::ActionPlayer::action_running here because the action
    // is erased from the queue right before this callback gets called

    // TODO: Verify if this is still true!
    if( this->player.num_actions() == 0 ) {
      action_done_time = nom::ticks();
      EXPECT_GT(action_done_time, action_start_time);
      EXPECT_GT(action_done_time, 2000);
      this->quit(); // graceful exit
    }
  });

  // Intentionally disabled frame throttling; I want to ensure that high frame
  // rates do not alter the 2s duration!
  this->append_render_callback( [=](const RenderWindow&) {
    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, RepeatForAction)
{
  // Testing parameters
  const float DURATION = 0.5f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const uint NUM_REPEATS = 4;
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);
  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w,0) );

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Red) );
  ASSERT_TRUE(rect != nullptr);

  // Gradient test
  // Color4iColors grad_colors = { Color4iColors{ Color4i( 114, 66, 66 ), Color4i( 251, 222, 232 ) } };
  // auto rect = std::make_shared<Gradient>();
  // ASSERT_TRUE(rect != nullptr);
  // rect->set_fill_direction(Gradient::FillDirection::Top);
  // rect->set_colors(grad_colors);
  // rect->set_position(RECT_POS);
  // rect->set_size(RECT_SIZE);

  auto translate =
    nom::create_action<MoveByAction>(rect, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto repeat =
    nom::create_action<RepeatForAction>(translate, NUM_REPEATS);
  ASSERT_TRUE(repeat != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {repeat}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    EXPECT_EQ(EXPECTED_TEX_POS, rect->position() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);
    this->expected_repeat_params(repeat.get(), NUM_REPEATS);
    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangle
    if( rect != nullptr ) {
      rect->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks This test does not honor custom speed modifiers passed via command
/// line.
TEST_F(AnimationTest, RepeatForeverAction)
{
  // Testing parameters
  const real32 TEST_DURATION = 2.5f;  // when to stop testing "forever"
  const nom::size_type NUM_REPEATS = 4;
  const real32 DURATION = 0.5f;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;

  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  const Point2i TRANSLATE_POS( Point2i(WINDOW_DIMS.w,0) );
  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);

  auto rect =
    std::make_shared<Rectangle>(IntRect(RECT_POS, RECT_SIZE), Color4i::Red);
  ASSERT_TRUE(rect != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(rect, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto repeat_forever =
    nom::create_action<RepeatForeverAction>(translate);
  ASSERT_TRUE(repeat_forever != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {repeat_forever}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  auto kill_timer =
    nom::create_action<WaitForDurationAction>(TEST_DURATION);
  ASSERT_TRUE(kill_timer != nullptr);

  auto action1 =
    nom::create_action<SequenceAction>( {kill_timer}, "action1" );
  ASSERT_TRUE(action1 != nullptr);
  action1->set_timing_mode(TIMING_MODE);
  action1->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {
    FAIL() << "action0 should never complete!";
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->run_action_ret =
  this->player.run_action(action1, [=]() {

    EXPECT_EQ(2, this->player.num_actions() );
    this->expected_repeat_params(repeat_forever.get(), NUM_REPEATS);
    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(translate.get(), DURATION, SPEED_MOD);
    this->expected_action_params(action1.get(), 1);
    this->expected_common_params(kill_timer.get(), TEST_DURATION, SPEED_MOD);

    this->quit();
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(2, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's rectangle
    if( rect != nullptr ) {
      rect->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, AnimationTexture)
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
                                  Color4i::Green );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  auto tex_bg =
    nom::create_action<AnimationTexture>(tex, DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_action_params(action0.get(), 1);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                      Anchor::MiddleCenter );

  this->append_render_callback( [=](const RenderWindow& win) {
    // Render our animation's texture
    if( tex != nullptr ) {
      tex->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

/// \remarks Thanks goes to Tim Jones of [sdltutorials.com](http://www.sdltutorials.com/sdl-animation)
/// for the sprite frames of Yoshi chosen for this test!
TEST_F(AnimationTest, AnimatedYoshiSpriteWithAnimationTexture)
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

  const real32 SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  this->init_animation_texture_test(texture_filenames, anim_frames);

  EXPECT_EQ( anim_frames.size(), texture_filenames.size() );

  auto tex_bg =
    nom::create_action<AnimationTexture>(anim_frames, FRAME_DURATION);
  ASSERT_TRUE(tex_bg != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {tex_bg}, "action0" );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ(1, this->player.num_actions() );
    this->expected_animation_texture_params(  tex_bg.get(), anim_frames.size(),
                                              DURATION, SPEED_MOD,
                                              "texture_params" );
    this->expected_action_params(action0.get(), 1);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue action0";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_render_callback( [=](const RenderWindow& win) {

    // Render our animation's texture
    if( tex_bg != nullptr ) {
      tex_bg->render(0);
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, RepeatingAnimatedCursorSpriteWithAnimationTextureGroup)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 2.0f;
  const uint32 action_type =
    ActionType::REPEAT_FOR | ActionType::GROUP;
  const nom::size_type NUM_REPEATS = 4;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

TEST_F(AnimationTest, RepeatingAnimatedCursorSpriteWithAnimationTextureSequence)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 2.0f;
  const uint32 action_type =
    ActionType::REPEAT_FOR | ActionType::SEQUENCE;
  const nom::size_type NUM_REPEATS = 4;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

TEST_F(AnimationTest, RepeatingAnimatedCursorSpriteWithAnimationTextureReversed)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 2.0f;
  const uint32 action_type =
    ActionType::REPEAT_FOR | ActionType::REVERSED;
  const nom::size_type NUM_REPEATS = 4;

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

/// \remarks This test does not honor custom speed modifiers passed via command
/// line.
TEST_F(AnimationTest, RepeatingForeverAnimatedCursorSpriteWithAnimationTextureGroup)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 2.0f;
  const uint32 action_type =
    ActionType::REPEAT_FOREVER | ActionType::GROUP;
  const nom::size_type NUM_REPEATS = 4;
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

/// \remarks This test does not honor custom speed modifiers and frame
/// intervals passed via command line.
TEST_F(AnimationTest, RepeatingForeverAnimatedCursorSpriteWithAnimationTextureSequence)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 2.0f;
  const uint32 action_type =
    ActionType::REPEAT_FOREVER | ActionType::SEQUENCE;
  const nom::size_type NUM_REPEATS = 4;
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

/// \remarks This test does not honor custom speed modifiers and frame
/// intervals passed via command line.
TEST_F(AnimationTest, RepeatingForeverAnimatedCursorSpriteWithAnimationTextureReversed)
{
  // Testing parameters

  // maximal test duration before termination
  const real32 TEST_DURATION = 2.0f;
  const uint32 action_type =
    ActionType::REPEAT_FOREVER | ActionType::REVERSED;
  const nom::size_type NUM_REPEATS = 4;
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // IMPORTANT: This value must remain constant for reproducing consistent test
  // results!
  const real32 SPEED_MOD = 1.0f;

  this->setup_repeating_cursor_test(  TEST_DURATION, SPEED_MOD, FPS,
                                      action_type, NUM_REPEATS,
                                      nom::UnitTest::test_name() );
}

TEST_F(AnimationTest, MoveByAction)
{
  // Testing parameters
  const float DURATION = 2.5f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const Point2i TRANSLATE_POS( Point2i(200,0) );
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i RECT_POS(Point2i::zero);
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i EXPECTED_TEX_POS(TRANSLATE_POS);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Green) );
  ASSERT_TRUE(rect != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(rect, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, rect->position() );
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
    if( rect != nullptr ) {
      rect->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, MoveByActionNegativeXDelta)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h);
  const Point2i RECT_POS(WINDOW_DIMS.w, 0);
  const Point2i TRANSLATE_POS( Point2i(-WINDOW_DIMS.w,0) );
  const Point2i EXPECTED_TEX_POS(0,0);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Green) );
  ASSERT_TRUE(rect != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(rect, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, rect->position() );
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
    if( rect != nullptr ) {
      rect->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, MoveByActionWithNegativeYDelta)
{
  // Testing parameters
  const float DURATION = 2.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Size2i RECT_SIZE(WINDOW_DIMS.w/4, WINDOW_DIMS.h/4);
  const Point2i RECT_POS(0, WINDOW_DIMS.h);
  const Point2i TRANSLATE_POS( Point2i(0,-WINDOW_DIMS.h) );
  const Point2i EXPECTED_TEX_POS(0,0);

  auto rect = std::make_shared<Rectangle>(
    Rectangle( IntRect(RECT_POS, RECT_SIZE), Color4i::Green) );
  ASSERT_TRUE(rect != nullptr);

  auto translate =
    nom::create_action<MoveByAction>(rect, TRANSLATE_POS, DURATION);
  ASSERT_TRUE(translate != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {translate}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_POS, rect->position() );
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
    if( rect != nullptr ) {
      rect->draw( this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, ScaleByAction)
{
  // Testing parameters
  const float DURATION = 1.5f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);
  const Size2f SCALE_FACTOR(2.0f, 2.0f);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(128, 128);
  const Size2i EXPECTED_TEX_SIZE( Size2i( TEX_SIZE.w * SCALE_FACTOR.w,
                                          TEX_SIZE.h * SCALE_FACTOR.h )
  );

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  auto scale_by =
    nom::create_action<ScaleByAction>(tex, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by != nullptr);

  auto action0 =
    nom::create_action<GroupAction>( {scale_by}, nom::UnitTest::test_name());
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_SIZE, tex->size() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(scale_by.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
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

TEST_F(AnimationTest, ScaleByActionWithNegativeFactor)
{
  // Testing parameters
  const float DURATION = 1.5f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);
  const Size2f SCALE_FACTOR(-3.0f, -3.0f);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(128, 128);
  const Size2i EXPECTED_TEX_SIZE(
    nom::round_float<int>( (real32)(TEX_SIZE.w) / abs(SCALE_FACTOR.w) ),
    nom::round_float<int>( (real32)(TEX_SIZE.h) / abs(SCALE_FACTOR.h) )
  );

  auto tex = std::make_shared<Texture>( Texture() );
  tex->load(resources[0].path() + "card.png");
  if(tex == nullptr) {
    FAIL() << "Could not load texture from file source.";
  }

  auto scale_by =
    nom::create_action<nom::ScaleByAction>(tex, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by != nullptr);
  scale_by->set_name("scale_by");

  auto action0 =
    nom::create_action<GroupAction>( {scale_by}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_SIZE, tex->size() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(scale_by.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
  EXPECT_EQ(1, this->player.num_actions() );

  this->append_update_callback( [=](float) mutable {
    nom::set_alignment( tex.get(), Point2i::zero, WINDOW_DIMS,
                        Anchor::MiddleCenter );
  });

  this->append_render_callback( [=](const RenderWindow& win) {
    // FIXME: Render our animation's texture
    // tex->draw( this->render_window() );
    if( tex != nullptr ) {
      // This is a temporary workaround
      tex->draw_scaled( scale_by->size_, 0, this->render_window() );
    }

    this->set_frame_interval(FPS);
  });

  EXPECT_EQ( NOM_EXIT_SUCCESS, this->on_run() );
}

TEST_F(AnimationTest, ScaleByActionWithNonPowerOfTwo)
{
  // Testing parameters
  const float DURATION = 1.5f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const Size2f SCALE_FACTOR(2.25f, 1.75f);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  // Initial texture position and size
  const Point2i TEX_POS(Point2i::zero);
  const Size2i TEX_SIZE(129, 129);
  const Size2i EXPECTED_TEX_SIZE =
    Size2i( TEX_SIZE.w * SCALE_FACTOR.w,
            nom::round_float<int>(TEX_SIZE.h * SCALE_FACTOR.h) );

  auto tex = create_rect_texture( this->render_window(), TEX_POS, TEX_SIZE,
                                  Color4i::Blue );
  if( tex == nullptr ) {
    // Out of memory???
    FAIL() << "Could not allocate memory for Texture.";
  }

  auto scale_by =
    nom::create_action<ScaleByAction>(tex, SCALE_FACTOR, DURATION);
  ASSERT_TRUE(scale_by != nullptr);
  scale_by->set_name("scale_by");

  auto action0 =
    nom::create_action<GroupAction>( {scale_by}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
  action0->set_speed(SPEED_MOD);

  EXPECT_EQ(0, this->player.num_actions() );
  this->run_action_ret =
  this->player.run_action(action0, [=]() {

    EXPECT_EQ( EXPECTED_TEX_SIZE, tex->size() );
    EXPECT_EQ(1, this->player.num_actions() );

    this->expected_action_params(action0.get(), 1);
    this->expected_common_params(scale_by.get(), DURATION, SPEED_MOD);

    this->quit(); // graceful exit
  });
  EXPECT_EQ(true, this->run_action_ret)
  << "Failed to queue the action!";
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

TEST_F(AnimationTest, CallbackActionDefaultDuration)
{
  // Testing parameters
  const float DURATION = 0.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  CallbackAction::callback_type callback_func;
  callback_func = [=]() {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_TEST, "Hello, there!" );
  };

  auto anim0 = nom::create_action<CallbackAction>(callback_func);

  auto action0 =
    nom::create_action<GroupAction>( {anim0}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
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
TEST_F(AnimationTest, CallbackActionWithNonZeroDuration)
{
  // Testing parameters
  const float DURATION = 0.05f;
  // const float DURATION = 1.0f;
  const float SPEED_MOD = NOM_ANIM_TEST_FLAG(speed);
  const IActionObject::timing_mode_func TIMING_MODE =
    NOM_ANIM_TEST_FLAG(timing_mode);
  const uint32 FPS = NOM_ANIM_TEST_FLAG(fps);

  CallbackAction::callback_type callback_func;
  callback_func = [=]() {
    std::cout << "Hello, there!\n";
  };

  auto anim0 = nom::create_action<CallbackAction>(callback_func, DURATION);

  auto action0 =
    nom::create_action<GroupAction>( {anim0}, nom::UnitTest::test_name() );
  ASSERT_TRUE(action0 != nullptr);
  action0->set_timing_mode(TIMING_MODE);
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
