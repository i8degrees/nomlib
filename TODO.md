# TODO #

## Incomplete ##

* Use C++ templates for Coords class

* Finish Update logic in BitmapFont class so that we can make it an official
Drawables type.

* Implement a NULL GameState class?
* Use Uint32, etc. definitions
* const types

* Test alpha values within color class with graphics engine; experimental support is there

* joystick buttons demo
* Homebrew Formula?

* nomlib_examples submodule repo and makefile setup

## Completed ##

* Makefile Dependencies
* Add SDL_EnableKeyRepeat() into SDLInput
* Fix Color ( -1, -1, -1, -1 ) issue
* GameStates as a singleton class?

* Perhaps GameState could also implement Load method?
* namespace nom
* Use nullptr instead of NULL

* * +=, ++, etc. overload methods for Sprite class among other potential candidates
  > Coords, Color classes now implement this.

* Wrap SDL_PollEvent ( &event ) within GameStates into SDLInput

* color constants (think: white, black, etc)

* Use coords class in sprite
* Fix Coords offsets ( -1, -1, -1, -1 ) issue with blitting
* Rename class SDLBitmapFont to SDL_BitmapFont
* GColor::getSDLColor or such that will return to us a SDL_Color struct for use in
SDL_TFont and wherever else
* Use GColor::getColorAsInt everywhere as needed
* Rename GColor to nom::Color
* GameState::Update ( SDL_Surface *video_buffer ) ?
* GameState::Draw ( SDL_Surface *video_buffer ) ?
* rename SDL_TFont && SDL_BitmapFont::setTextBuffer to setText
* rename methods in nomlib/Sprite to lowerCaseCamelStyle
* Clean up Timer class; 1. match Timer & FPS
* msgbox bottom, right border (needs additional depth)
* Cross-platform considerations (think: Makefiles, OS bins & args, etc.)
* Library versioning (think: sonames)
* man install(1), pkg-config(1)
