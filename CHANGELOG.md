# Change Log #

## Unreleased ##

* Added `nom::filesystem::path::lexically_normal()` plus free
  `nom::filesystem::absolute()`, `canonical()`, and `weakly_canonical()`.
  These mirror `std::filesystem`'s path resolution operations:
  `lexically_normal()` collapses `.` and `..` syntactically,
  `canonical()` resolves symlinks and normalizes existing paths, and
  `weakly_canonical()` canonicalizes the existing prefix while preserving
  missing trailing components. POSIX implementations use `realpath()`;
  Windows implementations use `GetFinalPathNameByHandleW()`.

* Fixed `nom::EventHandler::remove_event_watch` matching by using
  `std::function::target_type()` instead of the broken `target<event_filter>()`
  comparison. Distinct lambda-backed watchers are now removed precisely.

* Added `nom::InputStateMapper::remove_event_handler()` and made
  `set_event_handler()` idempotent. Installing the same handler twice is now
  a no-op, and calling `set_event_handler()` with a different handler detaches
  the previous watch first.

* Added `nom::IState::input_context()` hook so states can declare a
  `nom::InputStateMapper` context key.

* Extended `nom::StateMachine` with `set_input_mapper()` / `input_mapper()`
  and automatic per-state input context lifecycle management: contexts are
  activated after `on_init` and `on_resume`, disabled after `on_pause`, and
  erased after `on_exit`. Also fixed the pop/resume branch to clear
  `next_state_.deferred` / `next_state_.data`, preventing double-pops.

* Added `tests/src/system/StateMachineTest.cpp` with `StateMachineTest`
  covering keyboard, mouse button, joystick button, and game controller button
  input callbacks triggering deferred state transitions, plus
  `EventHandler`/`InputStateMapper` lifecycle tests.

* Rewrote `examples/nom_gamecontroller_ev/gamecontroller_events.cpp` to use
  `nom::StateMachine` and per-state input contexts for keyboard, game controller
  button, and game controller axis mappings. Controller connection/disconnection
  now transitions between `WaitingState` and `PlayingState`, with contexts
  managed automatically by the state machine.

* Added `examples/nom_paint`, a pixel paint program using `nom::Image`, `nom::Texture`, and `nom::EventHandler`. Features include pencil, eraser, outline and filled rectangles and circles, flood fill with optional vertical gradient, and text stamping; a left-hand toolbar with shape icons and hover tooltips; a 16×16 color palette grid built from the 16 basic HTML colors with white at the first swatch and black at the last; per-tool size control from 1 to 40 pixels; undo/redo history; and PNG save/load.

* Reworked nomlib-audio streaming to use `nom::StreamSound` backed by
  `libsndfile` for all streamed audio (OGG, WAV, FLAC, etc.). Fixed buffer
  size/duration accounting, removed the hardcoded compressed-bitrate divisor,
  and aligned chunk size with the previous `OggSound` implementation,
  eliminating the periodic static/click. Removed the `LOAD_WAV` compile
  definition; libsndfile is now linked unconditionally while `USE_LIBOGG`
  and the `OggSound` path remain in place.

* Fixed streaming audio temporary-emitter cleanup so emitters are deleted once
  playback finishes, using `time_left_` instead of the `state_` workaround.
  Streaming sources now track remaining playback time via per-buffer
  durations consumed by OpenAL.

* Made `ISoundSource::set_volume` virtual and added an `ALSoundSource`
  override that applies the OpenAL `AL_GAIN` immediately, removing the latency
  previously observed when changing a streaming source's volume.

* Fetched `SDL_GameControllerDB` unconditionally in `vendor/CMakeLists.txt`
  so the community game controller mapping database is always available to
  examples. The `master` download is now pinned to commit
  `9d63598d49a62b19580fc63aed6cdf906be8e1d6` with an updated SHA512 hash.

* Refactored `examples/nom_gamecontroller_ev` resource handling to resolve
  `Resources/SharedSupport` at runtime relative to the executable, matching
  `examples/GameOfLife`. Development builds now copy `gamecontrollerdb.txt`
  into `<build>/bin/Resources/SharedSupport`, and the executable directory is
  normalized with `realpath()` to survive unusual relative `argv[0]` values and
  launcher paths.

* Added `nom::filesystem::directory_entry` and `nom::filesystem::directory_iterator`
  for POSIX directory iteration (Windows via `FindFirstFileW`). Includes range-based
  `for` support and skips `.` and `..` entries, matching the Boost.Filesystem
  `tut3.cpp` behavior.

* Extended `nom::filesystem::path` with element iterators (`begin`, `end`) and
  `make_preferred()`, allowing paths to be decomposed element-by-element and to
  normalize separators to the platform's preferred form.

* Added `tests/src/system/FileInterfaceTest.cpp` cases for status queries
  (`exists`, `is_regular_file`, `is_directory`, `file_size`), directory
  iteration, directory contents sorted by filename (mirroring Boost.Filesystem
  `tut4.cpp`), empty directories, missing directories, file sizes, and Unicode
  path constructors (mirroring Boost.Filesystem `tut2.cpp`, `tut3.cpp`,
  `tut4.cpp`, and `tut5.cpp`).

* Hardened `nom::filesystem` based on code review: fixed the Windows build
  (`to_wide_path` helper ordering and `<cwchar>` include), fixed doubled-separator
  output when appending to a path with a trailing separator, made `remove()`
  use `lstat` on POSIX so symlinks to directories are unlinked, switched
  Windows `file_size()` to `_wstat64`, corrected `is_absolute()` after a
  root-name-preserving append, and made `stem()`/`extension()` treat `.` and
  `..` as extension-less. Also improved test isolation in
  `FileInterfaceTest.cpp` and removed the `/tmp`-deletion risk in the
  recursive-directory test.

* Fixed `nom::filesystem::directory_iterator` and `nom::filesystem::path::iterator`
  equality so that two non-end iterators over different sequences compare unequal.

* Added lexical normalization of `.` and `..` to `nom::filesystem::path`:
  trailing `..` walks up relative paths, absolute paths cannot escape the root,
  and `.` segments are discarded. This removes a directory-traversal vector
  identified in the security review.

* Refactored `examples/nom_app_ex` resource handling to match `GameOfLife` and
  `nom_gamecontroller_ev`: executable directory is normalized with `realpath()`
  on POSIX, CMake copies `Resources/examples/app` and `Resources/tests/gui`
  next to the built binary, and assets are listed as target sources. Also fixed
  the libRocket file-interface root to be the `Resources/tests/gui` directory
  (with trailing separator), migrated the second `info_box` document load to
  `MESSAGEBOX_RML`, resolved the sprite sheet image path relative to the JSON
  file parent directory, corrected install destination directories to match the
  runtime search layout, and fixed a double-semicolon log statement.

* Refactored `examples/nom_font/renderfont.cpp` to use `nom::filesystem`
  instead of `nom::File`/`nom::Path`. The font path is now resolved to an
  absolute path before engine initialization, so `nom_font` accepts both
  relative and absolute font file paths. The BMFont file root is set to the
  absolute parent directory of the resolved font file.

* Added `nom::filesystem::filesystem_error` exception type mirroring the
  Boost.Filesystem interface: stores the error code and up to two
  involved paths, and `what()` includes the original message, path(s), and
  system error text. `nom::filesystem::current_path()` operations now
  report failures by throwing `filesystem_error` instead of failing silently.

* Fixed a segmentation fault in `examples/nom_devinfo`: `OpenAL_version()`
  assigned the result of `alGetString()` directly to `std::string` members, but
  those calls return `nullptr` when no OpenAL context is active. The values are
  now only assigned when the pointer is non-null, preserving the existing
  "N/A" default.

* Added `nom::audio::OggSound::vorbis_version_string()` and wired
  `examples/nom_devinfo` to log the linked libvorbis version when
  `USE_LIBOGG` is enabled.

* Fixed `examples/nom_devinfo` so it initializes an OpenAL context before
  querying `alGetString()`. The example now prints the real OpenAL version,
  renderer, vendor, and extension list when the audio unit is built.

* Renamed `USE_LIBOGG` to `NOM_USE_LIBOGG` across the build system and
  source so the macro follows the project's namespacing convention. The
  CMake flag in `src/audio/CMakeLists.txt` and the conditional compilation in
  `src/audio/SoundManager.cpp` and `examples/nom_devinfo` now use
  `NOM_USE_LIBOGG`.

* Refactored `examples/nom_app_ex` for per-window rendering passes. Each
  window now gets its own `make_current()` / clear / draw / present cycle
  inside the main loop, and the blinking cursor sprite has been moved from
  the primary window to the background window (`window[1]`). The animated
  cursor texture is loaded while `window[1]` is current so it belongs to that
  window's `SDL_Renderer`. Also added `src/graphics/scout_findings.md`
  documenting the current multi-window capabilities and blockers in
  `nom::RenderWindow` / `nom::Renderer`.

* Fixed `examples/nom_app_ex` spawning all windows on the same display.
  The requested `display_index` is now wrapped modulo the number of available
  displays, and after each window is created it is explicitly centered on its
  target display using `RenderWindow::display_bounds()` and
  `RenderWindow::set_position()` so the compositor cannot ignore the
  per-display hint.

* Integrated `nom::InputStateMapper` with `nom::StateMachine` and fixed the
  event-watch plumbing that previously prevented safe per-state input contexts.
  `EventHandler::remove_event_watch` now matches by
  `std::function::target_type()`; `InputStateMapper::set_event_handler` /
  `remove_event_handler` are idempotent and detachable; `IState` exposes a
  `virtual input_context()` key; and `StateMachine` auto-activates,
  disables, and erases that context across on_init / on_pause / on_resume /
  on_exit. Also fixed the `StateMachine::update()` pop/resume branch so it
  resets `next_state_`, preventing a double-pop on subsequent updates. A new
  unit test, `StateMachineTest`, exercises all six scenarios plus a
  `SixConcurrentStates` case in which six stacked child states print their
  first counter at one-second intervals while being popped back down.

* Updated `AGENTS.md` to document installing a preset with `cmake --install`
  and to note that `examples/nom_font` accepts both relative and absolute
  font file paths.

* Added symlink-aware filesystem query functions to `nom::filesystem`:
  `is_symlink`, `exists_symlink`, `is_symlink_directory`,
  `is_symlink_regular_file`, and `file_size_symlink`. These use `lstat` where
  available and do not follow symbolic links.

* Added `nom::filesystem::valid_utf8()` for explicitly validating UTF-8
  strings. On Windows, `to_wide_path()` now throws `filesystem_error` if the
  path contains invalid UTF-8, instead of silently producing a malformed wide
  string.

* Improved `examples/nom_font` startup: failure to load the requested font
  now terminates the example with a non-zero exit code instead of continuing
  with an invalid font.

* Switched `FileInterfaceTest` file creation to a platform-aware
  `open_output_stream()` helper so UTF-8 filenames use the wide-character
  `ofstream` constructor on Windows.

* Fixed `examples/GameOfLife/CMakeLists.txt` missing `STATIC_ASSETS` definition
  and using an undefined `${STATIC_ASSETS}` variable in its target sources and
  post-build copy command.

* Fixed `examples/GameOfLife/game.cpp` duplicate global definitions of
  `IMG_CREEP`, `BG_LIGHT`, and `BG_DARK` that caused a build failure.

* Changed `examples/GameOfLife/game.cpp` resource paths to be resolved at
  runtime. The example now locates its `Resources/GameOfLife` directory
  relative to the executable, supporting multiple build, install, and bundle
  layouts.

* Removed the old `nomlib-audio` (v1) implementation. The new audio
  subsystem previously under `nomlib-audio2` has taken its place as
  `nomlib-audio` in both include paths and CMake target names.

* Renamed `include/nomlib/audio2/` to `include/nomlib/audio/` and
  `src/audio2/` to `src/audio/`. All public headers are now under
  `nomlib/audio/`.

* Added a C++14-compatible `nom::filesystem` layer compatible with the
  ISO/IEC TS 18822:2015 (N4100) / Boost.Filesystem path model.

* Rewrote `nom::Path` as a thin wrapper around `nom::filesystem::path`.

* Refactored `nom::File`, `nom::UnixFile`, and `nom::WinFile` to use
  `nom::filesystem` for path decomposition and file system queries.

* Updated `nomlib-file` to build `src/system/filesystem.cpp` and install the
  public header `include/nomlib/system/filesystem.hpp`.

* Rewrote `examples/nom_aplay` and `examples/nom_astream` to use the new
  `nom::filesystem::path` API for path composition and extension queries.

* Extended `nom::filesystem::path` with UTF-8 / UTF-16 / wide string support
  matching the ISO/IEC TS 18822:2015 (N4100) / Boost.Filesystem interface:
  added `u8string_type`, `u16string_type`, `u8string()`, `u16string()`,
  `wstring()`, and their `generic_*` aliases, plus constructors and
  assignment operators from `std::u16string` and `std::wstring`.

* Added internal UTF-8 / UTF-16 / UTF-32 conversion helpers used by the new
  `nom::filesystem::path` string accessors.

* Switched the Windows `nom::filesystem` operational functions to use the
  wide Windows APIs (`_wstat`, `CreateDirectoryW`, `RemoveDirectoryW`,
  `DeleteFileW`, `_wgetcwd`, `_wchdir`) so UTF-8 paths round-trip correctly.

* Fixed `nom::filesystem::file_size()` on POSIX to return `st.st_size`
  instead of the previous accidental `st.st_mode`.

* Expanded `tests/src/system/FileInterfaceTest.cpp` with unit tests covering
  `nom::filesystem::path` construction, decomposition, UTF-8 / UTF-16 /
  wide-string round-trips, `operator/` / `operator+=` composition, and the
  `create_directory`, `create_directories`, `exists`, `is_directory`,
  `is_regular_file`, `remove`, `file_size`, and `current_path` operations.

* Switched the GameOfLife example from C++17 `std::filesystem` to the new
  C++14 `nom::filesystem` implementation and removed the target-specific
  C++17 requirement.

* Added `tests/src/audio/Audio2Test.cpp` as a unit test for the new
  `nomlib-audio` API. The test exercises initialization, `SoundManager`
  lookup, WAV metadata accessors, `IAudioDevice::play_sound`, and renderer
  updates. A streaming test case mirrors `examples/nom_astream`.

* Disabled the old v1 audio actions (`FadeAudioGainBy`, `PlayAudioSource`)
  in `src/actions/CMakeLists.txt` and `include/nomlib/actions.hpp` pending
  migration to the new audio API.

* Removed the old audio subsystem example stubs in `examples/nom_aplay`.

* Fixed `SoundManager::free_sounds()` to erase entries while iterating,
  removing stale pointers after deleting cached `Sound` objects.

* Fixed `nom::audio::shutdown_audio()` to release the active audio renderer
  before freeing cached `Sound` objects, preventing use-after-free during
  shutdown.

* Fixed `nom::audio::ISoundFileReader.hpp` to wrap its interface in the
  `nom::audio` namespace so that `SoundFileReader` can correctly implement
  the abstract base class.

## nom::SpriteSheet Sheets ##

## v0.2.0 ##

* nomlib no longer uses json_spirit for its JSON input and output; we now use the jsoncpp library.

* Comments -- // and /* */ styles -- are not allowed anymore; this was a decision I made to ensure a greater level of compatibility with JSON parsers. Use _comment fields.

* Sorting order is alphabetical now; This is believed to be due to jsoncpp using std::map in its implementation.

* The ID field is now all lowercase: id.

## v0.1.0 ##

* Initial release
