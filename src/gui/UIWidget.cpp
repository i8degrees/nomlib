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
#include "nomlib/gui/UIWidget.hpp"

// Private headers
#include "nomlib/system/PlatformSettings.hpp"

// Forward declarations
#include "nomlib/graphics/shapes/Rectangle.hpp" // tool tip
#include "nomlib/graphics/Text.hpp"
#include "nomlib/system/Event.hpp"

#include "nomlib/gui/IDecorator.hpp"
#include "nomlib/gui/UILayout.hpp"
#include "nomlib/gui/UIStyle.hpp"
#include "nomlib/gui/UIEventDispatcher.hpp"
#include "nomlib/gui/UIEvent.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"

// #define NOM_DEBUG_OUTPUT_LAYOUT_DATA

namespace nom {

// Static initializations
int64 UIWidget::next_window_id_ = 0;

UIWidget::UIWidget( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_GUI );

  this->initialize( nullptr, nom::AUTO_ID, Point2i::null, Size2i::null, typeid( this ).name() );
}

UIWidget::~UIWidget( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_GUI );
}

void UIWidget::initialize (
                            UIWidget* parent,
                            int64 id,
                            const Point2i& pos,
                            const Size2i& size,
                            const std::string& name
                          )
{

  if( this->dispatcher() == nullptr )
  {
    this->set_event_dispatcher( new UIEventDispatcher() );
  }

  // Sane defaults
  this->set_focused( false );
  this->set_visibility( true );
  this->set_updated( false );

  // Initialize widget's default size policy (for use with layouts):
  this->set_size_policy( UILayoutPolicy::Policy::Preferred );

  // Initialize widget's default focus policy:
  this->set_focus_policy( FocusPolicy::NoFocus );

  // Top-level widget (parent).
  this->set_position( pos );
  this->set_size( size );

  // Generate a uniquely identifying integer when requested, otherwise honor
  // the number given, even if it may be a duplicate.
  if( id == nom::AUTO_ID )
  {
    this->set_id( this->generate_id() );
  }
  else
  {
    this->set_id( id );
  }

  // Use the parent object's existing state for the child widget.
  if( parent != nullptr )
  {
    // Calculate our position relative to the top-level widget (window).
    Point2i offset = pos + parent->position();

    IntRect parent_bounds( parent->global_bounds() );

    // Handle widget bounds that are outside the top-level widget's boundaries.
    if( parent_bounds.contains( offset ) == false )
    {
      {
        // FIXME: Temporarily commented to disable debugging output
        // NOM_DUMP( parent->name() );
        // NOM_DUMP( parent->position() );
        // NOM_DUMP( parent->size() );

        // NOM_DUMP( pos );
        // NOM_DUMP( offset );
        // NOM_DUMP( size.w );
      }

      {
        // FIXME: Not sure how we ought to handle this case yet?
        // this->set_position( Point2i::null );
        // this->set_visibility( false );
      }
    }
    else
    {
      this->set_position( offset );
    }

    // Top-level window (owner).
    this->set_parent( parent );
    this->set_font( parent->font() );
  }

  // Auto-generate our name tag if it is empty.
  if( name.empty() )
  {
    this->set_name( "Window" );
  }
  else
  {
    this->set_name( name );
  }

  // Ensure that a valid font resource is available.
  if( this->font().valid() == false )
  {
    this->set_font( PlatformSettings::get_system_font( SystemFontType::VariableTrueType ) );
  }

  // Initialize the default event listener implementations for the widget
  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );
  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WIDGET_UPDATE, this->on_update );
}

UIWidget::UIWidget  (
                      const Point2i& pos,
                      const Size2i& size
                    )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_GUI );

  this->initialize( nullptr, nom::AUTO_ID, pos, size, "\0" );
}

UIWidget::UIWidget  (
                      UIWidget* parent,
                      int64 id,
                      const Point2i& pos,
                      const Size2i& size
                    )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_GUI );

  this->initialize( parent, id, pos, size, "\0" );
}

UIWidget::self_type& UIWidget::operator =( const self_type& rhs )
{
  this->swap( &rhs );

  return *this;
}

UIWidget::UIWidget( const self_type* rhs )
{
  this->initialize( rhs->parent(), -1, rhs->position(), rhs->size(), "\0" );

  if( rhs->is_window() == true )
  {
    this->set_parent_bounds( rhs->global_bounds() );
  }
}

ObjectTypeInfo UIWidget::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

bool UIWidget::valid( void ) const
{
  if( this->dispatcher() != nullptr && this->position() != Point2i::null && this->size() != Size2i::null )
  {
    return true;
  }

  return false;
}

const Font& UIWidget::font( void ) const
{
  return this->font_;
}

uint32 UIWidget::id( void ) const
{
  return this->id_;
}

const std::string& UIWidget::name( void ) const
{
  return this->name_;
}

bool UIWidget::focused( void ) const
{
  return this->focused_;
}

uint32 UIWidget::focus_policy( void ) const
{
  return this->focus_policy_;
}

bool UIWidget::parent_has_focus( void ) const
{
  if( this->parent()->focused() == true )
  {
    return true;
  }

  return false;
}

UIWidget::raw_ptr UIWidget::parent( void ) const
{
  return this->parent_.get();
}

const UIWidget::Children& UIWidget::children( void ) const
{
  return this->children_;
}

std::shared_ptr<IDecorator> UIWidget::decorator( void ) const
{
  return this->decorator_;
}

const std::string UIWidget::title( void ) const
{
  if( this->title_ != nullptr )
  {
    return this->title_->text();
  }

  return "\0";
}

const UILayoutPolicy& UIWidget::size_policy( void ) const
{
  return this->policy_;
}

bool UIWidget::contains( IDrawable* obj, const Point2i& pt ) const
{
  IntRect bounds;

  if( NOM_ISA( Transformable*, obj ) != true )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not perform hit test: object does not derive from Transformable." );
    return false;
  }

  Transformable* object = NOM_DYN_PTR_CAST( Transformable*, obj );

  // Rendering coordinate bounds; we'll need both position & size for our
  // hit test.
  bounds = IntRect( object->position(), object->size() );

  if( object->position() == Point2i::null )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Object's position is NULL; hit test is likely to fail." );
  }

  if( object->size() == Size2i::null )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Object's size is NULL; hit test is likely to fail." );
  }

  if( bounds.contains( pt ) )
  {
    // The bounds of the visual object intersect with the point
    return true;
  }

  // No matches
  return false;
}

bool UIWidget::contains_label( Text* obj, const Point2i& pt )
{
  IntRect bounds;

  if( NOM_ISA( Text*, obj ) != true )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not perform hit test: object does not derive from Text." );
    return false;
  }

  Text* object = NOM_DYN_PTR_CAST( Text*, obj );

  // FIXME: Rendering coordinate bounds; we'll need both position & size for
  // our hit test. Note that Text::global_bounds is a bit different than the
  // usual expectations here -- width and height are in "font units", and must
  // be referred to using width & height functions (instead of the usual size
  // method call).
  bounds = IntRect( object->global_bounds() );

  if( object->position() == Point2i::null )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Object's position is NULL; hit test is likely to fail." );
  }

  if( object->size() == Size2i::null )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Object's size is NULL; hit test is likely to fail." );
  }

  if( bounds.contains( pt ) )
  {
    // The bounds of the visual object intersect with the point
    return true;
  }

  // No matches
  return false;
}

bool UIWidget::is_window( void ) const
{
  if( this->parent_ == nullptr )
  {
    return true;
  }

  return false;
}

bool UIWidget::visible( void ) const
{
  return this->visible_;
}

const Size2i UIWidget::maximum_size( void ) const
{
  if( this->max_size_ != Size2i::null )
  {
    return this->max_size_;

  }

  return Size2i( nom::WIDGET_MAX_SIZE, nom::WIDGET_MAX_SIZE );
}

const Size2i UIWidget::minimum_size( void ) const
{
  if( this->min_size_ != Size2i::null )
  {
    return this->min_size_;
  }

  return Size2i( 0, 0 );
}

const Size2i UIWidget::size_hint( void ) const
{
  if( this->layout() == nullptr )
  {
    return Size2i( 0,0 );
  }
  else // if( this->layout() != nullptr )
  {
    return this->layout()->size_hint();
  }
}

// const IntRect UIWidget::local_bounds( void ) const
// {
//   if( this->parent() != nullptr )
//   {
//     // Translate the widget's coordinates from global (screen) coordinates to
//     // relative.
//     return IntRect( this->position() - this->parent()->position(), this->size() );
//   }

//   return IntRect( this->position(), this->size() );
// }

const IntRect UIWidget::global_bounds( void ) const
{
  // return this->bounds_;
  return IntRect( this->position(), this->size() );
}

UILayout* UIWidget::layout( void ) const
{
  return this->layout_.get();
}

std::shared_ptr<UIStyle> UIWidget::style( void ) const
{
  return this->style_;
}

const IntRect& UIWidget::parent_bounds( void ) const
{
  return this->parent_bounds_;
}

void UIWidget::set_font( const Font& font )
{
  UIWidget::Children children = this->children();

  if( children.size() > 0 )
  {
    for( auto itr = children.begin(); itr != children.end(); ++itr )
    {
      // NOM_DUMP( (*itr)->name() );
      (*itr)->set_font( font );

      // FIXME: This method call yields nothing on widgets that require updated_
      // to be false.
      (*itr)->update();
    }
  }

  // Now that our children are fed, take care of ourselves, the top-level parent
  // AKA widget / window.
  if( font != this->font() )
  {
    this->font_ = font;
  }

  // Associate the widget's unique identifiers with the event notification.
  UIWidgetEvent evt;
  evt.set_index( this->id() );
  evt.set_text( this->name() );
  evt.set_id( this->id() );

  Event ev;
  ev.type = UIEvent::ON_WIDGET_UPDATE;
  ev.timestamp = nom::ticks();
  evt.set_event( ev );

  // Private event notification
  this->dispatcher()->emit( UIEvent::ON_WIDGET_UPDATE, evt );
}

void UIWidget::set_font( const Font* font )
{
  this->set_font( *font );
}

/*
void UIWidget::set_font( const IFont* font, bool inherit )
{
  // Children of this top-level window should inherit the font
  if( inherit == true )
  {
    UIWidget::Children children = this->children();

    if( children.size() > 0 )
    {
      for( auto itr = children.begin(); itr != children.end(); ++itr )
      {
        // NOM_DUMP( (*itr)->name() );

        // Skip the setting of the font if the child widget already has a copy
        if( (*itr)->font().get() != font )
        {
          (*itr)->set_font( font->clone() );

          // FIXME: This method call yields nothing on widgets that require updated_
          // to be false.
          (*itr)->update();
        }
      }
    }

    // Now that are children are fed, take care of us (the top-level window).

    // Skip the setting of the font if we already have a copy
    if( this->font().get() != font )
    {
      this->font_ = font->clone();
      this->update();
    }
  }

  // Do *not* allow children to inherit the font

  else  // inherit == false
  {
    // Skip the setting of the font if we already have a copy
    if( this->font().get() != font )
    {
      this->font_ = font->clone();
      this->update();
    }
  }
}
*/

void UIWidget::set_id( uint32 id )
{
  this->id_ = id;
}

void UIWidget::set_name( const std::string& name )
{
  this->name_ = name;
}

void UIWidget::set_focused( bool state )
{
  this->focused_ = state;
}

void UIWidget::set_focus_policy( uint32 flags )
{
  this->focus_policy_ = flags;
}

void UIWidget::set_parent( const UIWidget::raw_ptr parent )
{
  if( parent != nullptr )
  {
    this->parent_.reset( parent );

    // this->set_updated( false );
    // this->update();
  }
}

void UIWidget::set_children( const UIWidget::Children& children )
{
  this->children_ = children;

  // this->set_updated( false );
  // this->update();
}

void UIWidget::set_decorator( IDecorator* object )
{
  this->decorator_.reset( object );

  if( this->decorator_ != nullptr )
  {
    // Initialize the decorator object with safe
    this->decorator_->set_bounds( this->global_bounds() );

    // this->set_updated( false );
    this->update();
  }
}

void UIWidget::set_title( const std::string& title )
{
  if( this->font().valid() == false )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not set window title: font is NULL." );
    return;
  }

  // this->set_updated( false );

  this->title_.reset( new Text( title, this->font() ) );

  this->title_->set_position( this->position() );

  // This positions the title text of the message box on top of the second "top"
  // bordering color of nom::GrayFrame, commented as "top1".
  //
  // The original coords.x value was + 8, but I think + 4 looks best.
  // this->title_->set_position( Point2i ( this->position().x + 4, this->position().y - ( this->title_->height() / 2 ) ) );
  this->title_->set_position( Point2i ( this->position().x + 4, this->position().y ) );
  this->title_->set_size( this->size() );

  // FIXME:
  //
  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->title_->set_alignment( Text::Alignment::TopLeft );

  // Prevent rendering of text that is longer in length than its container's
  // set size parameters (see above).
  this->title_->set_features( Text::ExtraRenderingFeatures::CropText );

  // this->set_updated( false );
  // this->update();
}

void UIWidget::set_title( void )
{
  this->set_title( this->name() );
}

void UIWidget::set_size_policy( uint32 horiz, uint32 vert )
{
  this->policy_.set_horizontal_policy( horiz );
  this->policy_.set_vertical_policy( vert );
}

void UIWidget::set_size_policy( uint32 flags )
{
  this->policy_.set_horizontal_policy( flags );
  this->policy_.set_vertical_policy( flags );
}

void UIWidget::set_style( const std::shared_ptr<UIStyle> style )
{
  this->style_ = style;
}

void UIWidget::insert_child( const UIWidget::raw_ptr child )
{
  // NOM_DUMP( this->name() );
  // NOM_DUMP( child->id() );
  // NOM_DUMP( child->name() );

  if( child != nullptr )
  {
    this->children_.push_back( child );
  }
}

UIWidget::raw_ptr UIWidget::find_child( int64 id )
{
  UIWidget::Children children;

  children = this->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( (*itr)->id() == id )
    {
      return *itr;
    }
  }

  return nullptr;
}

UIWidget::raw_ptr UIWidget::find_child( const std::string& name )
{
  UIWidget::Children children;

  if( name.empty() )
  {
    return nullptr;
  }

  children = this->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( (*itr)->name() == name )
    {
      return *itr;
    }
  }

  return nullptr;
}

UIWidget::raw_ptr UIWidget::find_child( const UIWidget::raw_ptr widget ) const
{
  UIWidget::Children children = widget->parent()->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( widget->type() == (*itr)->type() )
    {
      return *itr;
    }
  }

  return nullptr;
}

UIWidget::Children UIWidget::find_children( const UIWidget::raw_ptr widget ) const
{
  UIWidget::Children widgets;
  UIWidget::Children children = widget->parent()->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( widget->type() == (*itr)->type() )
    {
      widgets.push_back( *itr );
    }
  }

  return widgets;
}

bool UIWidget::erase_child( int64 id )
{
  UIWidget::Children children = this->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( (*itr)->id() == id )
    {
      this->children_.erase( itr );

      return true;
    }
  }

  return false;
}

bool UIWidget::erase_child( const std::string& name )
{
  UIWidget::Children children = this->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( (*itr)->name() == name )
    {
      this->children_.erase( itr );

      return true;
    }
  }

  return false;
}

void UIWidget::erase_children( int64 id )
{
  UIWidget::Children children = this->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( (*itr)->id() == id )
    {
      this->children_.erase( itr );
    }
  }
}

void UIWidget::erase_children( const std::string& name )
{
  UIWidget::Children children = this->children();

  for( auto itr = children.begin(); itr != children.end(); ++itr )
  {
    if( (*itr)->name() == name )
    {
      this->children_.erase( itr );
    }
  }
}

void UIWidget::clear_children( void )
{
  this->children_.clear();
}

void UIWidget::update( void )
{
  // if( this->updated() == true )
  // {
  //   return;
  // }

  // Parent window
  if( this->decorator_ != nullptr )
  {
    this->decorator()->update();
  }

  if( this->title_ != nullptr )
  {
    // this->title_->update();
  }

  Children children = this->children();

  // Widgets
  for( auto it = children.begin(); it != children.end(); ++it )
  {
    if( *it != nullptr )
    {
      if( (*it)->decorator() != nullptr )
      {
        (*it)->decorator()->update();
      }

      // (*it)->update();
    }
  }

  if( this->tip_ == false )
  {
    this->tooltip_.clear();
  }

  for( auto itr = this->tooltip_.begin(); itr != this->tooltip_.end(); ++itr )
  {
    (*itr)->update();
  }

  // this->set_updated( true );
}

void UIWidget::draw( RenderTarget& target ) const
{
  // Parent window
  if( this->decorator_ != nullptr )
  {
    this->decorator()->draw( target );
  }

  if( this->title_ != nullptr )
  {
    this->title_->draw( target );
  }

  // Widgets
  for ( auto it = this->children_.begin(); it != this->children_.end(); ++it )
  {
    if( *it != nullptr )
    {
      // Do not render widgets that are not visible
      if( (*it)->visible() == false )
      {
        continue;
      }

      // Render the widget's border & background
      if( (*it)->decorator() != nullptr )
      {
        (*it)->decorator()->draw( target );
      }

      // Let the widget manage rendering of itself
      (*it)->draw( target );
    }
  }

  for( auto itr = this->tooltip_.begin(); itr != this->tooltip_.end(); ++itr )
  {
    (*itr)->draw( target );
  }
}

bool UIWidget::process_event( const Event& ev )
{
  // Widgets
  for( auto it = this->children_.begin(); it != this->children_.end(); ++it )
  {
    if( *it != nullptr )
    {
      // Sanity check; nothing to do if callback table is empty.
      if( (*it)->dispatcher()->size() < 1 )
      {
        // NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Callback table is empty; skipping ::process_event." );
        return false;
      }

      // return (*it)->process_event( ev );

      IntRect widget_bounds( (*it)->position(), (*it)->size() );
      Point2i mouse_coords( ev.mouse.x, ev.mouse.y );

      if( ev.type == SDL_MOUSEMOTION )
      {
        Point2i ev_mouse( ev.motion.x, ev.motion.y );

        if( this->contains( *it, ev_mouse ) == true )
        {
          // NOM_DUMP( (*it)->name() );
          // NOM_DUMP("motion");
          (*it)->on_mouse_enter( new UIWidgetEvent( ev ) );

          // return true;
        }
        else  // Widget bounds do not intersect mouse coordinates
        {
          (*it)->on_mouse_leave( new UIWidgetEvent( ev ) );

          // return true;
        }
      }
      else if( ev.type == SDL_MOUSEBUTTONDOWN )
      {
        if( widget_bounds.contains( mouse_coords ) )
        {
          (*it)->on_mouse_down( new UIWidgetEvent( ev ) );

          return true;
        }
      }
      else if( ev.type == SDL_MOUSEBUTTONUP )
      {

        if( widget_bounds.contains( mouse_coords ) )
        {
          (*it)->on_mouse_up( new UIWidgetEvent( ev ) );

          return true;
        }
      }
      else if( ev.type == SDL_MOUSEWHEEL )
      {
        (*it)->on_mouse_wheel( new UIWidgetEvent( ev ) );
      }
      else if( ev.type == SDL_KEYDOWN )
      {
        (*it)->on_key_down( new UIWidgetEvent( ev ) );
      }
      else if( ev.type == SDL_KEYUP )
      {
        (*it)->on_key_up( new UIWidgetEvent( ev ) );
      }
    }
  }

  // Sanity check; nothing to do if callback table is empty.
  if( this->dispatcher()->size() < 1 )
  {
    // NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Callback table is empty; skipping ::process_event." );
    return false;
  }
/*
  else if( ev.type == SDL_MOUSEMOTION )
  {
    Point2i ev_mouse( ev.motion.x, ev.motion.y );
    IntRect window( this->global_bounds() );

    SDL_GetMouseState( &l_x, &l_y );
    if( window.contains( ev_mouse ) )
    {
      if( ev.timestamp > l_time + 2000 )
      {
        if( ev.motion.x == l_x && ev.motion.y == l_y )
        {
          Text::raw_ptr tip = new Text( "boobies!", this->font().get() );
          tip->set_position( this->position() );
          tip->set_size( tip->size().w, tip->size().h );
          // tip->set_alignment( Text::Alignment::MiddleCenter );
          tip->set_color( Color4i::Black );
          Rectangle::raw_ptr background = new Rectangle( IntRect( Point2i( this->position().x, this->position().y ), Size2i( tip->width(), tip->height() ) ), Color4i::ToolTip );

          this->tooltip_.push_back( background );
          this->tooltip_.push_back( tip );
          this->tip_ = true;
        }

        l_time = SDL_GetTicks();

        // return true;
      }
      else
      {
        this->tip_ = false;
        l_time = 0;
      }
    }
    else
    {
      this->tip_ = false;
      l_time = 0;
    }

    // return true;
  } // end if SDL_MOUSEMOTION

  // l_time = SDL_GetTicks();
  // l_x = ev.motion.x;
  // l_y = ev.motion.y;
*/
  // else if( ev.type == SDL_MOUSEBUTTONDOWN )
  // {
  //   Point2i ev_mouse( ev.mouse.x, ev.mouse.y );

  //   if( this->contains( this, ev_mouse ) == true )
  //   {
  //     // Associate the Event object along with the window object's instance
  //     // identifiers.
  //     UIWidgetEvent evt( ev.mouse.window_id, this->name(), ev, this->id() );

  //     // Send a public UI event object to the registered callback; public event
  //     // slot.
  //     this->dispatcher()->emit( UIEvent::WINDOW_MOUSE_DOWN, evt );

  //     // Successfully processed events
  //     return true;

  //   } // end if window contains mouse coords
  // } // end if SDL_MOUSEBUTTONDOWN

  // No events to process
  return false;
}

bool UIWidget::updated( void ) const
{
  return this->updated_;
}

void UIWidget::set_updated( bool state )
{
  this->updated_ = state;
}

void UIWidget::set_visibility( bool state )
{
  this->visible_ = state;
}

void UIWidget::set_maximum_size( const Size2i& size )
{
  this->max_size_ = size;
}

void UIWidget::set_minimum_size( const Size2i& size )
{
  this->min_size_ = size;
}

void UIWidget::set_size_hint( const Size2i& size )
{
  this->size_hint_ = size;
}

// void UIWidget::set_global_bounds( const Point2i& pos, const Size2i& size )
// {
//   this->bounds_ = IntRect( pos, size );
// }

// void UIWidget::set_global_bounds( const IntRect& bounds )
// {
//   this->bounds_ = bounds;
// }

void UIWidget::set_geometry( const IntRect& bounds )
{
  this->set_position( bounds.position() + this->position() );
  this->set_size( bounds.size() );
}

void UIWidget::set_geometry( int x, int y, int w, int h )
{
  this->set_geometry( IntRect( x, y, w, h ) );
}

void UIWidget::set_layout( UILayout* layout )
{
  #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
    NOM_DUMP( this->name() );
  #endif

  this->layout_.reset( layout );

  // TODO: Bounds checking, NULL pointer checks

  if( this->parent() == nullptr ) // We are the top-level widget (window)
  {
    #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
      NOM_DUMP( this->layout_->bounds().position() );
      NOM_DUMP( this->position() );
    #endif

    // this->layout_->set_bounds( IntRect( this->layout_->bounds().position() + this->position(), this->size() ) );

    #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
      NOM_DUMP( this->layout_->bounds().size() );
      NOM_DUMP( this->size() );
    #endif
  }

  // Not sure if we still need to set anything here
  else
  {
    #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
      NOM_DUMP( this->layout_->bounds().position() );
      NOM_DUMP( this->parent()->position() );
    #endif

    // this->layout_->set_bounds( IntRect( this->layout_->bounds().position() + this->parent()->position(), this->size() ) );
    #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
      NOM_DUMP( this->layout_->bounds().size() );
      NOM_DUMP( this->parent()->size() );
    #endif
  }
  #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
    NOM_DUMP( this->name() );
    NOM_DUMP( this->position() );
    NOM_DUMP( this->size() );
  #endif

  // Associate the widget's unique identifiers with the sent event.
  // UIWidgetEvent evt;
  // evt.set_index( this->id() );
  // evt.set_text( this->name() );
  // evt.set_id( this->id() );

  // Event ev;
  // ev.type = SDL_WINDOWEVENT_SIZE_CHANGED;
  // ev.timestamp = ticks();
  // ev.window.event = SDL_WINDOWEVENT_SIZE_CHANGED;
  // ev.window.data1 = this->size().w;
  // ev.window.data2 = this->size().h;
  // ev.window.window_id = this->id();
  // evt.set_event( ev );

  // this->emit( UIEvent::ON_WINDOW_SIZE_CHANGED, evt );

  // this->set_updated( false );
  this->update();
}

void UIWidget::resize( const Size2i& size )
{
  this->set_size( size );

  // Associate the widget's unique identifiers with the sent event.
  // UIWidgetEvent evt;
  UIWidgetResizeEvent evt;
  // evt.set_index( this->id() );
  // evt.set_text( this->name() );
  evt.set_id( this->id() );
  evt.set_bounds( IntRect( this->position(), Size2i( size.w, size.h ) ) );
  // NOM_DUMP(evt.bounds() );

  Event ev;
  ev.type = SDL_WINDOWEVENT_SIZE_CHANGED;
  ev.timestamp = ticks();
  ev.window.event = SDL_WINDOWEVENT_SIZE_CHANGED;
  ev.window.data1 = size.w;
  ev.window.data2 = size.h;
  ev.window.window_id = this->id();
  evt.set_event( ev );

  this->dispatcher()->emit( UIEvent::ON_WINDOW_SIZE_CHANGED, evt );
  this->dispatcher()->emit( UIEvent::WINDOW_SIZE_CHANGED, evt );

  // this->update();
}

IUIEventDispatcher* UIWidget::dispatcher( void ) const
{
  // Sanity check; ensure that we are not an invalid object state (NULL).
  // NOM_ASSERT( this->dispatcher_.get() != nullptr );

  return this->dispatcher_.get();
}

// Protected scope

void UIWidget::on_update( UIEvent* ev )
{
  NOM_LOG_TRACE( NOM );

  NOM_ASSERT( ev != nullptr );
  UIWidgetEvent* event = NOM_DYN_PTR_CAST( UIWidgetEvent*, ev );
  NOM_ASSERT( event != nullptr );

  Event evt = event->event();

  if( evt.type != UIEvent::ON_WIDGET_UPDATE )
  {
    return;
  }

  // Required for the ListBox widget
  this->set_updated( false );

  this->update();
}

void UIWidget::on_size_changed( UIEvent* ev )
{
  NOM_ASSERT( ev != nullptr );
  UIWidgetResizeEvent* event = NOM_DYN_PTR_CAST( UIWidgetResizeEvent*, ev );
  NOM_ASSERT( event != nullptr );

  Event evt = event->event();

  // FIXME: UIEvent::ON_WINDOW_SIZE_CHANGED ..? (YES)
  if( evt.type != SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    return;
  }

  this->set_updated( false );

  if( this->decorator() != nullptr )
  {
    // Update the attached decorator (border & possibly a background)
    this->decorator()->set_bounds( event->bounds() );
  }

  // Update ourselves with the new rendering coordinates
  this->set_bounds( event->bounds() );

  UIWidgetEvent info;
  info.set_id( this->id() );
  this->dispatcher()->emit( UIEvent::ON_WIDGET_UPDATE, info );

  // this->update();
}

void UIWidget::on_mouse_down( UIEvent* ev )
{
  // Do nothing
}

void UIWidget::on_mouse_up( UIEvent* ev )
{
  // Do nothing
}

void UIWidget::on_mouse_enter( UIEvent* ev )
{
  // Do nothing
}

void UIWidget::on_mouse_leave( UIEvent* ev )
{
  // Do nothing
}

void UIWidget::on_mouse_wheel( UIEvent* ev )
{
  // Do nothing
}

void UIWidget::on_key_down( UIEvent* ev )
{
  // Do nothing
}

void UIWidget::on_key_up( UIEvent* ev )
{
  // Do nothing
}

bool UIWidget::focus_previous_child( void )
{
  NOM_STUBBED( NOM );

  return false;
}

bool UIWidget::focus_next_child( void )
{
  NOM_STUBBED( NOM );

  return false;
}

void UIWidget::swap( const self_type* rhs )
{
  this->set_position( rhs->position() );
  this->set_size( rhs->size() );

  this->set_id( rhs->id() );
  this->set_name( rhs->name() );
  this->set_font( rhs->font() );
  this->set_parent( rhs->parent() );
  this->set_children( rhs->children() );
  this->set_focused( rhs->focused() );
  this->set_visibility( rhs->visible() );
  this->set_updated( rhs->updated() );
  this->decorator_ = rhs->decorator();
  this->set_title( rhs->title() );
  this->set_maximum_size( rhs->maximum_size() );
  this->set_minimum_size( rhs->minimum_size() );
  this->set_size_hint( rhs->size_hint() );
  this->set_layout( rhs->layout() );
  this->set_focus_policy( rhs->focus_policy() );
  this->policy_ = rhs->policy_;
  this->set_style( rhs->style() );
  this->set_parent_bounds( rhs->parent_bounds() );
  this->set_event_dispatcher( rhs->dispatcher() );
}

// Private scope

int64 UIWidget::generate_id( void ) const
{
  return ( ++next_window_id_ );
}

void UIWidget::set_parent_bounds( const IntRect& bounds )
{
  this->parent_bounds_ = bounds;
}

void UIWidget::set_event_dispatcher( IUIEventDispatcher* dispatcher )
{
  this->dispatcher_.reset( dispatcher );
}

/*
void UIWidget::validate_dimensions( const IDrawable::raw_ptr object )
{
  // Check to see if the end-user (developer) has passed a drawable type that
  // inherits from nom::Transformable. If so, ensure that the dimensions are not
  // in an invalid (null) state, so that we are able to render the object.
  if( NOM_ISA( Transformable*, object ) == true )
  {
    Transformable* drawable = NOM_DYN_PTR_CAST(Transformable*, object );

    // If the object's positioning is in an invalid (null) state, use the
    // positioning of this (parent) object.
    if( drawable->position() == Point2i::null )
    {
      drawable->set_position( this->position() );
    }

    // If the object's size is in an invalid (null) state, use the size of this
    // (parent) object.
    if( drawable->size() == Size2i::null )
    {
      drawable->set_size( this->size() );
    }
  }
}
*/

} // namespace nom
