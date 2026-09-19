#pragma once

#include "nomlib/types.hpp"

namespace nom {
namespace audio {

bool init_audio(nom::uint32 channels);
void shutdown_audio();

} // namespace audio
} // namespace nom
