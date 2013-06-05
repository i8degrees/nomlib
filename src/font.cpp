/******************************************************************************
    Font.cpp

  Base font class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "Font.h"

using namespace nom;

Font::Font ( void )
{
  #ifdef DEBUG_FONT_OBJ
    std::cout << "Font::Font (): " << "Hello, world!" << "\n" << std::endl;
  #endif
}

Font::~Font ( void )
{
  #ifdef DEBUG_FONT_OBJ
    std::cout << "Font::~Font (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

signed int Font::getX ( void )
{
  return this->coords.getX();
}

signed int Font::getY ( void )
{
  return this->coords.getY();
}

Coords Font::getXY ( void )
{
  return this->coords;
}

void Font::setX ( signed int x_ )
{
  this->coords.setX ( x_ );
}

void Font::setY ( signed int y_ )
{
  this->coords.setY ( y_ );
}

void Font::setXY ( signed int x_, signed int y_ )
{
  this->coords.setXY ( x_, y_ );
}

nom::Color Font::getTextColor ( void )
{
  return this->text_color;
}

void Font::setTextColor ( const Color &color )
{
  this->text_color = color;
}

std::string Font::getText ( void )
{
  return this->text_buffer;
}

void Font::setText ( std::string text )
{
  this->text_buffer = text;
}
