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
#ifndef NOMLIB_SYSTEM_STATE_MACHINE_HPP
#define NOMLIB_SYSTEM_STATE_MACHINE_HPP

#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <functional>

#include "export.hpp"
#include "nomlib/config.hpp"
#include "nomlib/system/IState.hpp"

namespace nom {

// Forward declarations
struct Event;
class RenderWindow;
class InputStateMapper;

namespace priv {

/// \internal
/// \see nom::StateMachine::set_deferred_state
/// \endinternal
struct DeferredState
{
  bool deferred;
  std::unique_ptr<IState> state;
  void* data;

  /// Target symbol for a multi-pop transition; 0 means "not set."
  StateSymbol pop_target = NO_STATE;
};

} // namespace priv

/// \brief Action requested by an event-driven transition.
enum class NOM_EXPORT TransitionAction : uint32
{
  /// No transition occurs.
  None = 0,

  /// Replace the active state (Parent semantics).
  Replace,

  /// Push a child state on top of the active state.
  Push,

  /// Pop the active state and resume the previous state.
  Pop,

  /// Pop states until the requested parent becomes active again.
  PopToParent
};

/// \brief Result of an event-driven transition lookup.
struct NOM_EXPORT Transition
{
  /// \brief Action to perform.
  TransitionAction action = TransitionAction::None;

  /// \brief Target state symbol for Push/Replace/PopToParent actions.
  ///
  /// Ignored when action is Pop.
  StateSymbol target = NO_STATE;

  /// \brief User-defined data passed to lifecycle hooks.
  void* data = nullptr;
};

/// \brief Description used to register a state with StateMachine.
///
/// \remarks The factory must return a valid state instance. The lifetime of
/// the registered descriptors must not exceed that of the StateMachine.
struct NOM_EXPORT StateDescriptor
{
  /// Stack symbol for the state.
  StateSymbol symbol;

  /// Stack symbol of the state's declared parent, or NO_STATE for a root.
  StateSymbol parent = NO_STATE;

  /// Factory function that constructs the state instance.
  ///
  /// \param data User-defined data passed from the push/replace request. Use
  /// this for per-instance construction arguments that are not known when
  /// the state is registered.
  std::function<IState::unique_ptr( void* data )> factory;
};

/// \brief A transition table keyed by active state, event, and stack top.
class NOM_EXPORT TransitionTable
{
  public:
    /// Function signature for fallback transition resolution.
    using Callback = std::function<Transition(const IState& active, EventSymbol event, StateSymbol stack_top)>;

    /// \brief Add an exact transition.
    ///
    /// The transition is used only when (active_symbol, event_symbol,
    /// stack_top_symbol) matches exactly.
    void add( StateSymbol active, EventSymbol event, StateSymbol stack_top,
              const Transition& transition );

    /// \brief Add a transition that ignores the stack top.
    ///
    /// Wildcards are checked after exact matches.
    void add( StateSymbol active, EventSymbol event, const Transition& transition );

    /// \brief Query the table for a transition.
    ///
    /// Exact matches take precedence over wildcards. No fallback is invoked.
    Transition query( StateSymbol active, EventSymbol event,
                      StateSymbol stack_top ) const;

    /// \brief Resolve a transition with optional fallback.
    ///
    /// Performs exact and wildcard lookups; if neither matches and a fallback
    /// resolver was installed, it is called with the active state, event, and
    /// stack-top symbol.
    Transition resolve( const IState& active, EventSymbol event,
                        StateSymbol stack_top ) const;

    /// \brief Install a fallback resolver.
    ///
    /// Called when no exact or wildcard transition exists.
    void set_fallback( Callback callback );

  private:
    /// Exact-match transitions.
    std::map<std::tuple<StateSymbol, EventSymbol, StateSymbol>, Transition> transitions_;

    /// Wildcard transitions (stack top ignored).
    std::map<std::pair<StateSymbol, EventSymbol>, Transition> wildcards_;

    /// Optional fallback resolver.
    Callback fallback_;
};


/// \brief Finite State Machine manager class
class NOM_EXPORT StateMachine
{
  public:
    /// Default constructor
    StateMachine();

    /// Destructor
    ~StateMachine();

    // virtual void set_state ( uint32 id, void_ptr data = nullptr );

    /// \brief Get the number of active states.
    ///
    /// \returns Total number of states in the stack.
    uint32 count() const;

    /// \brief Check if the state stack is empty.
    bool empty() const;

    /// \brief Obtain the current state's identifier
    ///
    /// \returns The identifier of the active (top-most) state, or 0 if the
    /// stack is empty.
    ///
    /// \remarks The state identifier is user-defined. A state with id 0 is
    /// indistinguishable from the empty-stack sentinel.
    uint32 current_state_id( void ) const;

    /// \brief Obtain the next pending state's identifier.
    ///
    /// \returns The identifier of the state queued by set_state/pop_state if a
    /// transition is pending; otherwise the identifier of the current active
    /// state, or 0 if the stack is empty.
    ///
    /// \remarks This reflects the deferred transition target, not the state
    /// immediately below the active one on the stack.
    uint32 next_state_id( void ) const;

    /// \brief Obtain the current active state symbol.
    ///
    /// \returns The symbol of the active (top-most) state, or NO_STATE if
    /// the stack is empty.
    StateSymbol current_state_symbol() const;

    /// \brief Register a state factory and optional parent relationship.
    ///
    /// Registered states can be pushed by symbol via push_state and can act
    /// as targets for event-driven transitions.
    ///
    /// \param desc State descriptor containing the symbol, optional parent,
    /// and factory.
    void register_state( const StateDescriptor& desc );

    /// \brief Add an exact event-driven transition to the machine's table.
    void add_transition( StateSymbol active, EventSymbol event,
                         StateSymbol stack_top, const Transition& transition );

    /// \brief Add a wildcard event-driven transition to the machine's table.
    ///
    /// The stack top symbol is ignored when matching this transition.
    void add_transition( StateSymbol active, EventSymbol event,
                         const Transition& transition );

    /// \brief Install a fallback transition resolver.
    ///
    /// Called when no exact or wildcard transition exists.
    void set_transition_fallback( const TransitionTable::Callback& callback );

    /// \brief Install the input mapper used to auto-manage per-state input
    /// contexts during state transitions.
    ///
    /// \param mapper Non-owned pointer; it must outlive this machine or be
    /// reset via set_input_mapper(nullptr). May be nullptr (default).
    ///
    /// \remarks For every state whose nom::IState::input_context() key is not
    /// empty: the context is activated after on_init and on_resume, disabled
    /// after on_pause, and erased after on_exit. States with an empty context
    /// key manage their contexts manually.
    void set_input_mapper( InputStateMapper* mapper );

    /// \brief Obtain the installed input mapper, or nullptr.
    InputStateMapper* input_mapper() const;

    /// \param state  The nom::IState deriving object to be set as the current
    /// state.
    /// \param data   User-defined data; used for passing data in between
    /// states.
    ///
    /// \see StateMachine::set_deferred_state
    void set_state( std::unique_ptr<IState> state, void_ptr data );

    /// \param state  The nom::IState deriving object to be set as the current
    /// state.
    /// \param data   User-defined data; used for passing data in between
    /// states.
    ///
    /// \see StateMachine::set_deferred_state
    void pop_state( void_ptr data );

    /// \brief Push a registered state onto the stack by symbol.
    ///
    /// \param symbol Symbol registered with register_state().
    /// \param data   User-defined data passed to on_init.
    ///
    /// \remarks Equivalent to set_state for states registered as Child of the
    /// current active state; if no parent relationship is declared, the
    /// symbol is simply pushed as a Child overlay.
    void push_state( StateSymbol symbol, void_ptr data = nullptr );

    /// \brief Push a registered child state onto the active parent.
    ///
    /// The child must be registered with a parent matching the active state's
    /// symbol, or the request is rejected.
    ///
    /// \param child  Symbol of the child state to push.
    /// \param data   User-defined data passed to on_init.
    void push_child( StateSymbol child, void_ptr data = nullptr );

    /// \brief Pop states until the requested parent state is active.
    ///
    /// All intermediate states are exited. If the target parent is already
    /// active, this is a no-op.
    ///
    /// \param parent Symbol of the parent state to make active.
    /// \param data   User-defined data passed to lifecycle hooks.
    void pop_to_parent( StateSymbol parent, void_ptr data = nullptr );

    /// \brief Replace the active state with a registered state by symbol.
    ///
    /// \param symbol Symbol registered with register_state().
    /// \param data   User-defined data passed to lifecycle hooks.
    void replace_state( StateSymbol symbol, void_ptr data = nullptr );

    /// \brief Event handling logic for the active state.
    ///
    /// Forwards the event to the active state's on_event. No transition-table
    /// lookup is performed; use on_event(EventSymbol, const Event&) to drive
    /// event-driven transitions.
    ///
    /// \see nom::IState::on_event.
    void on_event(const Event& ev);

    /// \brief Event handling logic with transition-table evaluation.
    ///
    /// Queries the transition table using the active state symbol, the
    /// provided event symbol, and the current stack-top symbol. If a
    /// transition matches, it is applied and the event is consumed. If no
    /// transition matches and ev is not nullptr, the event is forwarded to
    /// the active state's on_event.
    void on_event( EventSymbol event_symbol, const Event* ev );

    /// \brief State logic handling
    ///
    /// \remarks Deferred state transition logic is handled here.
    void update( float delta );

    /// State rendering handling
    void draw( RenderWindow& );

  private:
    /// \brief Set the next state.
    ///
    /// \param state  The state to be put onto the stack. If NULL is passed,
    /// the current state exits and the next one after that is resumed.
    ///
    /// \note Where on the stack the state is put is dependent upon the type of
    /// state defined (see also: nom::IState::Type enumeration).
    ///
    /// \remarks The transition of the state must be deferred until the next
    /// frame update in order for engine callback systems, i.e.:
    /// nom::InputMapper and nom::UIWidget, to be used within application-level
    /// code that expects to be able to change its state from within said
    /// callback system. If not deferred, the result is invalid memory access
    /// (a crash if you are lucky!) due to the deletion of the state before the
    /// callback is completed.
    void set_deferred_state( std::unique_ptr<IState> state, void_ptr data );

    void activate_input_context( const IState& state );
    void disable_input_context( const IState& state );
    void erase_input_context( const IState& state );

    /// \brief Construct a registered state by symbol.
    ///
    /// \param symbol Registered symbol to construct.
    /// \param data   User-defined data passed to the registered factory.
    ///
    /// \returns A valid state instance, or nullptr if the symbol is not
    /// registered.
    IState::unique_ptr make_state( StateSymbol symbol, void_ptr data = nullptr ) const;

    /// \brief Validate that child is declared as a child of the given parent.
    bool can_push_child( StateSymbol child, StateSymbol parent ) const;

    /// \brief The pending state to be set on the next frame update
    priv::DeferredState next_state_;

    /// \brief Stack of states
    std::vector<std::unique_ptr<IState>> states_;

    /// \brief Registered state factories keyed by symbol.
    std::map<StateSymbol, StateDescriptor> registry_;

    /// \brief Event-driven transition table.
    TransitionTable transitions_;

    InputStateMapper* input_mapper_ = nullptr;
};

} // namespace nom

#endif // include guard defined
