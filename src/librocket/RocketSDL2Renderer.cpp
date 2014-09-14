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
#include "nomlib/librocket/RocketSDL2Renderer.hpp"

// Private headers
#include <SDL_image.h>

#if !(SDL_VIDEO_RENDER_OGL)
  #error "Only the opengl sdl backend is supported. To add support for others, see http://mdqinc.com/blog/2013/01/integrating-librocket-with-sdl-2/"
#endif

// Forward declarations
#include "nomlib/graphics/RenderWindow.hpp"

namespace nom {

bool RocketSDL2Renderer::gl_init( int width, int height )
{
  // Initialize OpenGL for SDL2 + libRocket play along
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  glMatrixMode( GL_PROJECTION | GL_MODELVIEW );
  glLoadIdentity();
  glOrtho( 0, width, height, 0, 0, 1 );

  return true;
}

RocketSDL2Renderer::RocketSDL2Renderer(RenderWindow* window )
{
  this->window_ = window;
}

void RocketSDL2Renderer::RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation)
{
  // SDL uses shaders that we need to disable here
  glUseProgramObjectARB(0);
  glPushMatrix();
  glTranslatef(translation.x, translation.y, 0);

  std::vector<Rocket::Core::Vector2f> Positions(num_vertices);
  std::vector<Rocket::Core::Colourb> Colors(num_vertices);
  std::vector<Rocket::Core::Vector2f> TexCoords(num_vertices);
  float texw, texh;

  SDL_Texture* sdl_texture = NULL;
  if(texture)
  {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    sdl_texture = (SDL_Texture *) texture;
    SDL_GL_BindTexture(sdl_texture, &texw, &texh);
  }

  for(int  i = 0; i < num_vertices; i++)
  {
    Positions[i] = vertices[i].position;
    Colors[i] = vertices[i].colour;
    if (sdl_texture)
    {
      TexCoords[i].x = vertices[i].tex_coord.x * texw;
      TexCoords[i].y = vertices[i].tex_coord.y * texh;
    }
    else
    {
      TexCoords[i] = vertices[i].tex_coord;
    }
  };  // ...does this semicolon at the end here affect this loop???

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, &Positions[0]);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, &Colors[0]);
  glTexCoordPointer(2, GL_FLOAT, 0, &TexCoords[0]);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, indices);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  if (sdl_texture)
  {
    SDL_GL_UnbindTexture(sdl_texture);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  glColor4f(1.0, 1.0, 1.0, 1.0);
  glPopMatrix();

  /* Reset blending and draw a fake point just outside the screen to let SDL know that it needs to reset its state in case it wants to render a texture */
  glDisable(GL_BLEND);
  SDL_SetRenderDrawBlendMode(this->window_->renderer(), SDL_BLENDMODE_NONE);
  SDL_RenderDrawPoint(this->window_->renderer(), -1, -1);

  // Reset the renderer's drawing color; this is necessary because otherwise
  // the GL color call above this statement overwrites any drawing colors set
  // by nomlib's SDL2 rendering subsystem.
  //
  // I don't really know what I'm doing here! I just know that it appears to
  // work in the instance I'm working in (custom libRocket decorator)...
  if( this->window_->set_color( Color4i::Blue ) == false )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
  }
}

void RocketSDL2Renderer::EnableScissorRegion(bool enable)
{
  if(enable)
  {
    glEnable(GL_SCISSOR_TEST);
  }
  else
  {
    glDisable(GL_SCISSOR_TEST);
  }
}

void RocketSDL2Renderer::SetScissorRegion(int x, int y, int width, int height)
{
    int w_width, w_height;
    SDL_GetWindowSize(this->window_->window(), &w_width, &w_height);
    glScissor(x, w_height - (y + height), width, height);
}

bool RocketSDL2Renderer::LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source)
{
  Rocket::Core::FileInterface* file_interface = Rocket::Core::GetFileInterface();
  Rocket::Core::FileHandle file_handle = file_interface->Open(source);

  if( !file_handle )
  {
    NOM_LOG_ERR( NOM_LOG_CATEGORY_GUI, "Could not obtain file handle for source:", source.CString() );
    return false;
  }

  file_interface->Seek(file_handle, 0, SEEK_END);
  size_t buffer_size = file_interface->Tell(file_handle);
  file_interface->Seek(file_handle, 0, SEEK_SET);

  char* buffer = new char[buffer_size];
  file_interface->Read(buffer, buffer_size, file_handle);
  file_interface->Close(file_handle);

  size_t i;
  for(i = source.Length() - 1; i > 0; i--)
  {
    if(source[i] == '.')
    {
      break;
    }
  }

  Rocket::Core::String extension = source.Substring(i+1, source.Length()-i);

  SDL_Surface* surface = IMG_LoadTyped_RW(SDL_RWFromMem(buffer, buffer_size), 1, extension.CString());

  if(surface)
  {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(this->window_->renderer(), surface);

    if(texture)
    {
      texture_handle = (Rocket::Core::TextureHandle) texture;
      texture_dimensions = Rocket::Core::Vector2i(surface->w, surface->h);
      SDL_FreeSurface(surface);
    }
    else
    {
      return false;
    }

    return true;
  }

  return false;
}

bool RocketSDL2Renderer::GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions)
{
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
  #else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
  #endif

  SDL_Surface *surface = SDL_CreateRGBSurfaceFrom ((void*) source, source_dimensions.x, source_dimensions.y, 32, source_dimensions.x*4, rmask, gmask, bmask, amask);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(this->window_->renderer(), surface);

  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_FreeSurface(surface);

  texture_handle = (Rocket::Core::TextureHandle) texture;

  return true;
}

void RocketSDL2Renderer::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
  SDL_DestroyTexture((SDL_Texture*) texture_handle);
}

} // namespace nom
