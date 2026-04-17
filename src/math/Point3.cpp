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
#include "nomlib/math/Point3.hpp"

namespace nom {

/// Null value for a nom::Point3 using signed integers
template <> const Point3i Point3i::null(-1, -1, -1);

/// Null value for a nom::Point3 using floating point numbers
template <> const Point3f Point3f::null(-1.0f, -1.0f, -1.0f);

/// Null value for a nom::Point3 using double precision floating point numbers
template <> const Point3d Point3d::null(-1.0f, -1.0f, -1.0f);

/// \brief Zero value for a nom::Point3 using signed integers
template <> const Point3i Point3i::zero(0, 0, 0);

/// \brief Zero value for a nom::Point3 using 32-bit floating-point numbers.
template <> const Point3f Point3f::zero(0.0f, 0.0f, 0.0f);

/// \brief Zero value for a nom::Point3 using double precision (64-bit)
/// floating-point numbers.
template <> const Point3d Point3d::zero(0.0f, 0.0f, 0.0f);

} // namespace nom
