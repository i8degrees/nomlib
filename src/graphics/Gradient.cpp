/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "nomlib/graphics/Gradient.hpp"

namespace nom {

Gradient::Gradient( void ) :
  Transformable( Point2i::null, Size2i::null ),   // Invalid position & size
  gradient_{ { Color4i::Blue, Color4i::Blue } }   // Opaque color to serve as
                                                  // warning!
{
  // No margins set.
  this->set_margins( Point2i( 0, 0 ) );

  // Default direction is left to right.
  this->set_fill_direction( Gradient::FillDirection::Left );

  this->set_dithering( true );

  // No need to update the object's rendered state because we do not have
  // sufficient information for rendering to occur -- position & size.
}

Gradient::Gradient  (
                      const Color4iColors& colors,
                      const Point2i& pos,
                      const Size2i& size,
                      const Point2i& margin,
                      Gradient::FillDirection direction
                    ) :
  Transformable( pos, size ), // Base class
  gradient_{ colors }
{
  this->set_margins( margin );
  this->set_fill_direction( direction );
  this->set_dithering( true );

  this->update();
}

Gradient::~Gradient( void )
{
  //NOM_LOG_TRACE( NOM );
}

Gradient::Gradient( const self_type& copy )  :
  Transformable { copy.position(), copy.size() }, // Base class
  rectangles_ { copy.rectangles_ },
  gradient_{ copy.colors() },
  margins_ { copy.margins() },
  fill_direction_ { copy.fill_direction() },
  dithering_{ copy.dithering() }
{
  // NOM_LOG_TRACE( NOM );

  this->update();
}

IDrawable::raw_ptr Gradient::clone( void ) const
{
  return Gradient::raw_ptr( new Gradient( *this ) );
}

ObjectTypeInfo Gradient::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

bool Gradient::valid( void ) const
{
  if ( this->position() != Point2i::null && this->size() != Size2i::null )
  {
    return true;
  }

  return false;
}

bool Gradient::dithering( void ) const
{
  return this->dithering_;
}

const Point2i& Gradient::margins ( void ) const
{
  return this->margins_;
}

const Color4iColors& Gradient::colors( void ) const
{
  return this->gradient_;
}

void Gradient::set_position( const Point2i& pos )
{
  Transformable::set_position( pos );

  this->update();
}

void Gradient::set_size( const Size2i& size )
{
  Transformable::set_size( size );

  this->update();
}

const Color4i& Gradient::start_color( void ) const
{
  return this->gradient_.front();
}

const Color4i& Gradient::end_color( void ) const
{
  return this->gradient_.back();
}

void Gradient::set_colors( const Color4iColors& colors )
{
  this->gradient_ = colors;

  this->update();
}

void Gradient::reverse_colors( void )
{
  // Front to back and back to front
  std::swap( this->gradient_.front(), this->gradient_.back() );
}

Gradient::FillDirection Gradient::fill_direction ( void ) const
{
  return this->fill_direction_;
}

void Gradient::set_fill_direction ( Gradient::FillDirection direction )
{
  this->fill_direction_ = direction;

  this->update();
}

void Gradient::set_margins ( const Point2i& margin )
{
  this->margins_ = margin;

  this->update();
}

void Gradient::set_dithering( bool state )
{
  this->dithering_ = state;

  this->update();
}

void Gradient::draw( RenderTarget& target ) const
{
  for ( auto itr = this->rectangles_.begin(); itr != this->rectangles_.end(); ++itr )
  {
    (*itr)->draw( target );
  }
}

// Private scope

void Gradient::update( void )
{
  // Ensure that we do not try to access an empty container -- that would be
  // undefined behavior; a segmentation fault (crash) if we are lucky!
  if( this->colors().empty() )
  {
    this->set_colors( { Color4i::Blue, Color4i::Blue } );
  }

  if( this->valid() == false ) return;

  // Clear the rendered drawables of the previously up-to-date object.
  this->rectangles_.clear();

  if ( this->fill_direction() == FillDirection::Top )
  {
    this->strategy_top_down();
  }
  else if ( this->fill_direction() == FillDirection::Bottom )
  {
    this->reverse_colors();
    this->strategy_top_down();
  }
  else if ( this->fill_direction() == FillDirection::Left )
  {
    this->strategy_left_right();
  }
  else if ( this->fill_direction() == FillDirection::Right )
  {
    this->reverse_colors();
    this->strategy_left_right();
  }
}

void Gradient::strategy_top_down ( void )
{
  uint32 y_offset = ( this->position().y + this->size().h ) - this->margins().y;

  Color4i first_color = this->start_color();
  Color4i last_color = this->end_color();

  float currentR = (float) first_color.r;
  float currentG = (float) first_color.g;
  float currentB = (float) first_color.b;

  float destR = (float) ( last_color.r - first_color.r )      / ( float ) ( this->size().h - this->margins().y );
  float destG = (float) ( last_color.g - first_color.g )  / ( float ) ( this->size().h - this->margins().y );
  float destB = (float) ( last_color.b - first_color.b )    / ( float ) ( this->size().h - this->margins().y );

  for ( uint32 rows = this->position().y + this->margins().y; rows < y_offset; rows++ )
  {
    // Calculate rendering offsets
    IntRect render_coords = IntRect( this->position().x + this->margins().x, rows, this->size().w - this->margins().x, 1 );
    Color4i render_color = Color4i( currentR, currentG, currentB );

    // Queue up to render
    this->rectangles_.push_back( Rectangle::shared_ptr( new Rectangle( render_coords, render_color ) ) );

    if ( this->dithering() )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
  } // end blit loop
}

void Gradient::strategy_left_right ( void )
{
  uint32 x_offset = ( this->position().x + this->size().w ) - this->margins().x;

  Color4i first_color = this->start_color();
  Color4i last_color = this->end_color();

  float currentR = (float) first_color.r;
  float currentG = (float) first_color.g;
  float currentB = (float) first_color.b;

  float destR = (float) ( last_color.r - first_color.r )      / ( float ) ( this->size().w - this->margins().x );
  float destG = (float) ( last_color.g - first_color.g )  / ( float ) ( this->size().w - this->margins().x );
  float destB = (float) ( last_color.b - first_color.b )    / ( float ) ( this->size().w - this->margins().x );

  for ( uint32 rows = this->position().x + this->margins().x; rows < x_offset; rows++ )
  {
    // Calculate rendering offsets
    IntRect render_coords = IntRect( rows, this->position().y + this->margins().y, 1, this->size().h - this->margins().y );
    Color4i render_color = Color4i( currentR, currentG, currentB );

    // Queue up to render
    this->rectangles_.push_back( Rectangle::shared_ptr( new Rectangle( render_coords, render_color ) ) );

    if ( this->dithering() )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
  } // end blit loop
}

} // namespace nom
