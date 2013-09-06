# BUGS #

## Unresolved ##

* We need to figure out how to free BitmapFont & TrueTypeFont resources when we are using them inside the MessageBox class -- we are leaking memory as-is.

* Fix segmentation fault crash when magic.mgc cannot be found

## Resolved ##

* Finish Update logic in SDL_Gradient so that we can make it an official
Drawables type; we can then also make SDL_MessageBox an official Drawables as
well -- const issues prevent us from doing so at the moment.

* Fix nom::Line memory leak (update variable is merely a temporary partial stopgap)

* Fix GameStates "leak" due to vector not destructing until end of app exec

* Fix SDL_SetVideoMode memory leak when toggling fullscreen mode
  > This is a known library leak; not likely to be fixed (maybe libSDL v2 does?)
