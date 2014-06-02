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
#include "nomlib/gui/ListBox.hpp"

// Private heaaders
#include "nomlib/graphics/Cursor.hpp"

namespace nom {

ListBox::ListBox(
                  UIWidget* parent,
                  int64 id,
                  const Point2i& pos,
                  const Size2i& size,
                  const UIItemContainer::raw_ptr store
                ) :
  UIWidget( parent, id, pos, size )   // Base class
{
  // NOM_LOG_TRACE( NOM );

  // Use explicitly set coordinates for our minimum widget size
  this->set_minimum_size( size );

  // Auto-generate a name tag for our widget.
  this->set_name( "listbox" );

  this->set_updated( false );

  this->set_item_store( store );

  // Default highlighted item text color.
  if( this->style() != nullptr )
  {
    this->set_selected_text_color( this->style()->font_selected_color() );
  }

  // Initialize the default event listeners for the widget.
  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

  // Widget accepts all focus types.
  //
  // TODO: Implement FinalFantasy theme style with a trigger to downgrade this
  // this focus policy.
  this->set_focus_policy( FocusPolicy::WheelFocus );

  // this->update();
}

ListBox::~ListBox( void )
{
  // NOM_LOG_TRACE( NOM );
}

ObjectTypeInfo ListBox::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

const Size2i ListBox::size_hint( void ) const
{
  // Maximum text width requirements for stored items
  // int max_text_width = 0;

  // Maximum text height requirements for stored items
  int max_text_height = 0;

  // Default point size of font
  uint point_size = nom::DEFAULT_FONT_SIZE;

  // Calculate the total text height requirements for the widget's items
  for( auto itr = this->labels_.begin(); itr != this->labels_.end(); ++itr )
  {
    Text* label = NOM_DYN_PTR_CAST( Text*, itr->get() );

    FontMetrics face = label->font()->metrics();

    // NOM_DUMP( this->name() );
    // NOM_DUMP( face.name );
    // NOM_DUMP( label->text() );

    // Use the point size of the widget's style, if one has been set:
    if( this->style() != nullptr )
    {
      point_size = this->style()->font_size();
    }

    // Text label's width, with respect to rendered font
    // max_text_width = std::max( label->width(), max_text_width );

    // Maximum pixel height of the font's glyph
    max_text_height += label->font()->newline( point_size );
  } // end for labels loop

  // NOM_DUMP( max_text_height );

  // If we have calculated a total text height requirement, we can stop here,
  // using the total text height for our preferred height field.
  if( max_text_height > 0 )
  {
    // FIXME:
    return Size2i( this->size().w, max_text_height );

    // Text label's width & height (with respect to rendered font):
    // return Size2i( max_text_width, max_text_height );
  }

  // We do not have any text labels stored, so assume a widget with a height
  // large enough for one item.
  if( this->font().valid() == true )
  {
    // NOM_DUMP( this->font()->newline( point_size ) );

    return Size2i( this->size().w, this->font()->newline( point_size ) );

    // Text label's width & height (with respect to rendered font):
    // return Size2i( max_text_width, this->font()->newline( point_size ) );
  }

  // If all else fails ... use the preset size of the widget as the preferred
  // size.
  //
  // Should we be using minimum_size here instead?
  return Size2i( this->size() );
}

bool ListBox::valid( void ) const
{
  if  (
        this->position() != Point2i::null && this->size() != Size2i::null &&
        this->store_.get() != nullptr
      )
  {
    return true;
  }

  return false;
}

UIItemContainer::raw_ptr ListBox::store( void ) const
{
  NOM_ASSERT( this->store_.get() != nullptr );

  return this->store_.get();
}

const Color4i& ListBox::selected_text_color( void ) const
{
  return this->selected_text_color_;
}

void ListBox::draw( RenderTarget& target ) const
{
  for( auto itr = this->labels_.begin(); itr != this->labels_.end(); ++itr )
  {
    (*itr)->draw( target );
  }

  // UIWidget::draw( target );
}

void ListBox::set_item_store( const UIItemContainer::raw_ptr store )
{
  this->set_updated( false );

  this->store_.reset( store );

  // FIXME: VBoxLayoutTest unit tests break if we do not update here.
  this->update();
}

void ListBox::set_selected_text_color( const Color4i& color )
{
  this->set_updated( false );

  this->selected_text_color_ = color;

  this->update();
}

int ListBox::hit_test( const Point2i& pt )
{
  // Counter for the position of each element; must start at zero and count our
  // way up to the last element to figure out where we are in the array.
  int index = 0;

  for( auto itr = this->labels_.begin(); itr != this->labels_.end(); ++itr )
  {
    if( NOM_ISA( Text*, itr->get() ) != true )
    {
      NOM_LOG_ERR( NOM, "Could not up-cast nom::IDrawable to nom::Text." );
      return npos;
    }

    Text* label = NOM_DYN_PTR_CAST( Text*, itr->get() );

    if( this->contains( label, pt ) == true )
    {
      // Found our array / element position within the internal storage
      // container!
      return index;
    }

    // Increment element position
    ++index;
  }

  // No position found
  return npos;
}

// Protected scope

void ListBox::on_key_down( const UIWidgetEvent& ev )
{
  nom::Event event = ev.event();
  UIWidgetEvent item;

  // Registered action for key press event
  if( event.type == SDL_KEYDOWN )
  {
    if( this->focused() == false )
    {
      return;
    }

    // Position of each item
    int index = this->store()->selection();

    // Send the array index in our event; this signifies which choice was
    // selected.
    item.set_index( index );

    // Set the current index position to the selected text label -- this
    // has the side effect of updating the text color; see also: ::update.
    this->store()->set_selection( index );

    // Send the text of the selection if there is a selection set. Note that
    // this needs to be set *after* the selection has been set.
    if( index != npos )
    {
      item.set_text( this->store()->item_label( index) );
    }
    else
    {
      item.set_text( "\0" );
    }

    // Set the associated nom::Event object for this UI event.
    item.set_event( event );

    item.set_id( this->id() );

    // Send the UI event object to the registered callback; this is the private
    // event interface.
    this->dispatcher()->emit( UIEvent::ON_KEY_DOWN, item );

    // Send the UI event object to the registered callback; this is the public
    // event interface.
    this->dispatcher()->emit( UIEvent::KEY_DOWN, item );
  }

  this->set_updated( false );

  if( event.type == SDL_KEYDOWN )
  {
    // Our internal storage container could also be used to obtain the
    // selection index.
    // int selected = ev.index();
    int selected = item.index();

    if( event.key.sym == SDLK_UP && selected > 0 )
    {
      --selected;

      this->store()->set_selection( selected );
      // NOM_DUMP(this->store()->selection());
    }
    else if( event.key.sym == SDLK_DOWN && ( selected < this->store()->size() - 1 ) )
    {
      ++selected;

      this->store()->set_selection( selected );
      // NOM_DUMP(this->store()->selection());
    }
    else
    {
      // NOM_DUMP( selected );
    }
  }

  // NOM_DUMP(ev.index());
  // NOM_DUMP(ev.text());

  this->update();
}

void ListBox::on_mouse_down( const UIWidgetEvent& ev )
{
  int index = 0;
  uint32 p = this->focus_policy();
  UIWidgetEvent item;
  Event evt = ev.event();

  // Registered action for mouse button event
  if( evt.type == SDL_MOUSEBUTTONDOWN )
  {
    if( p & FocusPolicy::ClickFocus )
    {
      Point2i ev_mouse( evt.mouse.x, evt.mouse.y );

      index = this->hit_test( ev_mouse );

      if( index != npos )
      {
        this->set_focused( true );
      }
    } // end if FocusPolicy::ClickFocus

    // Send the array index in our event; this signifies which choice was
    // selected.
    item.set_index( index );

    // Send the text of the selection.
    item.set_text( this->store()->item_label( index ) );

    // Set the current index position to the selected text label -- this
    // has the side effect of updating the text color; see also: ::update.
    this->store()->set_selection( index );

    // Set the associated nom::Event object for this UI event.
    item.set_event( evt );

    item.set_id( this->id() );

    // Send the UI event object to the registered callback; this is the
    // "private" interface -- reserved for internal class implementations.
    this->dispatcher()->emit( UIEvent::ON_MOUSE_DOWN, item );

    // Send the UI event object to the registered callback; this is the
    // event that gets heard by any end-user listening in, unlike the
    // private message above.
    this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );

    this->set_selected_text_color( this->selected_text_color() );
  } // end if event type == SDL_MOUSEBUTTONDOWN
}

void ListBox::on_mouse_enter( const UIWidgetEvent& ev )
{
  // this->set_focused( true );
}

void ListBox::on_mouse_wheel( const UIWidgetEvent& ev )
{
  uint32 p = this->focus_policy();
  int index = 0;
  Event event = ev.event();
  UIWidgetEvent item;

  if( event.type == SDL_MOUSEWHEEL )
  {
    if( p & FocusPolicy::WheelFocus )
    {
      MouseState mstate = Cursor::mouse_state();

      Point2i mouse( mstate.pos.x, mstate.pos.y );

      index = this->hit_test( mouse );

      if( index != nom::npos )
      {
        this->set_focused( true );
      }
      else
      {
        this->set_focused( false );
      }
    } // end if FocusPolicy::WheelFocus

    if( this->focused() == false )
    {
      return;
    }

    // Counter for the position of each element; must start at current
    // selection.
    index = this->store()->selection();

    // Send the array index in our event; this signifies which choice was
    // selected.
    item.set_index( index );

    // Set the current index position to the selected text label -- this
    // has the side effect of updating the text color; see also: ::update.
    this->store()->set_selection( index );

    // Send the text of the selection if there is a selection set. Note that
    // this needs to be set *after* the selection has been set.
    if( index != npos )
    {
      item.set_text( this->store()->item_label( index ) );
    }
    else
    {
      item.set_text( "\0" );
    }

    // Set the associated nom::Event object for this UI event.
    item.set_event( event );

    item.set_id( this->id() );

    // Send the UI event object to the registered callback; this is the private
    // event interface.
    this->dispatcher()->emit( UIEvent::ON_MOUSE_WHEEL, item );

    // Send the UI event object to the registered callback; this is the public
    // event interface.
    this->dispatcher()->emit( UIEvent::MOUSE_WHEEL, item );
  }

  this->set_updated( false );

  if( event.type == SDL_MOUSEWHEEL )
  {
    // Our internal storage container could also be used to obtain the
    // selection index.
    // int selected = ev.index();
    int selected = item.index();

    // Up
    if( event.wheel.y > 0 && selected > 0 )
    {
      --selected;
      this->store()->set_selection( selected );
      // NOM_DUMP(this->store()->selection());
    }

    // Down
    else if( event.wheel.y < 0 && selected < this->store()->size() - 1 )
    {
      ++selected;
      this->store()->set_selection( selected );
      // NOM_DUMP(this->store()->selection());
    }
  }

  // NOM_DUMP(ev.index());
  // NOM_DUMP(ev.text());

  this->update();
}

void ListBox::on_size_changed( const UIWidgetEvent& ev )
{
  Event evt = ev.event();

  if( evt.type != SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    return;
  }

  this->set_updated( false );

  if( this->decorator() )
  {
    // Update the attached decorator (border & possibly a background)
    this->decorator()->set_bounds( ev.resized_bounds_ );
  }

  // Update ourselves with the new rendering coordinates
  this->set_bounds( ev.resized_bounds_ );

  this->update();
}

void ListBox::set_focused( bool state )
{
  UIWidget::Children children = this->find_children( this );

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    // if( this->type() == (*itr)->type() && *itr != this )

    // Defocus all other ListBox widget types that do not match the
    // widget instance that we are executing from.
    if( (*itr) != this )
    {
      (*itr)->UIWidget::set_focused( false );
    }
    else  // Our widget gains the focus
    {
      UIWidget::set_focused( state );
    }
  }
}

// Private scope

void ListBox::update( void )
{
  int index = 0;    // Elapsed position counter

  Point2i pos = this->position(); // Offset position of the text label
  Text::raw_ptr label = nullptr;  // Temporary text label; created from string objects
                                  // objects.

  // UIWidget::update();

  // Conserve CPU cycles if our object is already rendered (up-to-date).
  if( this->updated() ) return;

  // FIXME:
  //
  // Sanity check; we'd crash here otherwise!
  if( this->font().valid() == false )
  {
    // NOM_LOG_ERR( NOM, "Font resource is NULL -- skipping update of object!" );
    return;
  }

  // Empty the list of previously rendered label text objects
  this->labels_.clear();

  ItemStrings labels = this->store()->item_labels();

  for( auto itr = labels.begin(); itr != labels.end(); ++itr )
  {
    label = Text::raw_ptr( new Text( *itr, this->font() ) );

    // FIXME: Automate the calculation of the text label size.
    //
    // NOTE: Variable text size is only applicable when using TrueType fonts;
    // we must make do with whatever the existing text size is when dealing with
    // Bitmap fonts.
    if( this->style() != nullptr )
    {
      label->set_text_size( this->style()->font_size() );
    }
    else
    {
      label->set_text_size( nom::DEFAULT_FONT_SIZE );
    }

    label->set_position( Point2i( pos.x + 4, pos.y ) );

    // Set the resulting label text width offset to make up for what we added
    // above, in addition to an extra four (4) pixels, just for a nice padding
    // buffer.
    //
    // Note that the label size is only used as reference for the optional
    // cropping feature in nom::Text -- Text::ExtraRenderingFeatures::Crop.
    label->set_size( Size2i( this->size().w - 8, this->size().h ) );
    // label->set_size( Size2i( label->width()+8, label->height() ) );

    label->set_features( Text::ExtraRenderingFeatures::CropText );

    if( this->style() != nullptr )
    {
      // label->set_alignment( this->style()->text_alignment() );
      label->set_alignment( Text::Alignment::TopLeft );
    }
    else
    {
      label->set_alignment( Text::Alignment::TopLeft );
    }

    // Vertical Spacing in between each text
    pos.y += label->height();

    // The element index (position) we are at is the current active selection,
    // so we visually indicate so by highlighting the text label with a color.
    if( this->store()->selection() == index )
    {
      // NOM_DUMP( index );
      label->set_color( this->selected_text_color() );
    }

    this->labels_.push_back( label );

    // TODO: Update layout geometry here?

    // EOF text label; update our elapsed position(s)
    ++index;
  }

  this->set_updated( true );
}

} // namespace nom
