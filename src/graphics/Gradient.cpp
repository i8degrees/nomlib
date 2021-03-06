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

// Forward declarations
#include "nomlib/graphics/Texture.hpp"

namespace nom {

Gradient::Gradient( void ) :
  Transformable( Point2i::null, Size2i::null ),   // Invalid position & size
  gradient_( { Color4i::Blue, Color4i::Blue } )   // Opaque color to serve as
                                                  // warning!
{
  this->texture_.reset( new Texture() );

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
  gradient_(colors)
{
  this->texture_.reset( new Texture() );

  this->set_margins( margin );
  this->set_fill_direction( direction );
  this->set_dithering( true );
}

Gradient::~Gradient( void )
{
  //NOM_LOG_TRACE( NOM );
}

IDrawable::raw_ptr Gradient::clone( void ) const
{
  return Gradient::raw_ptr( new Gradient( *this ) );
}

ObjectTypeInfo Gradient::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

Texture* Gradient::texture() const
{
  if( this->texture_ != nullptr ) {
    return( new Texture(*this->texture_) );
  } else {
    return nullptr;
  }
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
  if( this->texture_ != nullptr && this->texture_->valid() == true ) {
    this->texture_->draw(target);
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

  // Clear internal cache
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

  if( this->update_cache() == false )
  {
    return;
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
    // Calculate rendering offsets and cache for rendering to texture
    IntRect render_coords = IntRect( this->position().x + this->margins().x, rows, this->size().w - this->margins().x, 1 );
    Color4i render_color = Color4i( currentR, currentG, currentB );

    this->rectangles_.push_back( Rectangle( render_coords, render_color ) );

    if ( this->dithering() )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
  } // end blit loop

  // NOM_ASSERT( this->rectangles_.size() == ( this->size().w - this->margins().y ) - 1);
  // NOM_DUMP( this->size() );
  // NOM_DUMP( this->rectangles_.size() );
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
    // Calculate rendering offsets and cache for rendering to texture
    IntRect render_coords = IntRect( rows, this->position().y + this->margins().y, 1, this->size().h - this->margins().y );
    Color4i render_color = Color4i( currentR, currentG, currentB );

    this->rectangles_.push_back( Rectangle( render_coords, render_color ) );

    if ( this->dithering() )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
  } // end blit loop

  // NOM_ASSERT( this->rectangles_.size() == this->size().w );
  // NOM_ASSERT( this->rectangles_.size() == ( this->size().w - this->margins().x ) );
  // NOM_DUMP( this->size() );
  // NOM_DUMP( this->rectangles_.size() );
}

bool Gradient::update_cache()
{
  RenderWindow* context = nom::render_interface();
  NOM_ASSERT( context != nullptr );

  // Sanity check
  if( context == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update texture cache for the gradient:",
                  "invalid renderer.");
    return false;
  }

  // Obtain the optimal pixel format for the platform
  RendererInfo caps = context->caps();

  if( this->texture_ == nullptr ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update texture cache for the gradient:",
                  "NULL texture.");
    return false;
  }

  if( this->texture_->size() != this->size() ) {

    // Poor man's counter of how often we are re-allocating this texture
    NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_RENDER, NOM_LOG_PRIORITY_DEBUG);
    NOM_LOG_DEBUG(  NOM_LOG_CATEGORY_RENDER,
                    "old_size:", this->texture_->size(),
                    "new_size:", this->size() );

    // Create the texture cache that will hold our gradient; since the
    // dimensions are local to this object, we deal with translating relative
    // coordinates to what will be the absolute coordinate space -- our
    // output rendering window
    if( this->texture_->initialize( caps.optimal_texture_format(), SDL_TEXTUREACCESS_TARGET, this->size() ) == false )
    {
      NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                    "Could not initialize the texture cache for the gradient." );
      return false;
    }
  }

  // Local coordinates (relative)
  this->texture_->set_bounds( IntRect( this->margins(), this->size() ) );
  this->texture_->set_position( this->position() + this->margins() );

  // Set the rendering target to the texture (uses FBO)
  if( context->set_render_target( this->texture_.get() ) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update texture cache: failed to set rendering"
                  "target" );
    return false;
  }

  // Debugging aid; red background indicates something went wrong
  if( context->fill(Color4i::Red) == false ) {
  }

  // Relative to absolute transformations
  Point2i relative_pos;
  for( auto itr = this->rectangles_.begin(); itr != this->rectangles_.end(); ++itr )
  {
    relative_pos = ( (*itr).position() - this->position() ) + this->margins();

    itr->set_position(relative_pos);
    itr->draw(*context);
  }

  // Reset the rendering target now that we are done using it.
  if( context->reset_render_target() == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not update texture cache:",
                  "failed to reset the rendering target." );
    return false;
  }

  NOM_ASSERT( this->texture_->position() == this->position() + this->margins() );
  NOM_ASSERT( this->texture_->bounds() == IntRect( this->margins(), this->size() ) );

  return true;
}

// Broken
// Texture* Gradient::clone_texture() const
// {
//   Texture* texture = new Texture();
//   NOM_ASSERT(texture != nullptr);

//   RenderWindow* context = nom::render_interface();
//   NOM_ASSERT(context != nullptr);

//   // Sanity check
//   if( context == nullptr ) {
//     NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
//                   "Could not update texture cache for the gradient:",
//                   "invalid renderer.");
//     NOM_DELETE_PTR(texture);
//     return nullptr;
//   }

//   // Obtain the optimal pixel format for the platform
//   RendererInfo caps = context->caps();

//   // Create the texture cache that will hold our gradient; since the
//   // dimensions are local to this object, we deal with translating relative
//   // coordinates to what will be the absolute coordinate space -- our
//   // output rendering window
//   if( texture->initialize( caps.optimal_texture_format(), SDL_TEXTUREACCESS_TARGET, this->size() ) == false )
//   {
//     NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
//                   "Could not initialize the texture cache for the gradient." );
//     NOM_DELETE_PTR(texture);
//     return nullptr;
//   }

//   // Local coordinates (relative)
//   texture->set_bounds( IntRect( this->margins(), this->size() ) );
//   texture->set_position( this->position() + this->margins() );

//   // Set the rendering target to the texture (uses FBO)
//   if( context->set_render_target(texture) == false ) {
//     NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
//                   "Could not update texture cache: failed to set rendering"
//                   "target" );
//     NOM_DELETE_PTR(texture);
//     return nullptr;
//   }

//   // Debugging aid; red background indicates something went wrong
//   if( context->fill(Color4i::Red) == false ) {
//   }

//   this->draw(*context);

//   // Reset the rendering target now that we are done using it.
//   if( context->reset_render_target() == false ) {
//     NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
//                   "Could not update texture cache:",
//                   "failed to reset the rendering target." );
//     NOM_DELETE_PTR(texture);
//     return nullptr;
//   }

//   NOM_ASSERT( texture->position() == this->position() + this->margins() );
//   NOM_ASSERT( texture->bounds() == IntRect( this->margins(), this->size() ) );

//   return texture;
// }

} // namespace nom
