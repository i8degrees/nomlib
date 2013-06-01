# nom (gamelib)

## TODO

* Rename GColor to nom::Color

* GColor::getSDLColor or such that will return to us a SDL_Color struct for use in
SDL_TFont and wherever else

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

* rename SDL_TFont && SDL_BitmapFont::setTextBuffer to setText
* rename methods in gamelib/Sprite to lowerCaseCamelStyle
* Clean up Timer class; 1. match Timer & FPS; <strikethrough> 2. goodbye FPS (!) </strikethrough>
* msgbox bottom, right border (needs additional depth)

### Footer

Copyright (c) 2013 Jeffrey Carpenter
