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
#include "nomlib/gui/QuestionDialogBox.hpp"

namespace nom {

QuestionDialogBox::QuestionDialogBox( void ) :
  selection_{ -1 }
{
  // NOM_LOG_TRACE( NOM );

  this->set_updated( false );

  this->update();
}

QuestionDialogBox::~QuestionDialogBox( void )
{
  // NOM_LOG_TRACE( NOM );
}

QuestionDialogBox::self_type& QuestionDialogBox::operator =( const self_type& rhs )
{
  // this->choices_ = rhs.choices_;
  this->selection_ = rhs.selection_;

  return *this;
}

QuestionDialogBox::QuestionDialogBox  (
                                        UIWidget* parent,
                                        int64 id,
                                        const Point2i& pos,
                                        const Size2i& size
                                      ) :
  MessageBox( parent, id, pos, size ),
  selection_{ -1 }
{
  // NOM_LOG_TRACE( NOM );

  this->set_updated( false );

  // Auto-generate a name tag for our widget.
  this->set_name( "question_dialog_box" );

  this->update();
}

// IDrawable::raw_ptr QuestionDialogBox::clone( void ) const
// {
//   return QuestionDialogBox::raw_ptr( new QuestionDialogBox( *this ) );
// }

void QuestionDialogBox::draw( RenderTarget& target ) const
{
  // if ( this->enabled() == false ) return;

  // First, render our base class
  MessageBox::draw( target );

  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    (*itr)->draw( target );
  }
}

void QuestionDialogBox::append_choice( const Text& label )
{
  this->set_updated( false );

  Text::unique_ptr choice = Text::unique_ptr( new Text( label ) );

  this->choices_.push_back( std::move( choice ) );
  // this->set_shape( std::move( choice ) );

  this->update();
}

// Protected scope

void QuestionDialogBox::update( void )
{
  // Elapsed element position counter
  int index = 0;

  // Point2i pos( this->position().x, this->message_bounds().y - this->message_bounds().h );
  Point2i pos( this->position().x, this->position().y );

  if ( this->updated() == true ) return;

  // First, update our base class
  MessageBox::update();

  // FIXME:
  //
  // Empty the rendered drawables list before we attempt to update.
  // this->choices_.clear();

  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    (*itr)->set_position( pos );
    (*itr)->set_size( Size2i( this->size() ) );

    pos.x = pos.x;
    pos.y += (*itr)->height();

// NOM_DUMP( pos.x );
// NOM_DUMP( pos.y );
// NOM_DUMP( (*itr)->alignment() );
// NOM_DUMP( this->size() );

    (*itr)->set_alignment( (*itr)->alignment() );

    // this->set_shape( std::move( *itr ) );

    // Make the current item label selection stand out by marking it a different
    // color.
    if( this->selection() == index )
    {
      // FIXME: Color is a stub.
      (*itr)->set_color( Color4i::Red );
    }

    // ...Next element position!
    ++index;
  }

  // FIXME: The setting of the text label's color breaks if we set allow
  // updated to be set to true below.
  // this->set_updated( true );
}

bool QuestionDialogBox::process_event( const nom::Event& ev )
{
  Point2i mouse_coords( ev.mouse.x, ev.mouse.y );

  // Base class
  MessageBox::process_event( ev );

  // FIXME (?):
  //
  // if( MessageBox::process_event( ev ) ) return true;

  // Registered action for single mouse click event
  if( ev.type == SDL_MOUSEBUTTONDOWN )
  {
    // Counter for the position of each element; must start at zero.
    int index = 0;

    for ( auto it = this->choices_.begin(); it != this->choices_.end(); ++it )
    {
      IntRect label_bounds( (*it)->global_bounds() );

      if ( label_bounds.contains( mouse_coords ) )
      {
        UIWidgetEvent item;

        // Send the array index in our event; this signifies which choice was
        // selected.
        item.set_index( index );

        // Send the text of the selection.
        item.set_text( (*it)->text() );

        // Set the associated nom::Event object for this UI event.
        item.set_event( ev );

        // Associate the widget's unique identifier for this widget's event
        // object.
        item.set_id( this->id() );

        // Single mouse click event
        if( ev.mouse.clicks < 2 )
        {
          // Send the UI event object to the registered callback; public event
          // slot.
          this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );
        }
        else // Double mouse click event
        {
          // Send the UI event object to the registered callback; public event
          // slot.
          this->dispatcher()->emit( UIEvent::MOUSE_DCLICK, item );
        }

        // Processed events.
        return true;
      }

      // ...next element position!
      ++index;

    } // end if text label bounds intersect mouse coordinates
  } // end if ev.type == SDL_MOUSEBUTTONDOWN

  // Registered action for wheel events
  else if( ev.type == SDL_MOUSEWHEEL )
  {
    int index = this->selection();

    for( auto it = this->choices_.begin(); it != this->choices_.end(); ++it )
    {
      // index = this->selection();

      // Ensure that we are not touching an invalid (negative) value.
      // if( index < 0 ) index = 0;

      UIWidgetEvent item;

      // Send the array index in our event; this signifies which choice was
      // selected.
      item.set_index( index );

      // Set the current index position to the selected text label -- this
      // has the side effect of updating the text color; see also: ::update.
      this->set_selection( index );

      // Send the text of the selection; this needs to be set *after* the
      // selection has been set.
      item.set_text( this->choices_.at( index )->text() );

      // Set the associated nom::Event object for this UI event.
      item.set_event( ev );

      // Associate the widget's unique identifier for this widget's event
      // object.
      item.set_id( this->id() );

      // Send the UI event object to the registered callback; public event slot.
      this->dispatcher()->emit( UIEvent::MOUSE_WHEEL, item );

      // Processed events
      return true;
    }
  }

  // Registered action for key press event
  else if( ev.type == SDL_KEYDOWN )
  {
    // Position of each item
    int index = this->selection();

    // Ensure that we are not touching an invalid (negative) value.
    // if( index < 0 ) index = 0;

    for( auto it = this->choices_.begin(); it != this->choices_.end(); ++it )
    {
      UIWidgetEvent item;

      // Send the array index in our event; this signifies which choice was
      // selected.
      item.set_index( index );

      // Set the current index position to the selected text label -- this
      // has the side effect of updating the text color; see also: ::update.
      this->set_selection( index );

      // Send the text of the selection; this needs to be set *after* the
      // selection has been set.
      item.set_text( this->choices_[index]->text() );

      // Set the associated nom::Event object for this UI event.
      item.set_event( ev );

      // Associate the widget's unique identifier for this widget's event
      // object.
      item.set_id( this->id() );

      // Send the UI event object to the registered callback; public event slot.
      this->dispatcher()->emit( UIEvent::KEY_DOWN, item );

      // Processed events
      return true;

    } // end for labels_ loop

  } // end if event.type == SDL_KEYDOWN

  // No processed events.
  return false;
}

int QuestionDialogBox::selection( void ) const
{
  return this->selection_;
}

uint QuestionDialogBox::items_size( void ) const
{
  return this->choices_.size();
}

QuestionDialogBox::ItemStrings QuestionDialogBox::choices( void ) const
{
  ItemStrings label_texts;

  for( auto itr = this->choices_.begin(); itr != this->choices_.end(); ++itr )
  {
    label_texts.push_back( (*itr)->text() );
  }

  return label_texts;
}

void QuestionDialogBox::set_selection( int pos )
{
  this->selection_ = pos;

  this->update();
}

} // namespace nom
