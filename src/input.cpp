/******************************************************************************
    input.cpp

    Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "input.h"

void SDLInput::HandleInput ( SDL_Event *input )
{

  switch ( input->type )
  {
    case SDL_QUIT:
      onExit();
    break;

    case SDL_VIDEORESIZE:
      onResize();
    break;

    case SDL_KEYDOWN:
      keyDown ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_KEYUP:
      keyUp ( input->key.keysym.sym, input->key.keysym.mod );
    break;

    case SDL_MOUSEMOTION:
      mouseMotion ( input->motion.x, input->motion.y );
    break;

    case SDL_MOUSEBUTTONDOWN:
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          mouseLeftButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_MIDDLE:
          mouseMiddleButtonDown ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_RIGHT:
          mouseRightButtonDown ( input->button.x, input->button.y );
        break;
      }
    break;

    case SDL_MOUSEBUTTONUP:
      switch ( input->button.button )
      {
        case SDL_BUTTON_LEFT:
          mouseLeftButtonUp ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_MIDDLE:
          mouseMiddleButtonUp ( input->button.x, input->button.y );
        break;

        case SDL_BUTTON_RIGHT:
          mouseRightButtonUp ( input->button.x, input->button.y );
        break;
      }
    break;
  } // end switch input->type

}

void SDLInput::onExit ( void )
{
}

void SDLInput::onResize ( void )
{
}

void SDLInput::keyDown ( SDLKey key, SDLMod mod )
{
}

void SDLInput::keyUp ( SDLKey key, SDLMod mod )
{
}

void SDLInput::mouseMotion ( unsigned int x, unsigned int y )
{
}

void SDLInput::mouseLeftButtonDown ( unsigned int x, unsigned int y )
{
}

void SDLInput::mouseMiddleButtonDown ( unsigned int x, unsigned int y )
{
}

void SDLInput::mouseRightButtonDown ( unsigned int x, unsigned int y )
{
}

void SDLInput::mouseLeftButtonUp ( unsigned int x, unsigned int y )
{
}

void SDLInput::mouseMiddleButtonUp ( unsigned int x, unsigned int y )
{
}

void SDLInput::mouseRightButtonUp ( unsigned int x, unsigned int y )
{
}
