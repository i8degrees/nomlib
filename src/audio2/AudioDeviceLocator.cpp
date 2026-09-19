

#include "nomlib/audio2/AudioDeviceLocator.hpp"
#include "nomlib/audio2/IAudioDevice.hpp" // IAudioRenderer
#include "nomlib/audio2/ALAudioRenderer.hpp"
#include "nomlib/audio2/ALAudioContext.hpp"

// Private forward declarations
class IAudioRenderer;

using namespace nom;
using namespace nom::audio;

namespace nom {
namespace audio {

// Static initializations
ptr_renderer AudioDeviceLocator::audio_ = nullptr;

AudioDeviceLocator::~AudioDeviceLocator()
{
  NOM_LOG_TRACE(NOM);

  AudioDeviceLocator::release();
}

// static
void AudioDeviceLocator::release()
{
  NOM_LOG_TRACE(NOM);

  delete AudioDeviceLocator::audio_;
  AudioDeviceLocator::audio_ = nullptr;
}

// static
void AudioDeviceLocator::initialize()
{
  NOM_LOG_TRACE(NOM);
  AudioDeviceLocator::audio_ = create_audio_renderer(128);
}

// static
ptr_renderer AudioDeviceLocator::instance() {
  if(AudioDeviceLocator::audio_ == nullptr) {
    AudioDeviceLocator::initialize();
  }

  return AudioDeviceLocator::audio_;
}

// static
void AudioDeviceLocator::setup_provider(ptr_renderer service)
{
  NOM_LOG_TRACE(NOM);

  if(service == nullptr) {
    AudioDeviceLocator::initialize();
  }

  AudioDeviceLocator::audio_ = service;
}

// private function
ptr_renderer create_audio_renderer(uint32 channels)
{
  return(new ALAudioRenderer(channels));
}

} // namespace audio
} // namespace nom
