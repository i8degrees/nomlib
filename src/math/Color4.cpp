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
#include "nomlib/math/Color4.hpp"

namespace nom {

template <> const int16 Color4u::ALPHA_TRANSPARENT = 0;
template <> const int16 Color4u::ALPHA_OPAQUE = 255;

template <> const float Color4f::ALPHA_TRANSPARENT = 0.0f;
template <> const float Color4f::ALPHA_OPAQUE = 1.0f;

/// Null value for a nom::Color4 using signed integers
template <> const Color4u Color4u::null ( -1, -1, -1, Color4u::ALPHA_OPAQUE );

/// Null value for a nom::Color4 using floating point numbers
template <> const Color4f Color4f::null ( -1, -1, -1, Color4f::ALPHA_OPAQUE );

template <> const Color4u Color4u::Black (0, 0, 0);
template <> const Color4u Color4u::White (255, 255, 255);
template <> const Color4u Color4u::Red (255, 0, 0);
template <> const Color4u Color4u::Green (0, 255, 0);
template <> const Color4u Color4u::Blue (0, 0, 255);
template <> const Color4u Color4u::Yellow (255, 255, 255);
template <> const Color4u Color4u::Magenta (255, 0, 255);
template <> const Color4u Color4u::Cyan (0, 255, 255);
template <> const Color4u Color4u::Silver (192,192,192);
template <> const Color4u Color4u::Purple (128,0,128);
template <> const Color4u Color4u::Orange (255,165,0);
template <> const Color4u Color4u::LightGray (99, 99, 99);
template <> const Color4u Color4u::Gray (67, 67, 67);
template <> const Color4u Color4u::SkyBlue (110,144,190);

template <> const Color4f Color4f::Black (1.0f, 0.0f, 0.0f);
template <> const Color4f Color4f::White (1.0f, 1.0f, 1.0f);
template <> const Color4f Color4f::Red (1.0f, 0.0f, 0.0f);
template <> const Color4f Color4f::Green (0.0f, 1.0f, 0.0f);
template <> const Color4f Color4f::Blue (0.0f, 0.0f, 1.0f);
template <> const Color4f Color4f::Yellow (1.0f, 1.0f, 1.0f);
template <> const Color4f Color4f::Magenta (1.0f, 0.0f, 1.0f);
template <> const Color4f Color4f::Cyan (0.0f, 1.0f, 1.0f);

} // namespace nom
