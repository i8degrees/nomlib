# Change Log #

## Unreleased ##

* Fixed `examples/GameOfLife/CMakeLists.txt` missing `STATIC_ASSETS` definition and
  using an undefined `${STATIC_ASSETS}` variable in its target sources and
  post-build copy command.

* Fixed `examples/GameOfLife/game.cpp` duplicate global definitions of
  `IMG_CREEP`, `BG_LIGHT`, and `BG_DARK` that caused a build failure.

* Changed `examples/GameOfLife/game.cpp` resource paths to be resolved at
  runtime with C++17 `std::filesystem`. The example now locates its
  `Resources/GameOfLife` directory relative to the executable, supporting
  multiple build, install, and bundle layouts.

* Added `target_compile_features(nom_GameOfLife PRIVATE cxx_std_17)` to enable
  `<filesystem>` support for the GameOfLife example.

## nom::SpriteSheet Sheets ##

## v0.2.0 ##

* nomlib no longer uses json_spirit for its JSON input and output; we now use the jsoncpp library.

* Comments -- // and /* */ styles -- are not allowed anymore; this was a decision I made to ensure a greater level of compatibility with JSON parsers. Use _comment fields.

* Sorting order is alphabetical now; This is believed to be due to jsoncpp using std::map in its implementation.

* The ID field is now all lowercase: id.

## v0.1.0 ##

* Initial release
