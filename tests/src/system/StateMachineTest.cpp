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
#include <SDL.h>

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include <nomlib/config.hpp>
#include <nomlib/core.hpp>
#include <nomlib/system.hpp>

using namespace nom;

namespace {

enum TestStateID { STATE_A = 1, STATE_B = 2, STATE_C = 3 };
enum class StateAction { None, SwapToB, SwapToA, PopState };

class TestState: public IState
{
  public:
    TestState( uint32 id, IState::Type type, const std::string& context,
               StateMachine* sm, InputStateMapper* im,
               StateAction action, int32 key_sym ):
      IState( id, 0, type ),
      context_{ context },
      sm_{ sm },
      im_{ im }
    {
      InputActionMapper action_map;

      if( action != StateAction::None ) {
        event_callback cb;
        switch( action ) {
          default: break;

          case StateAction::SwapToB:
          {
            cb = nom::event_callback( [&](const nom::Event& ev) {
              this->sm_->set_state(
                std::make_unique<TestState>(
                  STATE_B, IState::Type::Parent, "state_b",
                  this->sm_, this->im_, StateAction::SwapToA, SDLK_3 ),
                nullptr );
            });
            break;
          }

          case StateAction::SwapToA:
          {
            cb = nom::event_callback( [&](const nom::Event& ev) {
              this->sm_->set_state(
                std::make_unique<TestState>(
                  STATE_A, IState::Type::Parent, "state_a",
                  this->sm_, this->im_, StateAction::SwapToB, SDLK_1 ),
                nullptr );
            });
            break;
          }

          case StateAction::PopState:
          {
            cb = nom::event_callback( [&](const nom::Event& ev) {
              this->sm_->pop_state( nullptr );
            });
            break;
          }
        }

        action_map.insert( "swap", nom::KeyboardAction( key_sym ), cb );
      }

      im_->insert( context_, action_map, false );
    }

    std::string input_context() const override { return this->context_; }

    bool on_event( const nom::Event& ev ) override {
      this->event_handled_ = true;
      return true;
    }

    void on_init( void_ptr ) override { this->initialized_ = true; }
    void on_exit( void_ptr ) override { this->exited_ = true; }
    void on_pause( void_ptr ) override { this->paused_ = true; }
    void on_resume( void_ptr ) override { this->resumed_ = true; }

    bool initialized_ = false;
    bool exited_ = false;
    bool paused_ = false;
    bool resumed_ = false;
    bool event_handled_ = false;

  private:
    std::string context_;
    StateMachine* sm_ = nullptr;
    InputStateMapper* im_ = nullptr;
};

// Shared trackers for the multi-state lifecycle test.
std::vector<uint32> g_init_order;
std::vector<uint32> g_resume_order;
std::vector<uint32> g_exit_order;

class CountingChildState: public IState
{
  public:
    CountingChildState( uint32 id, StateMachine* sm ):
      IState( id, 0, IState::Type::Child ),
      sm_{ sm }
    {
    }

    std::string input_context() const override { return ""; }

    void on_init( void_ptr ) override {
      g_init_order.push_back( this->id() );
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "CountingChildState", this->id(), "init: counter =", this->counter_ );
    }

    void on_resume( void_ptr ) override {
      // Each state prints starting at 1 when it becomes active.
      this->counter_ = 1;
      g_resume_order.push_back( this->id() );
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "CountingChildState", this->id(), "resume: counter =", this->counter_ );
    }

    void on_update( float delta ) override {
      this->elapsed_ += delta;
      if( this->elapsed_ >= 1.0f ) {
        this->elapsed_ -= 1.0f;
        NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "CountingChildState", this->id(), "tick:", this->counter_ );
        ++this->counter_;
      }
    }

    void on_pause( void_ptr ) override {
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "CountingChildState", this->id(), "pause" );
    }

    void on_exit( void_ptr ) override {
      g_exit_order.push_back( this->id() );
      NOM_LOG_DEBUG( NOM_LOG_CATEGORY_STATES, "CountingChildState", this->id(), "exit" );
    }

  private:
    StateMachine* sm_ = nullptr;
    int counter_ = 1;
    float elapsed_ = 0.0f;
};

class StateMachineTest: public ::testing::Test
{
  protected:
    // Declaration order matters: evt_handler is destroyed after input_mapper.
    nom::EventHandler evt_handler;
    nom::InputStateMapper input_mapper;
    nom::StateMachine sm;

    void SetUp() override
    {
      this->input_mapper.set_event_handler( this->evt_handler );
      this->sm.set_input_mapper( &this->input_mapper );
    }

    void TearDown() override
    {
      this->input_mapper.remove_event_handler();
      this->input_mapper.clear();
    }
};

// A context key can be inserted only when it is not already present, so probing
// for the absence of a key is done by attempting to insert a blank map and
// then erasing the probe.
void expect_context_absent( InputStateMapper& mapper, const std::string& key )
{
  InputActionMapper probe;
  EXPECT_TRUE( mapper.insert( key, probe, false ) );
  EXPECT_TRUE( mapper.erase( key ) );
}

TEST_F( StateMachineTest, InputCallbackParentSwapTransition )
{
  auto a = std::make_unique<TestState>(
    STATE_A, IState::Type::Parent, "state_a",
    &this->sm, &this->input_mapper, StateAction::SwapToB, SDLK_1 );
  TestState* a_ptr = a.get();

  this->sm.set_state( std::move( a ), nullptr );
  this->sm.update( 0.0f );

  EXPECT_TRUE( a_ptr->initialized_ );
  EXPECT_EQ( STATE_A, this->sm.current_state_id() );
  EXPECT_EQ( 1u, this->sm.count() );
  EXPECT_TRUE( this->input_mapper.active( "state_a" ) );

  // Push an event that triggers the input-mapper callback. The transition is
  // deferred and must not happen until update() runs.
  this->evt_handler.push_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  EXPECT_EQ( STATE_A, this->sm.current_state_id() );
  EXPECT_EQ( 1u, this->sm.count() );

  this->sm.update( 0.0f );

  EXPECT_EQ( STATE_B, this->sm.current_state_id() );
  EXPECT_EQ( 1u, this->sm.count() );
  EXPECT_TRUE( a_ptr->exited_ );
  EXPECT_TRUE( this->input_mapper.active( "state_b" ) );

  // "state_a" must have been erased by the machine. Probing for it proves that.
  expect_context_absent( this->input_mapper, "state_a" );
}

TEST_F( StateMachineTest, ParentSwapRoundTripFreshContexts )
{
  auto a = std::make_unique<TestState>(
    STATE_A, IState::Type::Parent, "state_a",
    &this->sm, &this->input_mapper, StateAction::SwapToB, SDLK_1 );

  this->sm.set_state( std::move( a ), nullptr );
  this->sm.update( 0.0f );
  EXPECT_EQ( STATE_A, this->sm.current_state_id() );

  this->evt_handler.push_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  this->sm.update( 0.0f );
  EXPECT_EQ( STATE_B, this->sm.current_state_id() );

  this->evt_handler.push_event( nom::create_key_press( SDLK_3, KMOD_NONE, 0 ) );
  this->sm.update( 0.0f );
  EXPECT_EQ( STATE_A, this->sm.current_state_id() );
  EXPECT_TRUE( this->input_mapper.active( "state_a" ) );

  // The new A's constructor could only insert "state_a" if the old A erased it.
  expect_context_absent( this->input_mapper, "state_b" );

  // Exercise another swap to ensure the resetting machinery stays consistent.
  this->evt_handler.push_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  this->sm.update( 0.0f );
  EXPECT_EQ( STATE_B, this->sm.current_state_id() );
  EXPECT_EQ( 1u, this->sm.count() );
}

TEST_F( StateMachineTest, InputCallbackPopResumesParent )
{
  auto a = std::make_unique<TestState>(
    STATE_A, IState::Type::Parent, "state_a",
    &this->sm, &this->input_mapper, StateAction::SwapToB, SDLK_1 );
  TestState* a_ptr = a.get();

  this->sm.set_state( std::move( a ), nullptr );
  this->sm.update( 0.0f );

  auto c = std::make_unique<TestState>(
    STATE_C, IState::Type::Child, "state_c",
    &this->sm, &this->input_mapper, StateAction::PopState, SDLK_2 );
  TestState* c_ptr = c.get();

  this->sm.set_state( std::move( c ), nullptr );
  this->sm.update( 0.0f );

  EXPECT_EQ( 2u, this->sm.count() );
  EXPECT_TRUE( a_ptr->paused_ );
  EXPECT_FALSE( this->input_mapper.active( "state_a" ) );
  EXPECT_TRUE( this->input_mapper.active( "state_c" ) );

  this->evt_handler.push_event( nom::create_key_press( SDLK_2, KMOD_NONE, 0 ) );
  EXPECT_EQ( 2u, this->sm.count() );

  this->sm.update( 0.0f );

  EXPECT_EQ( 1u, this->sm.count() );
  EXPECT_EQ( STATE_A, this->sm.current_state_id() );
  EXPECT_TRUE( c_ptr->exited_ );
  EXPECT_TRUE( a_ptr->resumed_ );
  EXPECT_TRUE( this->input_mapper.active( "state_a" ) );

  expect_context_absent( this->input_mapper, "state_c" );

  // Verify the pop branch resets its deferred flag: a second pop cycle must
  // transition exactly once, not double-pop.
  this->evt_handler.push_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  this->sm.update( 0.0f );
  EXPECT_EQ( STATE_B, this->sm.current_state_id() );

  this->evt_handler.push_event( nom::create_key_press( SDLK_3, KMOD_NONE, 0 ) );
  this->sm.update( 0.0f );
  EXPECT_EQ( STATE_A, this->sm.current_state_id() );
}

TEST_F( StateMachineTest, StateOnEventForwarding )
{
  // on_event on an empty machine must not crash.
  this->sm.on_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );

  auto a = std::make_unique<TestState>(
    STATE_A, IState::Type::Parent, "state_a",
    &this->sm, &this->input_mapper, StateAction::None, SDLK_1 );
  TestState* a_ptr = a.get();

  this->sm.set_state( std::move( a ), nullptr );
  this->sm.update( 0.0f );

  this->sm.on_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  EXPECT_TRUE( a_ptr->event_handled_ );
}

TEST_F( StateMachineTest, MapperDetachIdempotentInstall )
{
  nom::EventHandler h;
  nom::InputStateMapper m;

  m.set_event_handler( h );
  EXPECT_EQ( 1u, h.num_event_watchers() );

  m.set_event_handler( h );
  EXPECT_EQ( 1u, h.num_event_watchers() );

  m.remove_event_handler();
  EXPECT_EQ( 0u, h.num_event_watchers() );

  m.remove_event_handler();
  EXPECT_EQ( 0u, h.num_event_watchers() );

  h.push_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  EXPECT_EQ( 0u, h.num_event_watchers() );

  m.set_event_handler( h );
  EXPECT_EQ( 1u, h.num_event_watchers() );

  h.push_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  EXPECT_EQ( 1u, h.num_event_watchers() );
}

TEST_F( StateMachineTest, SixConcurrentStates )
{
  g_init_order.clear();
  g_resume_order.clear();
  g_exit_order.clear();

  // Bump log priority so the lifecycle is visible when the test runs.
  nom::SDL2Logger::set_logging_priority( NOM_LOG_CATEGORY_STATES,
                                          NOM_LOG_PRIORITY_DEBUG );

  // Build a stack of six child states: one through six.
  for( uint32 id = 1; id <= 6; ++id ) {
    this->sm.set_state(
      std::make_unique<CountingChildState>( id, &this->sm ),
      nullptr );
    this->sm.update( 0.0f );
  }

  EXPECT_EQ( 6u, this->sm.count() );
  EXPECT_EQ( 6u, g_init_order.size() );
  EXPECT_EQ( 0u, g_resume_order.size() ); // nothing to resume until we pop

  // Each second, tick the active top state (it logs its current counter,
  // starting at 1), then pop it so the state below resumes and starts at 1.
  for( uint32 step = 1; step <= 6; ++step ) {
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    this->sm.update( 1.0f );

    if( step < 6 ) {
      this->sm.pop_state( nullptr );
      this->sm.update( 0.0f );
    }
  }

  // The API refuses to pop the last state, so the bottom state remains.
  EXPECT_EQ( 1u, this->sm.count() );
  EXPECT_EQ( 5u, g_exit_order.size() );
  EXPECT_EQ( 5u, g_resume_order.size() );

  // Exit order should be top-down: 6, 5, 4, 3, 2.
  uint32 expected_id = 6;
  for( auto id : g_exit_order ) {
    EXPECT_EQ( expected_id, id );
    --expected_id;
  }
}

TEST_F( StateMachineTest, RemoveEventWatchPrecision )
{
  nom::EventHandler h;

  bool fired_a = false;
  bool fired_b = false;

  nom::event_filter watch_a = [&](const nom::Event&, void*) {
    fired_a = true;
  };

  nom::event_filter watch_b = [&](const nom::Event&, void*) {
    fired_b = true;
  };

  h.append_event_watch( watch_a, nullptr );
  h.append_event_watch( watch_b, nullptr );
  EXPECT_EQ( 2u, h.num_event_watchers() );

  h.remove_event_watch( watch_b );
  EXPECT_EQ( 1u, h.num_event_watchers() );

  h.push_event( nom::create_key_press( SDLK_1, KMOD_NONE, 0 ) );
  EXPECT_TRUE( fired_a );
  EXPECT_FALSE( fired_b );
}

} // namespace

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  if( nom::init( argc, argv ) == false )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib." );
    return NOM_EXIT_FAILURE;
  }
  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
