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
#ifndef NOMLIB_GRAPHICS_HPP
#define NOMLIB_GRAPHICS_HPP

// Public header file

#include <nomlib/config.hpp>
#include <nomlib/graphics/Text.hpp>
#include <nomlib/graphics/RendererInfo.hpp>
#include <nomlib/graphics/Texture.hpp>
#include <nomlib/graphics/DisplayMode.hpp>
#include <nomlib/graphics/RenderWindow.hpp>
#include <nomlib/graphics/Renderer.hpp>
#include <nomlib/graphics/IDrawable.hpp>
#include <nomlib/graphics/Gradient.hpp>
#include <nomlib/graphics/Image.hpp>
#include <nomlib/graphics/fonts/BMFont.hpp>
#include <nomlib/graphics/fonts/BitmapFont.hpp>
#include <nomlib/graphics/fonts/FontMetrics.hpp>
#include <nomlib/graphics/fonts/FontPage.hpp>
#include <nomlib/graphics/fonts/FontRow.hpp>
#include <nomlib/graphics/fonts/Glyph.hpp>
#include <nomlib/graphics/fonts/TrueTypeFont.hpp>
#include <nomlib/graphics/fonts/Font.hpp>
#include <nomlib/graphics/shapes/Shape.hpp>
#include <nomlib/graphics/shapes/Point.hpp>
#include <nomlib/graphics/shapes/Line.hpp>
#include <nomlib/graphics/shapes/Rectangle.hpp>
#include <nomlib/graphics/sprite/Sprite.hpp>
#include <nomlib/graphics/sprite/SpriteBatch.hpp>
#include <nomlib/graphics/sprite/SpriteSheet.hpp>
#include <nomlib/graphics/sprite/AnimatedSprite.hpp>
#include <nomlib/graphics/Cursor.hpp>
#include <nomlib/graphics/graphics_helpers.hpp>

#include <nomlib/graphics/AnimationTimingModes.hpp>
#include <nomlib/graphics/IActionObject.hpp>
#include <nomlib/graphics/AnimationTexture.hpp>
#include <nomlib/graphics/FadeInAction.hpp>
#include <nomlib/graphics/FadeOutAction.hpp>
#include <nomlib/graphics/FadeAlphaByAction.hpp>
#include <nomlib/graphics/WaitForDurationAction.hpp>
#include <nomlib/graphics/MoveByAction.hpp>
#include <nomlib/graphics/MoveToAction.hpp>
#include <nomlib/graphics/ScaleByAction.hpp>
#include <nomlib/graphics/ReversedAction.hpp>
#include <nomlib/graphics/RepeatForAction.hpp>
#include <nomlib/graphics/RepeatForeverAction.hpp>
#include <nomlib/graphics/CallbackAction.hpp>
#include <nomlib/graphics/FadeAudioByAction.hpp>
#include <nomlib/graphics/RemoveAction.hpp>
#include <nomlib/graphics/SequenceAction.hpp>
#include <nomlib/graphics/GroupAction.hpp>
#include <nomlib/graphics/ActionPlayer.hpp>

#endif // include guard defined
