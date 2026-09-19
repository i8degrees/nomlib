#pragma once

#include "nomlib/types.hpp"
#include "nomlib/audio2/IAudioDevice.hpp"

typedef nom::audio::IAudioDevice* ptr_renderer;
// typedef IAudioContext* ptr_ctx;

/* Experimental typedefs - unused but reserved */
typedef std::unique_ptr<ptr_renderer> unique_ptr;
typedef std::shared_ptr<ptr_renderer> shared_ptr;
typedef std::weak_ptr<ptr_renderer> weak_ptr;

namespace nom {
namespace audio {

// Forward declarations
class IAudioDevice;
class IAudioContext;

// Global "Singleton" class interface for audio subsystem
class AudioDeviceLocator
{
  public:
    static void release();
    static ptr_renderer instance();

    static void setup_provider(ptr_renderer dev);
  protected:
    AudioDeviceLocator();
    ~AudioDeviceLocator();
  private:
    static void initialize();
    static ptr_renderer audio_; // device handle
};

ptr_renderer create_audio_renderer(nom::uint32 channels);
// ?? Future implementation
// ptr_renderer create_audio_renderer(nom::uint32 channels = 64, uint32 audio_specs);

//ictx_renderer create_audio_input(nom::uint32 channels);

} // namespace audio
} // namespace nom
