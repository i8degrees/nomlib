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
#include "nomlib/gui/UILayout.hpp"

// Private forward declarations
#include "nomlib/gui/UIWidget.hpp"

namespace nom {

UILayout::UILayout( void ) :
  spacing_{ 1 },
  contraints_{ UILayout::SetDefaultConstraint },
  directions_{ Orientations::Horizontal }
{
  // NOM_LOG_TRACE( NOM );
}

UILayout::~UILayout( void )
{
  // NOM_LOG_TRACE( NOM );
}

UILayout::UILayout( UIWidget* parent ) :
  spacing_{ 1 },
  contraints_{ UILayout::SetDefaultConstraint },
  directions_{ Orientations::Horizontal }
{
  // NOM_LOG_TRACE( NOM );

  this->add_child_widget( parent );
}

enum Orientations UILayout::directions( void ) const
{
  return this->directions_;
}

IntRect UILayout::bounds( void ) const
{
  return this->bounds_;
}

void UILayout::invalidate( void )
{
  this->invalid_ = true;

  this->update();
}

bool UILayout::empty( void ) const
{
  if( this->count() == 0 )
  {
    return true;
  }

  return false;
}

UILayout::raw_ptr UILayout::layout( void )
{
  return this;
}

Size2i UILayout::maximum_size( void ) const
{
  return UILayoutItem::layout_max_;
}

Size2i UILayout::minimum_size( void ) const
{
  return Size2i( 0, 0 );
}

void UILayout::set_bounds( const IntRect& rect )
{
  this->bounds_ = rect;
}

bool UILayout::activate( void ) const
{
  NOM_STUBBED( NOM );

  return false;
}

void UILayout::add_widget( UIWidget* widget )
{
  this->add_item( new UIWidgetLayoutItem( widget ) );
}

int UILayout::find_widget( UIWidget* widget ) const
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item->widget() == widget )
    {
      return pos;
    }

    ++pos;
  }

  // Not found
  return npos;
}

bool UILayout::enabled( void ) const
{
  return this->enabled_;
}

UIWidget* UILayout::parent( void ) const
{
  UIWidget* window = nullptr;

  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    window = item->widget()->parent();

    if( item->widget() == window )
    {
// NOM_DUMP( item->widget()->name() );
// NOM_DUMP( window->name() );
      return window;
    }

    ++pos;
  }

  // NULL
  return window;
}

int UILayout::erase_item( UILayoutItem* layout )
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item == layout )
    {
      this->erase( pos );

      // Successful removal
      return pos;
    }

    ++pos;
  }

  // Nothing removed
  return npos;
}

int UILayout::erase_widget( UIWidget* widget )
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item->widget() == widget )
    {
      this->erase( pos );

      // Successful removal
      return pos;
    }

    ++pos;
  }

  // Nothing removed
  return npos;
}

void UILayout::set_alignment( enum Anchor align )
{
  UILayoutItem::set_alignment( align );
}

bool UILayout::set_alignment( UIWidget* widget, enum Anchor align )
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item == nullptr )
    {
      return false;
    }

    if( item->widget() == widget )
    {
      item->set_alignment( align );
      this->invalidate();

      return true;
    }

    ++pos;
  }

  return false;
}

bool UILayout::set_alignment( const UILayout::raw_ptr layout, enum Anchor align )
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item->layout() == layout )
    {
      item->set_alignment( align );
      this->invalidate();

      return true;
    }

    ++pos;
  }

  return false;
}

void UILayout::set_margins( const IntRect& margins )
{
  this->margins_ = margins;
}

void UILayout::set_enabled( bool state )
{
  this->enabled_ = state;
}

void UILayout::set_size_constraint( enum SizeConstraint size )
{
  this->contraints_ = size;
}

void UILayout::set_spacing( int space )
{
  this->spacing_ = space;
}

enum UILayout::SizeConstraint UILayout::size_constraint( void ) const
{
  return this->contraints_;
}

int UILayout::spacing( void ) const
{
  return this->spacing_;
}

void UILayout::update( void )
{
  // NOM_STUBBED( NOM );
}

// Protected scope

void UILayout::add_child_widget( UIWidget* widget )
{
  widget->set_layout( this );
}

} // namespace nom
