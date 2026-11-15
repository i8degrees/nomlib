/******************************************************************************

  nomlib - C++14 cross-platform game engine

  StateMachineTest.cpp

  Unit tests for nom::StateMachine integration with nom::InputStateMapper.

  Copyright (c) 2013-2015 Jeffrey Carpenter <i8degrees@gmail.com>
  All rights reserved.

******************************************************************************/

#include <functional>
#include <memory>
#include <string>

#include <SDL.h>
#include <gtest/gtest.h>

#include <nomlib/config.hpp>
#include <nomlib/core.hpp>
#include <nomlib/system.hpp>

enum TestStateID {
  STATE_INVALID = 0,
  STATE_A = 1,
  STATE_B = 2,
  STATE_C = 3
};

enum class StateAction {
  None,
  SwapToB,
  SwapToA,
  PopState,
  ControllerSwapToB,
  MouseSwapToB,
  JoystickSwapToB
};

class TestState: public nom::IState
{
  public:
    TestState(  nom::uint32 id,
                nom::IState::Type type,
                const std::string& context,
                nom::StateMachine* sm,
                nom::InputStateMapper* im,
                StateAction action,
                nom::int32 key_sym ) :
      nom::IState(id, 0, type),
      context_{context},
      sm_{sm},
      im_{im}
    {
      nom::InputActionMapper action_map;

      std::function<void(const nom::Event&)> cb;
      if( action == StateAction::SwapToB ) {
        cb = [this](const nom::Event&) {
          this->sm_->set_state(
            std::make_unique<TestState>(  STATE_B,
                                          nom::IState::Type::Parent,
                                          "state_b",
                                          this->sm_,
                                          this->im_,
                                          StateAction::SwapToA,
                                          SDLK_3 ),
            nullptr );
        };
      } else if( action == StateAction::SwapToA ) {
        cb = [this](const nom::Event&) {
          this->sm_->set_state(
            std::make_unique<TestState>(  STATE_A,
                                          nom::IState::Type::Parent,
                                          "state_a",
                                          this->sm_,
                                          this->im_,
                                          StateAction::SwapToB,
                                          SDLK_1 ),
            nullptr );
        };
      } else if( action == StateAction::PopState ) {
        cb = [this](const nom::Event&) {
          this->sm_->pop_state(nullptr);
        };
      } else if( action == StateAction::ControllerSwapToB ) {
        cb = [this](const nom::Event&) {
          this->sm_->set_state(
            std::make_unique<TestState>(  STATE_B,
                                          nom::IState::Type::Parent,
                                          "state_b",
                                          this->sm_,
                                          this->im_,
                                          StateAction::SwapToA,
                                          SDLK_3 ),
            nullptr );
        };
      } else if( action == StateAction::MouseSwapToB ) {
        cb = [this](const nom::Event&) {
          this->sm_->set_state(
            std::make_unique<TestState>(  STATE_B,
                                          nom::IState::Type::Parent,
                                          "state_b",
                                          this->sm_,
                                          this->im_,
                                          StateAction::SwapToA,
                                          SDLK_3 ),
            nullptr );
        };
      } else if( action == StateAction::JoystickSwapToB ) {
        cb = [this](const nom::Event&) {
          this->sm_->set_state(
            std::make_unique<TestState>(  STATE_B,
                                          nom::IState::Type::Parent,
                                          "state_b",
                                          this->sm_,
                                          this->im_,
                                          StateAction::SwapToA,
                                          SDLK_3 ),
            nullptr );
        };
      }

      if( cb != nullptr ) {
        const nom::JoystickID dev_id = 1;

        if( action == StateAction::ControllerSwapToB ) {
          action_map.insert(
            "controller_action",
            nom::GameControllerButtonAction(
              dev_id,
              nom::GameController::Button::BUTTON_A,
              nom::InputState::PRESSED ),
            cb );
        } else if( action == StateAction::MouseSwapToB ) {
          action_map.insert(
            "mouse_action",
            nom::MouseButtonAction(
              nom::MouseButton::LEFT_MOUSE_BUTTON,
              nom::InputState::PRESSED ),
            cb );
        } else if( action == StateAction::JoystickSwapToB ) {
          action_map.insert(
            "joystick_action",
            nom::JoystickButtonAction(
              dev_id,
              0,
              nom::InputState::PRESSED ),
            cb );
        } else {
          action_map.insert("action", nom::KeyboardAction(key_sym), cb);
        }
      }

      this->im_->insert(context_, action_map, false);
    }

    std::string input_context() const override
    {
      return this->context_;
    }

    bool on_event(const nom::Event& ev) override
    {
      this->event_handled_ = true;
      return true;
    }

    void on_init(nom::void_ptr) override
    {
      this->initialized_ = true;
    }

    void on_exit(nom::void_ptr) override
    {
      this->exited_ = true;
    }

    void on_pause(nom::void_ptr) override
    {
      this->paused_ = true;
    }

    void on_resume(nom::void_ptr) override
    {
      this->resumed_ = true;
    }

    bool initialized_ = false;
    bool exited_ = false;
    bool paused_ = false;
    bool resumed_ = false;
    bool event_handled_ = false;

  private:
    std::string context_;
    nom::StateMachine* sm_;
    nom::InputStateMapper* im_;
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
      this->input_mapper.set_event_handler(this->evt_handler);
      this->sm.set_input_mapper(&this->input_mapper);
    }

    void TearDown() override
    {
      this->input_mapper.remove_event_handler();
      this->input_mapper.clear();
    }
};

TEST_F(StateMachineTest, InputCallbackParentSwapTransition)
{
  auto a = std::make_unique<TestState>( STATE_A,
                                        nom::IState::Type::Parent,
                                        "state_a",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::SwapToB,
                                        SDLK_1 );
  TestState* a_ptr = a.get();

  this->sm.set_state(std::move(a), nullptr);
  this->sm.update(0.f);

  EXPECT_TRUE(a_ptr->initialized_);
  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_a"));

  this->evt_handler.push_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());

  this->sm.update(0.f);

  EXPECT_EQ(STATE_B, this->sm.current_state_id());
  EXPECT_TRUE(a_ptr->exited_);
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_b"));

  // Probe that the exited state_a context was erased.
  nom::InputActionMapper probe;
  EXPECT_TRUE(this->input_mapper.insert("state_a", probe, false));
  this->input_mapper.erase("state_a");
}

TEST_F(StateMachineTest, InputCallbackMouseButtonSwapTransition)
{
  auto a = std::make_unique<TestState>( STATE_A,
                                        nom::IState::Type::Parent,
                                        "state_a",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::MouseSwapToB,
                                        0 );
  TestState* a_ptr = a.get();

  this->sm.set_state(std::move(a), nullptr);
  this->sm.update(0.f);

  EXPECT_TRUE(a_ptr->initialized_);
  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_a"));

  this->evt_handler.push_event(
    nom::create_mouse_button_click(
      nom::MouseButton::LEFT_MOUSE_BUTTON,
      1,
      0 ) );

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());

  this->sm.update(0.f);

  EXPECT_EQ(STATE_B, this->sm.current_state_id());
  EXPECT_TRUE(a_ptr->exited_);
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_b"));

  // Probe that the exited state_a context was erased.
  nom::InputActionMapper probe;
  EXPECT_TRUE(this->input_mapper.insert("state_a", probe, false));
  this->input_mapper.erase("state_a");
}

TEST_F(StateMachineTest, InputCallbackJoystickButtonSwapTransition)
{
  const nom::JoystickID dev_id = 1;

  auto a = std::make_unique<TestState>( STATE_A,
                                        nom::IState::Type::Parent,
                                        "state_a",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::JoystickSwapToB,
                                        0 );
  TestState* a_ptr = a.get();

  this->sm.set_state(std::move(a), nullptr);
  this->sm.update(0.f);

  EXPECT_TRUE(a_ptr->initialized_);
  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_a"));

  this->evt_handler.push_event(
    nom::create_joystick_button_press(dev_id, 0) );

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());

  this->sm.update(0.f);

  EXPECT_EQ(STATE_B, this->sm.current_state_id());
  EXPECT_TRUE(a_ptr->exited_);
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_b"));

  // Probe that the exited state_a context was erased.
  nom::InputActionMapper probe;
  EXPECT_TRUE(this->input_mapper.insert("state_a", probe, false));
  this->input_mapper.erase("state_a");
}

TEST_F(StateMachineTest, InputCallbackGameControllerButtonSwapTransition)
{
  const nom::JoystickID dev_id = 1;

  auto a = std::make_unique<TestState>( STATE_A,
                                        nom::IState::Type::Parent,
                                        "state_a",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::ControllerSwapToB,
                                        0 );
  TestState* a_ptr = a.get();

  this->sm.set_state(std::move(a), nullptr);
  this->sm.update(0.f);

  EXPECT_TRUE(a_ptr->initialized_);
  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_a"));

  this->evt_handler.push_event(
    nom::create_game_controller_button_press(
      dev_id,
      static_cast<nom::uint8>(nom::GameController::Button::BUTTON_A) ) );

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());

  this->sm.update(0.f);

  EXPECT_EQ(STATE_B, this->sm.current_state_id());
  EXPECT_TRUE(a_ptr->exited_);
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(this->input_mapper.active("state_b"));

  // Probe that the exited state_a context was erased.
  nom::InputActionMapper probe;
  EXPECT_TRUE(this->input_mapper.insert("state_a", probe, false));
  this->input_mapper.erase("state_a");
}

TEST_F(StateMachineTest, ParentSwapRoundTripFreshContexts)
{
  auto a = std::make_unique<TestState>( STATE_A,
                                        nom::IState::Type::Parent,
                                        "state_a",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::SwapToB,
                                        SDLK_1 );
  this->sm.set_state(std::move(a), nullptr);
  this->sm.update(0.f);

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_TRUE(this->input_mapper.active("state_a"));

  this->evt_handler.push_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));
  this->sm.update(0.f);

  EXPECT_EQ(STATE_B, this->sm.current_state_id());
  EXPECT_TRUE(this->input_mapper.active("state_b"));
  EXPECT_EQ(1u, this->sm.count());

  this->evt_handler.push_event(nom::create_key_press(SDLK_3, KMOD_NONE, 0));
  this->sm.update(0.f);

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_TRUE(this->input_mapper.active("state_a"));

  // Probe that the exited state_b context was erased.
  nom::InputActionMapper probe;
  EXPECT_TRUE(this->input_mapper.insert("state_b", probe, false));
  this->input_mapper.erase("state_b");

  this->evt_handler.push_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));
  this->sm.update(0.f);

  EXPECT_EQ(STATE_B, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());
}

TEST_F(StateMachineTest, InputCallbackPopResumesParent)
{
  auto a = std::make_unique<TestState>( STATE_A,
                                        nom::IState::Type::Parent,
                                        "state_a",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::SwapToB,
                                        SDLK_1 );
  TestState* a_ptr = a.get();

  this->sm.set_state(std::move(a), nullptr);
  this->sm.update(0.f);

  EXPECT_EQ(STATE_A, this->sm.current_state_id());

  auto c = std::make_unique<TestState>( STATE_C,
                                        nom::IState::Type::Child,
                                        "state_c",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::PopState,
                                        SDLK_2 );
  TestState* c_ptr = c.get();

  this->sm.set_state(std::move(c), nullptr);
  this->sm.update(0.f);

  EXPECT_EQ(2u, this->sm.count());
  EXPECT_TRUE(a_ptr->paused_);
  EXPECT_FALSE(this->input_mapper.active("state_a"));
  EXPECT_TRUE(this->input_mapper.active("state_c"));

  this->evt_handler.push_event(nom::create_key_press(SDLK_2, KMOD_NONE, 0));

  EXPECT_EQ(STATE_C, this->sm.current_state_id());
  EXPECT_EQ(2u, this->sm.count());

  this->sm.update(0.f);

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
  EXPECT_EQ(1u, this->sm.count());
  EXPECT_TRUE(c_ptr->exited_);
  EXPECT_TRUE(a_ptr->resumed_);
  EXPECT_TRUE(this->input_mapper.active("state_a"));

  // Probe that the exited state_c context was erased.
  nom::InputActionMapper probe;
  EXPECT_TRUE(this->input_mapper.insert("state_c", probe, false));
  this->input_mapper.erase("state_c");

  // Verify the pop-branch no longer re-pops on the next update.
  a_ptr->event_handled_ = false;
  this->evt_handler.push_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));
  this->sm.update(0.f);

  EXPECT_EQ(STATE_B, this->sm.current_state_id());

  this->evt_handler.push_event(nom::create_key_press(SDLK_3, KMOD_NONE, 0));
  this->sm.update(0.f);

  EXPECT_EQ(STATE_A, this->sm.current_state_id());
}

TEST_F(StateMachineTest, StateOnEventForwarding)
{
  auto a = std::make_unique<TestState>( STATE_A,
                                        nom::IState::Type::Parent,
                                        "state_a",
                                        &this->sm,
                                        &this->input_mapper,
                                        StateAction::None,
                                        SDLK_1 );
  TestState* a_ptr = a.get();

  this->sm.set_state(std::move(a), nullptr);
  this->sm.update(0.f);

  this->sm.on_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));
  EXPECT_TRUE(a_ptr->event_handled_);

  // Should not crash on an empty machine; previous test left a state on stack,
  // so we use a fresh instance.
  nom::StateMachine empty_sm;
  empty_sm.on_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));
}

TEST_F(StateMachineTest, MapperDetachIdempotentInstall)
{
  nom::EventHandler h;
  nom::InputStateMapper m;

  m.set_event_handler(h);
  EXPECT_EQ(1u, h.num_event_watchers());

  m.set_event_handler(h);
  EXPECT_EQ(1u, h.num_event_watchers());

  m.remove_event_handler();
  EXPECT_EQ(0u, h.num_event_watchers());

  m.remove_event_handler();
  EXPECT_EQ(0u, h.num_event_watchers());

  h.push_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));

  m.set_event_handler(h);
  EXPECT_EQ(1u, h.num_event_watchers());

  h.push_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));

  m.remove_event_handler();
}

TEST_F(StateMachineTest, RemoveEventWatchPrecision)
{
  nom::EventHandler h;
  bool fired_a = false;
  bool fired_b = false;

  nom::event_filter watch_a =
    [&](const nom::Event&, void*) { fired_a = true; };

  nom::event_filter watch_b =
    [&](const nom::Event&, void*) { fired_b = true; };

  h.append_event_watch(watch_a, nullptr);
  h.append_event_watch(watch_b, nullptr);

  EXPECT_EQ(2u, h.num_event_watchers());

  h.remove_event_watch(watch_b);

  EXPECT_EQ(1u, h.num_event_watchers());

  h.push_event(nom::create_key_press(SDLK_1, KMOD_NONE, 0));

  EXPECT_TRUE(fired_a);
  EXPECT_FALSE(fired_b);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if( nom::init(argc, argv) == false ) {
    return NOM_EXIT_FAILURE;
  }
  atexit(nom::quit);

  return RUN_ALL_TESTS();
}
