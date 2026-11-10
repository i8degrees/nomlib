# Change Log #

## Unreleased ##

* Fixed `examples/GameOfLife/CMakeLists.txt` missing `STATIC_ASSETS` definition
  and using an undefined `${STATIC_ASSETS}` variable in its target sources and
  post-build copy command.

* Fixed `examples/GameOfLife/game.cpp` duplicate global definitions of
  `IMG_CREEP`, `BG_LIGHT`, and `BG_DARK` that caused a build failure.

* Changed `examples/GameOfLife/game.cpp` resource paths to be resolved at
  runtime. The example now locates its `Resources/GameOfLife` directory
  relative to the executable, supporting multiple build, install, and bundle
  layouts.

* Added a C++14-compatible `nom::filesystem` layer compatible with the
  ISO/IEC TS 18822:2015 (N4100) / Boost.Filesystem path model.

* Rewrote `nom::Path` as a thin wrapper around `nom::filesystem::path`.

* Refactored `nom::File`, `nom::UnixFile`, and `nom::WinFile` to use
  `nom::filesystem` for path decomposition and file system queries.

* Updated `nomlib-file` to build `src/system/filesystem.cpp` and install the
  public header `include/nomlib/system/filesystem.hpp`.

* Switched the GameOfLife example from C++17 `std::filesystem` to the new
  C++14 `nom::filesystem` implementation and removed the target-specific
  C++17 requirement.

* Added `tests/src/audio2/Audio2Test.cpp` as a unit test for `nomlib-audio2`.
  The test exercises initialization, `SoundManager` lookup, WAV metadata
  accessors, `IAudioDevice::play_sound`, and renderer updates.

* Fixed `SoundManager::free_sounds()` to erase entries while iterating,
  removing stale pointers after deleting cached `Sound` objects.

* Fixed `nom::audio::shutdown_audio()` to release the active audio renderer
  before freeing cached `Sound` objects, preventing use-after-free during
  shutdown.

## nom::SpriteSheet Sheets ##

## v0.2.0 ##

* nomlib no longer uses json_spirit for its JSON input and output; we now use the jsoncpp library.

* Comments -- // and /* */ styles -- are not allowed anymore; this was a decision I made to ensure a greater level of compatibility with JSON parsers. Use _comment fields.

* Sorting order is alphabetical now; This is believed to be due to jsoncpp using std::map in its implementation.

* The ID field is now all lowercase: id.

## v0.1.0 ##

* Initial release
