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
#ifndef NOMLIB_ACTIONS_ACTION_TEST_HPP
#define NOMLIB_ACTIONS_ACTION_TEST_HPP

#include <string>
#include <vector>

// Private headers (third-party)
#include "tclap/CmdLine.h"

// nom::VisualUnitTest framework
#include "nomlib/tests/VisualUnitTest.hpp"

#include <nomlib/config.hpp>
#include <nomlib/math.hpp>
#include <nomlib/system.hpp>
#include <nomlib/serializers.hpp>
#include <nomlib/graphics.hpp>
#include <nomlib/actions.hpp>

/// \brief Set or get a global state control flag.
#define NOM_ACTION_TEST_FLAG(var) nom::anim_test_flags.var

namespace nom {

/// \brief Testing state flags container.
///
/// \remarks See all of the available testing flags by passing --help to this
/// executable.
struct ActionTestFlags
{
  /// \brief The frame interval to update by in action tests.
  ///
  /// \remarks This should **not** affect test results (in particular, test
  /// duration).
  uint32 fps = 30;

  /// \brief Control flag for whether or not to try enable Vertical Blank
  /// Synchronization (VSYNC).
  ///
  /// \remarks This should **not** affect test results (in particular, test
  /// duration, animation smoothness, etc.).
  bool enable_vsync = false;

  /// \brief The speed modifier used in action tests.
  ///
  /// \remarks This affects the test duration.
  real32 speed = 1.0f;

  /// \brief The timing mode used in action tests.
  IActionObject::timing_curve_func timing_curve = nom::Linear::ease_in_out;
  std::string timing_mode_str = "linear_ease_in_out";

  /// \brief The number of objects to spawn in a test.
  ///
  /// \remarks This value is currently only used in
  /// ActionTest.RainingRectsStressTest.
  nom::size_type num_objects = 500;

  /// \brief Enable debug logging of actions.
  bool enable_action_logging = false;

  /// \brief Enable debug logging of the action queues.
  bool enable_queue_logging = false;
};

/// \brief Testing state flags.
///
/// \remarks These are accessible at build-time via NOM_ACTION_TEST_FLAG and
/// run-time via command line switch.
extern ActionTestFlags anim_test_flags;

/// \brief Animation engine unit tests
class ActionTest: public nom::VisualUnitTest
{
  public:
    /// \remarks This method is called at the start of each unit test.
    ActionTest();

    /// \remarks This method is called at the end of each unit test.
    virtual ~ActionTest();

    virtual bool init_rendering();

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp();

    /// \remarks This method is called before destruction, at the end of each
    /// unit test.
    virtual void TearDown();

    /// \remarks This method is called at the start of each test case.
    static void SetUpTestCase();

    /// \remarks This method is called at the end of each test case.
    static void TearDownTestCase();

    /// \brief Helper method for testing properties of nom::IActionObject.
    void
    expected_common_params( const IActionObject* obj, float duration,
                            float speed, const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of nom::GroupAction.
    void expected_action_params(  const GroupAction* action,
                                  nom::size_type size,
                                  const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of nom::SequenceAction.
    void expected_action_params(  const SequenceAction* action,
                                  nom::size_type size,
                                  const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of nom::FadeInAction.
    void expected_alpha_in_params(  const FadeInAction* obj, uint8 alpha,
                                    const Sprite* tex = nullptr,
                                    const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of nom::FadeOutAction.
    void expected_alpha_out_params( const FadeOutAction* obj, uint8 alpha,
                                    const Sprite* tex = nullptr,
                                    const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of nom::FadeAlphaByAction.
    void expected_alpha_by_params(  const FadeAlphaByAction* obj, uint8 alpha,
                                    const Sprite* tex = nullptr,
                                    const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of nom::RepeatForAction.
    void expected_repeat_params(  const RepeatForAction* obj,
                                  uint32 num_repeats,
                                  const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of
    /// nom::RepeatForeverAction.
    void expected_repeat_params(  const RepeatForeverAction* obj,
                                  uint32 num_repeats,
                                  const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of
    /// nom::AnimateTexturesAction.
    void
    expected_animate_textures_params( const AnimateTexturesAction* obj,
                                      nom::size_type num_frames,
                                      real32 duration, real32 speed,
                                      const std::string& scope_name = "" );

    /// \brief Helper method for testing properties of nom::SpriteBatchAction.
    void expected_sprite_batch_action_params( const SpriteBatchAction* obj,
                                              nom::size_type num_frames,
                                              real32 duration, real32 speed,
                                              const
                                              std::string& scope_name = "" );

    bool
    expected_min_duration(  real32 duration, real32 speed,
                            const std::string& scope_name = "" );

    /// \brief Set the frame interval at which actions are updated.
    ///
    /// \param interval A value of zero disables this function.
    ///
    /// \remarks This should always be at the very end of the render
    /// callback(s) when used.
    void set_frame_interval(uint32 interval);

    void
    init_animate_textures_test( const std::vector<const char*>& texture_filenames,
                                texture_frames& anim_frames );

    enum ActionType: uint32
    {
      GROUP = 0x2,
      SEQUENCE = 0x4,
      ACTION = 0x8,
      REPEAT_FOR = 0x100,
      REPEAT_FOREVER = 0x200,
      REVERSED = 0x400,
    };

    /// \params duration The maximum testing duration before termination of
    /// the test occurs. The number of times the action has repeated will be
    /// validated at the time of termination.
    ///
    /// \params num_repeats The number of times to repeat the action.
    void
    setup_repeating_cursor_test(  real32 duration, real32 speed, real32 fps,
                                  uint32 type, nom::size_type num_repeats,
                                  const std::string& scope_name = "" );

    void
    append_render_queue(const Texture* drawable);

    void
    append_render_queue(const Sprite* drawable);

    void
    append_render_queue(const std::vector<std::shared_ptr<Sprite>>& drawables);

    /// \remarks This should only be done once per test.
    void append_frame_interval(uint32 fps);

  protected:
    const Size2i WINDOW_DIMS = Size2i( this->resolution() );

    /// \brief Test-specific and shared font resource paths.
    SearchPath resources[2];

    /// \brief Our instance of the animation player.
    ActionPlayer player;

    /// \brief The stored return value of ActionPlayer::run_action.
    bool run_action_ret = false;

    Timer test_timer;
    uint32 dt_ = 0;
    bool fseek_ = false;
};

bool init_cmd_line_args(int argc, char** argv);

/// \brief Helper method for converting the timing mode string passed in from
/// command-line or NOM_ACTION_TEST_FLAG to a timing mode function.
IActionObject::timing_curve_func
timing_curve_from_str(const std::string& timing_mode);

} // namespace nom

#endif // include guard defined
