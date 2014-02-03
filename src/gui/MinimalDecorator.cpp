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
#include "nomlib/gui/MinimalDecorator.hpp"

namespace nom {

MinimalDecorator::MinimalDecorator( void )
{
  // NOM_LOG_TRACE( NOM );
}

MinimalDecorator::~MinimalDecorator( void )
{
  // NOM_LOG_TRACE( NOM );
}

ObjectTypeInfo MinimalDecorator::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( MinimalDecorator );
}

IDrawable::raw_ptr MinimalDecorator::clone( void ) const
{
  return( MinimalDecorator::raw_ptr( new MinimalDecorator( *this ) ) );
}

void MinimalDecorator::update( void )
{
  IntRect margins = this->margins();

  FinalFantasyFrame::raw_ptr frame = nullptr;

  // Everything is already up-to-date; nothing to do...
  if( this->drawables_.updated() == true ) return;

  // Wipe our existing object state; we are out of date and need to rebuild
  this->drawables_.clear();

  frame = new FinalFantasyFrame();

  // Initialize the border frame with the position and size coordinates that
  // our caller hopefully remembered to set on us.
  if( margins == IntRect::null )
  {
    frame->set_position( this->position() );
    frame->set_size( this->size() );
  }
  else
  {
    // FIXME: See VBoxLayout::set_bounds for incomplete / broken implementation;
    // reasons unknown...
    frame->set_position( this->position() );
    // frame->set_size( this->size() + margins.size() );

    frame->set_size( Size2i( this->size().w, this->size().h + margins.size().h ) );

NOM_DUMP( margins );
  }

  this->drawables_.push_back( frame );

  // Update rendering
  for( auto itr = this->drawables_.begin(); itr != this->drawables_.end(); ++itr )
  {
    (*itr)->update();
  }

  // Set the intended unaffected rendering coordinates of the border we are
  // using.
  this->set_frame_bounds( frame->frame_bounds() );

  // Dirty flag cleared; we are up-to-date
  this->drawables_.set_updated( true );
}

void MinimalDecorator::draw( const RenderTarget& target ) const
{
  // Try to render whatever we may have cached from ::update.
  for( auto itr = this->drawables_.begin(); itr != this->drawables_.end(); ++itr )
  {
    (*itr)->draw( target );
  }
}

} // namespace nom
