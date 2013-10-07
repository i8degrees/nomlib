# TODO #

- [ ] Use C++ templates for Coords class
- [ ] joystick buttons demo
- [ ] Homebrew Formula?
- [ ] nomlib_examples submodule repo and makefile setup
- [ ] Finish Update logic in BitmapFont class so that we can make it an official
Drawables type.
- [ ] Implement a NULL GameState class?
- [ ] Use Uint32, etc. definitions
- [ ] const types
- [ ] Test alpha values within color class with graphics engine; experimental support is there
- [x] MSVC++ does not accept valid C++11 code in EventDispatcher by defining an enum class.
> Resolved by removing the desire for the enum class (duplicate definitions). I moved the enum definition to inside the class.
- [x] Makefile Dependencies
- [x] Add SDL_EnableKeyRepeat() into SDLInput
- [x] Fix Color ( -1, -1, -1, -1 ) issue
- [x] GameStates as a singleton class?
- [x] Perhaps GameState could also implement Load method?
- [x] namespace nom
- [x] Use nullptr instead of NULL
- [x] +=, ++, etc. overload methods for Sprite class among other potential candidates
- [x] Wrap SDL_PollEvent ( &event ) within GameStates into SDLInput
- [x] color constants (think: white, black, etc)
- [x] Use coords class in sprite
- [x] Fix Coords offsets ( -1, -1, -1, -1 ) issue with blitting
- [x] Rename class SDLBitmapFont to SDL_BitmapFont
- [x] GColor::getSDLColor or such that will return to us a SDL_Color struct for use in SDL_TFont and wherever else
- [x] Use GColor::getColorAsInt everywhere as needed
- [x] Rename GColor to nom::Color
- [x] GameState::Update ( SDL_Surface *video_buffer ) ?
- [x] GameState::Draw ( SDL_Surface *video_buffer ) ?
- [x] rename SDL_TFont && SDL_BitmapFont::setTextBuffer to setText
- [x] rename methods in nomlib/Sprite to lowerCaseCamelStyle
- [x] Clean up Timer class; 1. match Timer & FPS
- [x] msgbox bottom, right border (needs additional depth)
- [x] Cross-platform considerations (think: Makefiles, OS bins & args, etc.)
- [x] Library versioning (think: sonames)
- [x] man install(1), pkg-config(1)
