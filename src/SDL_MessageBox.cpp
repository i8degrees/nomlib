/******************************************************************************
    SDL_MessageBox.cpp

  Simple UI interface for drawing a styled message box

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_MessageBox.hpp"

nom::SDL_MessageBox::SDL_MessageBox ( void )
{
  #ifdef DEBUG_SDL_MESSAGEBOX_OBJ
    std::cout << "SDL_MessageBox::SDL_MessageBox (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->enabled = true;

  this->window_borders.clear();

  for ( int32_t i = 0; i < this->window_borders.size(); i++ )
    this->window_borders[i] = nom::Color::Black;

  this->coords = nom::Coords ( 0, 0, 0, 0 );

  this->background = NULL; // SDL_Gradient
}


nom::SDL_MessageBox::~SDL_MessageBox ( void )
{
  #ifdef DEBUG_SDL_MESSAGEBOX_OBJ
    std::cout << "SDL_MessageBox::~SDL_MessageBox (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  this->enabled = false;

  // Goodbye cruel drawables!
  //for ( auto it = this->lines.begin(); it != this->lines.end(); it++ )
  //{
    //delete *it;
  //}

  this->lines.clear(); // Better safe than sorry!

  this->background = NULL; // SDL_Gradient
}

void nom::SDL_MessageBox::Init ( int32_t x, int32_t y, int32_t width, int32_t height, const std::vector<nom::Color> border_colors, nom::SDL_Gradient* gradient )
{
  unsigned int padding = 1;
  unsigned int x_offset = x + width; //unsigned int x_offset = ( x + width ) - padding;
  unsigned int y_offset = y + height; //unsigned int y_offset = ( y + height ) - padding;

  // init geometry coords w/ arguments list
  this->coords = nom::Coords ( x, y, width, height );
  this->window_borders = border_colors;
  this->background = gradient;

  if ( this->background != nullptr )
    this->background->Init ( nom::Color ( 66, 66, 66 ), nom::Color ( 99, 99, 99 ), this->coords.x, this->coords.y, this->coords.width, this->coords.height, 0, 0, 0 );

  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x, y, x_offset - padding, y, this->window_borders[0].getColor() ) ) ); // top0
  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x, y + 1, x_offset - padding, y + 1, this->window_borders[1].getColor() ) ) ); // top1
  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x, y + 1, x, y_offset - padding, this->window_borders[2].getColor() ) ) ); // left0
  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x + 1, y + 2, x + 1, y_offset - padding, this->window_borders[3].getColor() ) ) ); // left1
  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x, y_offset - padding, x_offset - padding, y_offset - padding, this->window_borders[4].getColor() ) ) ); //bottom0
  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x, y_offset, x_offset + padding, y_offset, this->window_borders[5].getColor() ) ) ); // bottom1
  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x_offset - padding, y, x_offset - padding, y_offset + padding, this->window_borders[6].getColor() ) ) ); // right0
  this->lines.push_back ( std::shared_ptr<nom::SDL_Drawable> ( new nom::Line ( x_offset, y, x_offset, y_offset + padding, this->window_borders[7].getColor() ) ) ); // right1
}

bool nom::SDL_MessageBox::isEnabled ( void )
{
  if ( this->enabled == true )
    return true;
  else
    return false;
}

void nom::SDL_MessageBox::disable ( void )
{
  this->enabled = false;
}

void nom::SDL_MessageBox::enable ( void )
{
  this->enabled = true;
}

// FIXME: how do we iterate through this with unique_ptr type ?
void nom::SDL_MessageBox::Update ( void )
{
  for ( auto it = this->lines.begin(); it != this->lines.end(); it++ )
  {
    std::shared_ptr<nom::SDL_Drawable> obj = *it;
    obj->Update();
  }
}

// FIXME: how do we iterate through this with unique_ptr type ?
void nom::SDL_MessageBox::Draw ( void* video_buffer ) const
{
  if ( this->background != nullptr )
  {
    background->Draw ( video_buffer, this->coords.x, this->coords.y, this->coords.width, this->coords.height, 0 );
  }

  if ( this->box.mustLock ( video_buffer ) == true )
  {
    // Not sure if this is the proper placement of the check, but seems to do
    // alright for now!
    while ( this->box.lockCanvas ( video_buffer ) == false )
      SDL_Delay ( 10 );

    for ( auto it = this->lines.begin(); it != this->lines.end(); it++ )
    {
      std::shared_ptr<nom::SDL_Drawable> obj = *it;
      obj->Draw ( video_buffer );
    }

    this->box.unlockCanvas ( video_buffer );
  }
}
