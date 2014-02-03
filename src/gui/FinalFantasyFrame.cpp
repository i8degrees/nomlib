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
#include "nomlib/gui/FinalFantasyFrame.hpp"

namespace nom {

FinalFantasyFrame::FinalFantasyFrame( void )
{
  // NOM_LOG_TRACE( NOM );

  // No need to update the object's rendered state because we do not have
  // sufficient information for rendering to occur -- position & size.
  // this->update();
}

FinalFantasyFrame::~FinalFantasyFrame( void )
{
  // NOM_LOG_TRACE( NOM );
}

FinalFantasyFrame::FinalFantasyFrame  (
                                        const Point2i& pos,
                                        const Size2i& size
                                      ) :
  Transformable{ pos, size }
{
  // NOM_LOG_TRACE( NOM );

  this->update();
}

IDrawable::raw_ptr FinalFantasyFrame::clone( void ) const
{
  return FinalFantasyFrame::raw_ptr( new FinalFantasyFrame( *this ) );
}

ObjectTypeInfo FinalFantasyFrame::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

bool FinalFantasyFrame::valid( void ) const
{
  if( this->position() != Point2i::null && this->size() != Size2i::null )
  {
    // Valid object state
    return true;
  }

  // Invalid object state
  return false;
}

const IntRect& FinalFantasyFrame::frame_bounds( void ) const
{
  return this->frame_bounds_;
}

void FinalFantasyFrame::draw( RenderTarget& target ) const
{
  Drawables::container drawables = this->drawables_.drawables();

  for( auto itr = drawables.begin(); itr != drawables.end(); ++itr )
  {
    (*itr)->draw( target );
  }
}

void FinalFantasyFrame::set_frame_bounds( const IntRect& bounds )
{
  this->frame_bounds_ = bounds;
}

// Private scope

void FinalFantasyFrame::update( void )
{
  // Our rendering bounds are invalid -- nothing to render!
  if( this->valid() == false ) return;

  // We have already updated our window -- nothing to render!
  if ( this->drawables_.updated() == true ) return;

  // Need to always clear our data cache before updating
  this->drawables_.clear();

  int x = this->position().x;
  int y = this->position().y;

  int width = this->size().w;
  int height = this->size().h;

  int x_offset = x + width - 1;
  int y_offset = y + height - 1;

  // Outer border frame -- top0
  this->drawables_.push_back( new Line( IntRect( x, y, x_offset, y), Color4i::FinalFantasyOuterBorder ) );

  // Inner border frame -- top1
  this->drawables_.push_back( new Line( IntRect( x, y + 1, x_offset, y + 1), Color4i::FinalFantasyInnerBorder ) );

  // Outer border frame -- left0
  this->drawables_.push_back( new Line( IntRect( x, y + 1, x, y_offset), Color4i::FinalFantasyOuterBorder ) );

  // Inner border frame -- left1
  this->drawables_.push_back( new Line( IntRect( x + 1, y + 2, x + 1, y_offset), Color4i::FinalFantasyInnerBorder ) );

  // Outer border frame -- right0
  this->drawables_.push_back( new Line( IntRect( x_offset, y, x_offset, y_offset), Color4i::FinalFantasyOuterBorder ) );

  // Outer border frame -- bottom0
  this->drawables_.push_back( new Line( IntRect( x, y_offset, x_offset, y_offset), Color4i::FinalFantasyOuterBorder ) );

  // Calculate our border boundary; this is a convenience to other interfaces
  // that have to deal with such visual details, such as nom::UIWidget and
  // friends.
  Point2i frame_pos;
  Size2i frame_size;

  frame_pos.x = this->position().x + 2;
  frame_pos.y = this->position().y + 2;

  frame_size.w = this->size().w - 3;
  frame_size.h = this->size().h - 3;

  this->set_frame_bounds( IntRect( frame_pos, frame_size ) );

  this->drawables_.set_updated( true );
}

} // namespace nom
