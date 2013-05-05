/******************************************************************************
    input.cpp

    Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "input.h"

SDLInput::SDLInput ( void )
{
  #ifdef DEBUG_INPUT_OBJ
    std::cout << "SDLInput::SDLInput(): Hello, world!" << std::endl << std::endl;
  #endif

  // Init
}

SDLInput::~SDLInput ( void )
{
  #ifdef DEBUG_INPUT_OBJ
    std::cout << "SDLInput::~SDLInput(): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Cleanup of vars, etc.
}
