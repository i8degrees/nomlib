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
#include "CustomUILayout.hpp"

// Private forward declarations
// #include "nomlib/gui/UIWidget.hpp"

namespace nom {

CustomUILayout::CustomUILayout( void )
{
  // NOM_LOG_TRACE( NOM );

  this->set_spacing( 10 );
}

CustomUILayout::~CustomUILayout( void )
{
  // NOM_LOG_TRACE( NOM );
}

CustomUILayout::CustomUILayout( UIWidget* parent ) :
  UILayout( parent )
{
  // NOM_LOG_TRACE( NOM );

  this->set_spacing( 10 );
}

Size2i CustomUILayout::size_hint( void ) const
{
  int idx = 0;  // Iterator
  int count = this->count();  // Items size

  // Default size hint; if nothing better comes along!
  Size2i psize( 0, 0 );

  if( count > 0 )
  {
    // start with a nice default size
    psize = Size2i( 100, 70 );

    // TODO: Let widget set its size hint???

    while( idx < count )
    {
      UILayoutItem* item = this->at( idx );

      UIWidget* widget = item->widget();

      if( widget != nullptr )
      {
        if( widget->font() )
        {
          FontMetrics face = widget->font()->metrics();

          // NOM_DUMP( widget->name() );
          // NOM_DUMP( widget->id() );
          NOM_DUMP( face.name );
        }
      }

      ++idx;
    }
  }

  idx = 0;

  while( idx < count )
  {
    UILayoutItem* item = this->at( idx );

    psize = psize.max( item->size_hint() );

// NOM_DUMP(psize);
// NOM_DUMP(item->size_hint());

    ++idx;
  }

  // return psize + count * Size2i( this->spacing(), this->spacing() );
  return Size2i( psize.w, psize.h + count * this->spacing() );
}

Size2i CustomUILayout::minimum_size( void ) const
{
  int idx = 0;  // Iterator
  int count = this->count();  // Items size

  // Default minimum size; if nothing better comes along!
  Size2i msize( 0, 0 );

  while( idx < count )
  {
    UILayoutItem* item = this->at( idx );

    msize = msize.max( item->minimum_size() );

    ++idx;
  }

  // return msize + count * Size2i( this->spacing(), this->spacing() );
  return Size2i( msize.w, msize.h + count * this->spacing() );
}

void CustomUILayout::set_bounds( const IntRect& rect )
{
  int idx = 0;  // Iterator
  int count = this->count();  // Items size

  UILayout::set_bounds( rect );

  if( count < 1 )
  {
    return;
  }

  int total_width = rect.w - ( count - 1 ) * this->spacing();
  int total_height = rect.h - ( count - 1 ) * this->spacing();

  while( idx < count )
  {
    UILayoutItem* item = this->at( idx );
    IntRect geom( rect.x + idx * this->spacing(), rect.y + idx * this->spacing(), total_width, total_height );
    item->set_bounds( geom );
    ++idx;
  }
}

int CustomUILayout::count( void ) const
{
  return this->items_.size();
}

UILayoutItem* CustomUILayout::at( int pos ) const
{
  if( pos < this->count() )
  {
    return this->items_.at( pos );
  }

  return nullptr;
}

void CustomUILayout::erase( int pos )
{
  if( pos < this->count() )
  {
    auto itr = this->items_.begin() + pos;

    this->items_.erase( itr );

    // TODO:
    this->invalidate();
  }
}

void CustomUILayout::add_item( UILayoutItem::raw_ptr item )
{
  this->items_.push_back( item );
}

} // namespace nom
