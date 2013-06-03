/******************************************************************************
    SDL_Cursor.cpp

  Helper class for loading, positioning and keeping track of a cursor controlled
  by a mouse and / or keyboard.

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Cursor.h"

nom::SDL_Cursor::SDL_Cursor ( void )
{
  #ifdef DEBUG_SDL_CURSOR_OBJ
    std::cout << "SDL_Cursor::SDL_Cursor (): Hello, world!" << "\n" << std::endl;
  #endif

  // Not yet implemented
}

nom::SDL_Cursor::SDL_Cursor ( unsigned int x, unsigned int y, unsigned int width, unsigned int height )
{
  #ifdef DEBUG_SDL_CURSOR_OBJ
    std::cout << "SDL_Cursor::SDL_Cursor (): Hello, world!" << "\n" << std::endl;
  #endif

  this->cursor = nom::Sprite ( width, height );
  this->cursor.setXY ( x, y );
  this->cursor.setState ( 0 ); // default state init
}

nom::SDL_Cursor::~SDL_Cursor ( void )
{
  #ifdef DEBUG_SDL_CURSOR_OBJ
    std::cout << "SDL_Cursor::~SDL_Cursor (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  // Clean up
}

void nom::SDL_Cursor::Load ( std::string filename, nom::Color colorkey ) // sheet_id ?
{
  this->cursor.Load ( filename, colorkey );
}

unsigned int nom::SDL_Cursor::getX ( void )
{
  return this->cursor.getX();
}

unsigned int nom::SDL_Cursor::getY ( void )
{
  return this->cursor.getY();
}

void nom::SDL_Cursor::setX ( unsigned int x )
{
  this->cursor.setX ( x );
}

void nom::SDL_Cursor::setY ( unsigned int y )
{
  this->cursor.setY ( y );
}

void nom::SDL_Cursor::setXY ( unsigned int x, unsigned int y )
{
  this->cursor.setXY ( x, y );
}

void nom::SDL_Cursor::updateXY ( unsigned int x, unsigned int y )
{
  this->cursor.updateXY ( x, y );
}

signed int nom::SDL_Cursor::getSheetID ( void )
{
  return this->cursor.getSheetID();
}

void nom::SDL_Cursor::setSheetID ( signed int sheet_id )
{
  this->cursor.setSheetID ( sheet_id );
}

unsigned int nom::SDL_Cursor::getState ( void )
{
  return this->cursor.getState();
}

void nom::SDL_Cursor::setState ( unsigned int state )
{
  this->cursor.setState ( state );
}

void nom::SDL_Cursor::Update ( void )
{
  // Stub
}

void nom::SDL_Cursor::Draw ( SDL_Surface *video_buffer )
{
  this->cursor.Draw ( video_buffer );
}
