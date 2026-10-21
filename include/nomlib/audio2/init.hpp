#pragma once

#include "export.hpp"
#include "nomlib/types.hpp"

namespace nom {
namespace audio {

NOM_EXPORT bool init_audio(nom::uint32 channels);
NOM_EXPORT void shutdown_audio();

} // namespace audio
} // namespace nom
