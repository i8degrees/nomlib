/******************************************************************************
    sprite.cpp

    SDL-based Sprite Blitting API

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "sprite.h"

using namespace nom;

nom::Sprite::Sprite ( void )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::Sprite (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->state = 0;

  this->sheet.sprite_width = 0;
  this->sheet.sprite_height = 0;
  this->sheet.width = 0;
  this->sheet.height = 0;
  this->sheet.id = -1;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

nom::Sprite::Sprite ( unsigned int width, unsigned int height )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::Sprite (): " << "Hello, world!" << "\n" << std::endl;
  #endif

  this->coords.setWidth ( width );
  this->coords.setHeight ( width );

  this->state = 0;

  this->sheet.id = -1;
  this->sheet.sprite_width = 0;
  this->sheet.sprite_height = 0;
  this->sheet.width = width = 0;
  this->sheet.height = height = 0;
  this->sheet.spacing = 0;
  this->sheet.padding = 0;
}

nom::Sprite::~Sprite ( void )
{
  #ifdef DEBUG_SPRITE_OBJ
    std::cout << "Sprite::~Sprite (): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif
}

unsigned int nom::Sprite::getState ( void )
{
  return this->state;
}

void nom::Sprite::setState ( unsigned int state )
{
  this->state = state;
}

signed int nom::Sprite::getSheetID ( void )
{
  return this->sheet.id;
}

void nom::Sprite::setSheetID ( signed int id )
{
  this->sheet.id = id;
}

void nom::Sprite::setSheetDimensions ( unsigned int sheet_width, unsigned int sheet_height, unsigned int spacing, unsigned int padding )
{
  this->sheet.sprite_width = this->getWidth();
  this->sheet.sprite_height = this->getHeight();
  this->sheet.width = sheet_width;
  this->sheet.height = sheet_height;
  this->sheet.spacing = spacing;
  this->sheet.padding = padding;
}

bool nom::Sprite::Load ( std::string filename, nom::Color colorkey, unsigned int flags )
{
  this->sprite_buffer.loadImageFromFile ( filename, colorkey, flags );

  if ( this->sprite_buffer.get() == nullptr )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::Load (): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  return true;
}

void nom::Sprite::Draw ( void* video_buffer )
{
  nom::Coords offsets; // FIXME

  if ( this->sprite_buffer.get() == nullptr )
  {
    #ifdef DEBUG_SPRITE
      std::cout << "ERR in Sprite::Draw(): " << "NULL sprite_buffer" << std::endl << std::endl;
    #endif
  }

  if ( this->sheet.id != -1 )
  {
    // FIXME: Presently, we assume every sprite on our sheet is on the same row
    offsets.setX ( this->sheet.id * this->sheet.sprite_width );
    offsets.setY ( 0 );
    offsets.setDimensions ( this->sheet.sprite_width, this->sheet.sprite_height );
    this->sprite_buffer.setOffsets ( offsets );
  }

  this->sprite_buffer.setPosition ( coords );
  this->sprite_buffer.Draw ( video_buffer );
}
