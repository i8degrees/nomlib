/******************************************************************************

    Simple UI interface for drawing a styled message box

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/gui/MessageBox.hpp"

namespace nom {

void MessageBox::initialize ( void )
{
NOM_LOG_CLASSINFO;

  this->enabled = true;
  this->lines.clear();
  this->window_borders.clear();
  this->coords = Coords ( 0, 0, 0, 0 );

  for ( ulong i = 0; i < this->window_borders.size(); ++i )
    this->window_borders[i] = Color::Black;
}

MessageBox::MessageBox ( void )
{
  initialize();
}

MessageBox::MessageBox  ( int32 x, int32 y, int32 width, int32 height,
                                  const std::vector<Color> border_colors,
                                  const Gradient& background
                                )
{
  unsigned int padding = 1;
  unsigned int x_offset = x + width; //unsigned int x_offset = ( x + width ) - padding;
  unsigned int y_offset = y + height; //unsigned int y_offset = ( y + height ) - padding;

  initialize();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );
  this->window_borders = border_colors;
  this->background = background;

  this->background = Gradient ( background.getStartColor(), background.getEndColor(), this->coords.x, this->coords.y, this->coords.width, this->coords.height, background.getFillDirection(), 0, 0 );

  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x, y, x_offset - padding, y, this->window_borders[0].getColor() ) ) ); // top0
  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x, y + 1, x_offset - padding, y + 1, this->window_borders[1].getColor() ) ) ); // top1
  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x, y + 1, x, y_offset - padding, this->window_borders[2].getColor() ) ) ); // left0
  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x + 1, y + 2, x + 1, y_offset - padding, this->window_borders[3].getColor() ) ) ); // left1
  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x, y_offset - padding, x_offset - padding, y_offset - padding, this->window_borders[4].getColor() ) ) ); //bottom0
  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x, y_offset, x_offset + padding, y_offset, this->window_borders[5].getColor() ) ) ); // bottom1
  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x_offset - padding, y, x_offset - padding, y_offset + padding, this->window_borders[6].getColor() ) ) ); // right0
  this->lines.push_back ( std::shared_ptr<IDrawable> ( new Line ( x_offset, y, x_offset, y_offset + padding, this->window_borders[7].getColor() ) ) ); // right1
}

MessageBox::~MessageBox ( void )
{
NOM_LOG_CLASSINFO;

  this->enabled = false;

  this->lines.clear(); // Better safe than sorry!
}

bool MessageBox::isEnabled ( void )
{
  if ( this->enabled == true )
    return true;
  else
    return false;
}

void MessageBox::disable ( void )
{
  this->enabled = false;
}

void MessageBox::enable ( void )
{
  this->enabled = true;
}

void MessageBox::Update ( void )
{
  // ...
}

// FIXME: how do we iterate through this with unique_ptr type ?
void MessageBox::Draw ( void* video_buffer ) const
{
  this->background.Draw ( video_buffer );

  // nom::Line needs pixel write access, so we must first lock the surface
  this->box.Lock ( video_buffer );

  for ( auto it = this->lines.begin(); it != this->lines.end(); ++it )
  {
    std::shared_ptr<IDrawable> obj = *it;
    obj->Draw ( video_buffer );
  }

  this->box.Unlock ( video_buffer );
}


} // namespace nom
