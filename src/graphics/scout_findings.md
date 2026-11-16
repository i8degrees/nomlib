# Scout Findings: nomlib Graphics API (RenderWindow / Renderer / Multi-Window)

Scope: `src/graphics/*`, `include/nomlib/graphics/*`, plus the SDL2 event/init
machinery in `src/system` that windows depend on. Focus: window lifecycle,
SDL2 video/event handling, current multi-window support, and what blocks
per-window concurrent event loops on separate displays.

Note: there is **no `RenderQueue` class** in the graphics subsystem (or anywhere
in `src/`). The only similarly-named symbol is
`tests/include/nomlib/tests/actions/ActionTest.hpp:219`
(`ActionTest::append_render_queue`), a test-local helper unrelated to the
windowing stack.

## Key classes and headers

| Class | Header | Impl | Owns (SDL) |
|---|---|---|---|
| `nom::Renderer` | `include/nomlib/graphics/Renderer.hpp` | `src/graphics/Renderer.cpp` | `SDL_Renderer` via `unique_ptr` + `priv::FreeRenderTarget` (Renderer.cpp:38-41) |
| `nom::RenderWindow : public Renderer` | `include/nomlib/graphics/RenderWindow.hpp` | `src/graphics/RenderWindow.cpp` | `SDL_Window` via `unique_ptr` + `priv::FreeWindow` (RenderWindow.cpp:48-50) |
| `nom::EventHandler` | `include/nomlib/system/EventHandler.hpp` | `src/system/EventHandler.cpp` | global SDL event queue (no ownership) |
| `nom::SDLApp` | `include/nomlib/system/SDLApp.hpp` | `src/system/SDLApp.cpp` | none (app framework) |
| `nom::Texture` | `include/nomlib/graphics/Texture.hpp` | `src/graphics/Texture.cpp` | `SDL_Texture` created on the *static active* renderer (Texture.cpp:100,122) |

SDL deleter definitions: `src/system/SDL_helpers.cpp:240-264`
(`FreeWindow` → `SDL_DestroyWindow`, `FreeRenderTarget` →
`SDL_DestroyRenderer`, `FreeTexture` → `SDL_DestroyTexture`).

Global SDL lifetime: `src/system/init.cpp` — `nom::init` →
`init_third_party` → `SDL_Init(SDL_INIT_VIDEO)` (init.cpp:143);
`nom::quit` → `TTF_Quit(); IMG_Quit(); SDL_Quit();` (init.cpp:~190).
Everything is process-global; there is no per-window init/teardown.

## Window lifecycle

Create — `RenderWindow::create(title, pos, display_index, res, flags,
driver, renderer_flags)` (RenderWindow.cpp:82-130):
1. Translates `WINDOW_POS_CENTERED`/`DEFAULT_WINDOW_POS` to
   `SDL_WINDOWPOS_{CENTERED,UNDEFINED}_DISPLAY(display_index)`.
2. `SDL_CreateWindow`, stored in `window_`.
3. `Renderer::create(window, driver, flags)` → `SDL_CreateRenderer`
   (Renderer.cpp:48-57; default flags
   `SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE`,
   `DEFAULT_RENDERING_DRIVER = -1`, Renderer.hpp:~80-86).
4. Records `window_id_` (`SDL_GetWindowID`) and
   `window_display_id_` (`SDL_GetWindowDisplayIndex`).
5. **Calls `make_current()`** (sets the *static* active context, see below),
   `fill(Transparent)`, and `nom::set_render_interface(*this)` (global
   "current window" pointer, `priv::render_dev_`, RenderWindow.cpp:595-602).

The 4-arg overload (RenderWindow.cpp:64-68) delegates with
`DEFAULT_WINDOW_POS` and display `0`.

Update/present — `Renderer::update()` → `SDL_RenderPresent`
(Renderer.cpp:235-238). `RenderWindow::flip()` →
`SDL_UpdateWindowSurface` for the legacy surface path (RenderWindow.cpp:~230).
Drawing calls take the target window explicitly
(e.g. `sprite.draw(window[0], ...)`); `SDLApp::on_draw(RenderWindow&)`
(SDLApp.hpp:~125) is a single-window signature.

Destroy — `~RenderWindow` (RenderWindow.cpp:56-60) does
`priv::FreeRenderTarget( context_ )` — i.e. it destroys the **static global
active context**, not necessarily its own `renderer_` (the base `Renderer`
unique_ptr then destroys its own renderer). There is no public `destroy()`/
`shutdown()` API; teardown is purely via the destructor.

Display awareness — `window_display_id()`, `display_bounds()`,
`display_name()` (static and instance), `num_video_displays()` (static,
`SDL_GetNumVideoDisplays`), `display_modes()`, `refresh_rate()` — all keyed
off the per-window `window_display_id_` (RenderWindow.cpp:~160-230).

## SDL2 video/event handling locations

- Video init: `src/system/init.cpp:143` (`SDL_Init(SDL_INIT_VIDEO)`);
  per-subsystem `SDL_InitSubSystem` in `Joystick.cpp:47`,
  `GameController.cpp:61`, `Timer.cpp:42`.
- Event pump: `src/system/EventHandler.cpp`
  - `process_events()` (line ~301): `SDL_PumpEvents()` + drain-all
    `SDL_PeepEvents(&ev, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)`
    into `std::deque<Event> events_`.
  - `process_event(const SDL_Event*)` (line ~334): maps every SDL event type
    to `nom::Event`; **every event struct carries `window_id`** populated from
    `ev->window.windowID` / `ev->key.windowID` / `ev->motion.windowID` /
    `ev->button.windowID` / `ev->wheel.windowID` / text/user (EventHandler.cpp
    lines 366-905). Per-window routing data is present; routing is left to
    the app.
  - `push_event` (line ~256): enqueues **and synchronously invokes all
    `event_filter` watchers** (`std::function<void(const Event&, void*)>`,
    Event.hpp:549). Real SDL events flow through `push_event` too, so
    watchers fire inside `pop_event`/`process_events`.
  - `pop_event` (line ~268): pumps SDL only when the deque is empty;
    blocking-wait mode is an unimplemented TODO (line ~282). No
    `SDL_WaitEvent` anywhere in `src/`.
  - Watchers: `event_watcher` struct defined at EventHandler.cpp:61;
    append/remove at EventHandler.cpp:~213-248 (removal matches on
    `std::function::target_type()`).
  - Joystick/game-controller state is global per-`EventHandler`
    (single non-owning pointer, one active handler type).
- App framework: `SDLApp::set_event_handler` (SDLApp.cpp:~172) appends one
  watch that funnels everything through `on_app_event` → `process_event`
  (QUIT / window event switch) and `StateMachine::on_event`.
  `SDLApp::Run()` (SDLApp.cpp:~70) is a **stub returning
  `NOM_EXIT_SUCCESS`** — the real loop is written per-app (see
  `examples/nom_app_ex/app.cpp`).
- Only in-tree direct `SDL_PollEvent` use is the
  `examples/GameOfLife/main.cpp:236` example (bypasses nomlib).

## Current multi-window support and limitations

Supported today (proven by `examples/nom_app_ex`):
- N independent `RenderWindow` objects, each with its own `SDL_Window` +
  `SDL_Renderer`, creatable on specific displays
  (`create(..., display_index, ...)`; example places window i on display i).
- Per-window present/update/FPS/title; window events carry `window_id` so
  apps can filter by window.

Structural limitations:
1. **Global "active renderer" context.** `RenderWindow::context_` (static
   `SDL_Renderer*`) + `make_current()`/`set_context()` (RenderWindow.cpp:563-571)
   and `nom::render_interface()` (`priv::render_dev_`, a single global
   `RenderWindow*`). `Texture::create` builds `SDL_CreateTexture` on
   `RenderWindow::context()` (Texture.cpp:100,122); `Text`, `Rectangle`,
   `Gradient`, `BMFont` all consult `nom::render_interface()` /
   `RenderWindow::context()`. So all resource creation is bound to whichever
   window was `make_current()` last; on a GPU driver (OpenGL) those resources
   live in that context and are not guaranteed valid when drawn via a second
   window's renderer. The `make_current` doc (RenderWindow.hpp:~348) itself
   says it "must be called before doing any drawing ... when using multiple
   rendering windows."
2. **Destructor destroys the global context.**
   `~RenderWindow` calls `FreeRenderTarget(context_)`
   (RenderWindow.cpp:56-60) — the *static* active context. Destroying window A
   while window B is active destroys B's renderer (which B's destructor will
   then free again → double-free); destroying A when A is active double-frees
   A's renderer (static + base-class `unique_ptr`). Multi-window teardown is
   landmined in every order.
3. **One event source for all windows.** A single `EventHandler` drains the
   entire (global) SDL event queue; there is no per-window queue, no
   `SDL_WaitEvent`, and no per-window handler object. Two `EventHandler`
   instances would both `SDL_PumpEvents`/`SDL_PeepEvents` the same queue —
   nondeterministic split, lost/duplicated delivery.
4. **App-level quit semantics.** `SDLApp::on_window_close` default calls
   `on_app_quit` → `quit()`, ending the whole app when *any* window closes
   (SDLApp.cpp:~210, ~240).
5. **`SDLApp` assumes one primary window** (`on_draw(RenderWindow&)`,
   single `StateMachine`).

## Example usage (examples/nom_app_ex/app.cpp)

- `MAXIMUM_WINDOWS = 3`; `nom::RenderWindow window[MAXIMUM_WINDOWS]`
  (app.cpp:~75, ~715) — fixed C array, one `nom::EventHandler evt_handler`
  member (single event source for all windows).
- `on_init` (app.cpp:~210-470):
  - `available_render_driver("opengl")`, vsync off, scale quality Nearest.
  - Loop `idx < MAXIMUM_WINDOWS`:
    `window[idx].create(APP_NAME, WINDOW_POS_CENTERED, display_index, ...)`
    with `display_index` incremented while `display_index <
    num_video_displays()` — i.e. window i on display i, wrapping display 0.
  - libRocket (GUI) is initialized **against `window[0]` only**
    (`RocketSDL2RenderInterface(&this->window[0])`, `gl_init`).
  - Texture loading is interleaved with explicit context switches:
    `window[0].make_current()` → load sprite sheet;
    `window[1].make_current()` → load `background` texture; then back to
    `window[0].make_current()` (app.cpp:372, 442, 451).
  - `SDLApp::set_event_handler(this->evt_handler)`.
- `Run` (app.cpp:~470-590): **single-threaded loop**:
  1. Drain: `while(evt_handler.poll_event(evt))` — events are actually
     consumed by the watchers installed via `set_event_handler` (the app's
     `on_input_event`); the poll loop is effectively a no-op placeholder.
  2. Per-window update pass: `window[idx].update()`, `desktop.update()`,
     `actions.update(0)`, per-window `Timer`/`FPS`; window titles updated
     with "WID / Display ID / fps".
  3. Per-window draw pass: `window[i].fill(...)` + sprite/GUI draws (window 0),
     `background.draw(window[1])`, magenta fill for window 2, random fills
     for 3+.
- Per-window input is hand-routed in `on_key_down` (app.cpp:~660-700) via
  `this->window[ev.key.window_id - 1]` for `F1` screenshot and `f`
  fullscreen toggle — this index math **assumes SDL window IDs are dense
  starting at 1 and created in array order** (fragile once windows are
  destroyed/recreated).

## Risks / blockers for per-window concurrent event loops

1. **SDL main-thread rule.** `nom::init`/`SDL_Init(SDL_INIT_VIDEO)` and event
   pumping are done from the calling thread. SDL expects video/event pumping
   from the main thread; a worker-thread event loop per display cannot simply
   own `SDL_PumpEvents` (and two threads pumping one global queue race
   anyway).
2. **Single global SDL event queue.** No per-window queue in SDL2 (only
   `SDL_PushEvent` to post). Supporting N concurrent per-window loops needs a
   redesign: one pump thread that fans out by `window_id` (events already
   carry it) into per-window thread-safe queues, or per-window consumers
   using `SDL_PeepEvents` filtering — none of this exists;
   `EventHandler` has no per-window partitioning, no blocking wait
   (unimplemented TODO at EventHandler.cpp:~282), and its
   joystick/controller state is global.
3. **Static active-renderer context.** `RenderWindow::context_` +
   `priv::render_dev_` are process-global; `Texture::create` and all
   `render_interface()`-based code (Text/Rectangle/Gradient/BMFont) bind to
   one renderer. Concurrent draw/resource-creation from two threads needs
   per-window (non-static) context plumbing.
4. **Renderer thread affinity / GL contexts.** On the OpenGL driver (what
   the example forces), each `SDL_Renderer` binds a GL context made current
   on the creating thread; drawing from another thread (or sharing
   `SDL_Texture`s created in context A under renderer B) is not supported.
   Per-window concurrent rendering would need per-thread GL contexts and
   per-window resource sets.
5. **Destruction race + cross-destruction.** `~RenderWindow` destroys the
   static `context_` (RenderWindow.cpp:56-60) — concurrent teardown of
   multiple windows is a double-free/cross-destroy hazard independent of
   threading; it must be fixed before any multi-window concurrency (and
   `nom::quit` → `SDL_Quit` while other windows render is a second one).
6. **App framework is single-window/single-threaded by design.**
   `SDLApp::on_draw(RenderWindow&)`, one `StateMachine`, `on_window_close`
   → quit-all, `SDLApp::Run` being a stub. Per-window loops imply either
   dropping SDLApp or extending it to per-window sub-apps.
7. **`window[ev.key.window_id - 1]` idiom** in the example (and presumably in
   downstream code such as ttcards) breaks under per-window teardown — any
   window-id → handle map must be an explicit registry (SDL already offers
   `SDL_GetWindowFromID`, wrapped as static `RenderWindow::window_id(uint32)`
   at RenderWindow.hpp:~205 / RenderWindow.cpp:~370).

## Bottom line

Multi-window *creation* on separate displays works today (per-window
`SDL_Window`/`SDL_Renderer`, display-indexed positioning, `window_display_id`
bookkeeping, per-event `window_id`). What does **not** exist: per-window
event queues/handlers, a thread-safe event pump (single global
`EventHandler` draining one SDL queue with no blocking wait), non-static
renderer contexts (resource creation is glued to the last `make_current()`
window), and safe multi-window destruction (`~RenderWindow` frees the global
context). Those five are the blockers for running independent event loops
per display concurrently.
