# nom (gamelib)

## TODO

* Use nullptr instead of NULL
* Fix Color ( -1, -1, -1, -1 ) issue
* Fix GameStates leak due to vector not destructing until end of app exec
* Implement a NULL GameState class
* Perhaps GameState could also implement Load method
* Perhaps break Coords class into just x, y points
* Use exceptions library in C++
* Use Uint32, etc. definitions
* const types
* color constants (think: white, black, etc)
* Use coords class in sprite
* Test alpha values within color class with graphics engine; experimental support
is there

* joystick buttons demo

* gamelib_examples submodule repo and makefile setup

* namespace nom

* Add SDL_EnableKeyRepeat() into SDLInput

* Wrap SDL_PollEvent ( &event ) within GameStates into SDLInput

* +=, ++, etc. overload methods for Sprite class among other potential candidates
> http://stackoverflow.com/questions/4421706/operator-overloading
> http://courses.cms.caltech.edu/cs11/material/cpp/donnie/cpp-ops.html

* Try using ( SDL_WaitEvent == true ) wrapped within main.cpp while loop or such
so that we do not prematurely terminate while we may still have events in queue;
this may fix the SDLInput crashes within TTcards
> while ( game.isRunning() && SDL_WaitEvent() != 0 )
> See SDL-- library for idea details

## Completed

* Fix Coords offsets ( -1, -1, -1, -1 ) issue with blitting
* Rename class SDLBitmapFont to SDL_BitmapFont
* GColor::getSDLColor or such that will return to us a SDL_Color struct for use in
SDL_TFont and wherever else
* Use GColor::getColorAsInt everywhere as needed
* Rename GColor to nom::Color
* GameState::Update ( SDL_Surface *video_buffer ) ?
* GameState::Draw ( SDL_Surface *video_buffer ) ?
* rename SDL_TFont && SDL_BitmapFont::setTextBuffer to setText
* rename methods in gamelib/Sprite to lowerCaseCamelStyle
* Clean up Timer class; 1. match Timer & FPS; <strikethrough> 2. goodbye FPS (!) </strikethrough>
* msgbox bottom, right border (needs additional depth)

### Footer

Copyright (c) 2013 Jeffrey Carpenter
