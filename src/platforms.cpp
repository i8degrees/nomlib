/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/platforms.hpp"
#include "nomlib/types.hpp"

#include <SDL.h>

namespace nom {

// Source: http://www.brue.org/2013/01/using-rdtsc-in-gcc-and-clang/
volatile uint64 rdtsc()
{
  uint32 a = 0.0f;
  uint32 d = 0.0f;
  asm volatile
      (".byte 0x0f, 0x31 #rdtsc\n" // edx:eax
       :"=a"(a), "=d"(d)::);
  return( ( (uint64) d) << 32) | (uint64) a;
}

PlatformSpec platform_info()
{
  PlatformSpec spec = {};
  spec.num_cpus = SDL_GetCPUCount();
  auto cpu_cache_size_kb = SDL_GetCPUCacheLineSize();
  auto total_ram_kb =  SDL_GetSystemRAM();

  // Conversion to bytes
  spec.cpu_cache_size = cpu_cache_size_kb * 1024;
  spec.total_ram =  total_ram_kb * 1024;

#if defined(NOM_PLATFORM_OSX)
  spec.name = "Mac OS X";
  spec.env = PLATFORM_OSX;
#elif defined(NOM_PLATFORM_LINUX)
  spec.name = "Linux";
  spec.env = PLATFORM_LINUX;
#elif defined(NOM_PLATFORM_BSD)
  spec.name = "BSD";
  spec.env = PLATFORM_BSD;
#elif defined(NOM_PLATFORM_WINDOWS)
  spec.name = "Windows";
  spec.env = PLATFORM_WINDOWS;
#endif

  return spec;
}

} // namespace nom
