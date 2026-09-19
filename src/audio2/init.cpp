
#include "nomlib/audio2/init.hpp"
#include "nomlib/audio2/ALAudioRenderer.hpp"
#include "nomlib/audio2/AudioDeviceLocator.hpp"
#include "nomlib/audio2/SoundManager.hpp"

#include <iostream>

using namespace nom;

namespace nom {
namespace audio {

bool init_audio(uint32 channels)
{
  auto dev = new ALAudioRenderer(channels);
  if(dev == nullptr) {
    std::cerr << "Failed to allocate memory for audio system." << std::endl;
    return false;
  }

  AudioDeviceLocator::setup_provider(dev);
  return true;
}

void shutdown_audio()
{
  NOM_LOG_TRACE(NOM);
  SoundManager::free_sounds();
  AudioDeviceLocator::release();
}

#if defined(EXPERIMENTAL)
bool init_audio_input(uint32 channels)
{
  auto dev = new ALAudioRenderer();
}
#endif

} // namespace audio
} // namespace nom
