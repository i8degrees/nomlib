/******************************************************************************
    SDLWindowFrame.h

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef GAMELIB_WINDOWFRAME_HEADERS
#define GAMELIB_WINDOWFRAME_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

class SDLWindowFrame
{
  public:
    SDLWindowFrame ( void );
    ~SDLWindowFrame ( void );

    // ...

    std::string getWindowTitle ( void );
    void setWindowTitle ( std::string title );

  //protected:
    //GCoords geometry; // x, y, width & height
    // SDLWindowFrame pixmap;
    // SDLFont
    //GColor opacity;
    // SDLIcon
    //std::string object_name;
  private:
    std::string window_title;
};

#endif // GAMELIB_WINDOWFRAME_HEADERS defined
