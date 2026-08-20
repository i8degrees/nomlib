/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016, 2026
Jeffrey Carpenter <1329364+i8degrees@users.noreply.github.com>
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
#pragma once

#if defined(NOM_USE_OPENAL_SOFT)
  // Standard OpenAL API signature
  typedef struct ALCcontext ALCcontext_struct;
  typedef struct ALCdevice ALCdevice_struct;
#elif defined(NOM_USE_APPLE_OPENAL)
  // Apple's OpenAL API signature
  typedef struct ALCcontext_struct ALCcontext;
  typedef struct ALCdevice_struct ALCdevice;

  typedef ALCcontext ALCcontext_struct;
  typedef ALCdevice ALCdevice_struct;
#else // Catch-all
  // Fallback for when neither OpenAL header style test suceeds for whatever
  // reason...
  typedef struct ALCcontext ALCcontext_struct;
  typedef struct ALCdevice ALCdevice_struct;
#endif

