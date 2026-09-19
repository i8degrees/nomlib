#pragma once

#include "nomlib/types.hpp"
#include "nomlib/audio/SoundFile.hpp"
#include "nomlib/math/Matrix.hpp"
#include "nomlib/audio2/IAudioDevice.hpp"
#include "nomlib/audio2/ALAudioContext.hpp"
#include "nomlib/audio2/ISoundSource.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"

#include <vector>

class Matrix4;

namespace nom {
namespace audio {

// Forward declarations
class ISoundSource;
class Sound;
struct OALSource;
//class RenderWindow;

using SoundSourceIterator = std::vector<ISoundSource*>::iterator;

// SoundSystem impl
class ALAudioRenderer: public IAudioDevice
{
  public:
    // Default constructor
    ALAudioRenderer();
    ALAudioRenderer(nom::uint32 channels);

    virtual ~ALAudioRenderer();
    virtual void release() override;

    // used by ALAudioSource::update
    // audio sources attached by this method are part of the emitters
    // container (managed by this class)
    // (see ALAudioRenderer::update).
    virtual void add_source(ISoundSource* node) override;

    // audio sources attached by this method are part of the temporary
    // emitters container (managed by this class).
    virtual void play_sound(Sound* s) override;

    // audio sources attached by this method are part of the temporary
    // emitters container (managed by this class).
    virtual void play_sound(Sound* s, const nom::Point3f& pos) override;

    // audio sources attached by this method are part of the temporary
    // emitters container (managed by this class).
    virtual void play_sound(Sound* s, SoundPriority p) override;

    // audio sources attached by this method are part of the temporary
    // emitters container (managed by this class).
    virtual void play_sound(Sound* s, nom::real32 gain) override;

    //virtual void set_listener(const Matrix4& node) override;

    // responsible for updating emitters && temporary emitters and must be
    // called inside the main game loop
    virtual void update(nom::real32 delta) override;

    // Return a free source (from the number of channels specified at init)
    OALSource* source();

    virtual void set_master_volume(nom::real32 gain) override;
  private:
    //void init_audio(int32 channels);
    void initialize(nom::uint32 channels);
    void update_listener();
    //void update_temps(real32 delta);
    //void update_internal(real32 delta);
    void cull_nodes();
    void detach_sources(SoundSourceIterator from, SoundSourceIterator to);
    void attach_sources(SoundSourceIterator from, SoundSourceIterator to);

    // Relocate to ALAudioContext or IAudioContext?
    Matrix4 listener_;
    // OpenAL audio sources that are in use -- this can be greater than the
    // number of channels specified at init time
    std::vector<OALSource*> sources_;
    // container for audio source emitters that are expected to be around for
    // some time, i.e.: streaming sources.
    std::vector<ISoundSource*> emitters_;
    // container for audio source emitters that are not expected to be around
    // but for a brief time.
    std::vector<ISoundSource*> temporary_emitters_;

    ALCdevice* impl_;
    ALAudioContext* ctx_;
};

bool extension_available(const char* key);
int /*ALenum*/ enum_available(const char* key);
void* device_function(const ALCchar* fn);

// Convert from nomlib's native AudioFormat enumeration to
// the equivalent OpenAL format.
//
// AudioFormat -> AL_FORMAT_XXX
// num_channels == 1 == AL_FORMAT_MONO
// num_channels == 2 == AUDIO_FORMAT_AL_FORMAT_STEREO
// ...
// ?? Rename to openal_format
ALenum channel_format(nom::uint32 num_channels, nom::audio::AudioFormat channel_format);

// Convert from nomlib's native AudioCaps enumeration to
// the equivalent OpenAL format.
//
// CAPS_FORMAT_MONO_S8 -> AL_FORMAT_MONO8
//
//ALenum channel_format(uint32 num_channels, AudioCaps channel_format);

} // namespace audio
} // namespace nom
