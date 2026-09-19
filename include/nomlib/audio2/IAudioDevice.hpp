#pragma once

#include "nomlib/types.hpp"
#include "nomlib/math/Matrix.hpp"
#include "nomlib/audio2/ISoundSource.hpp"

namespace nom {
namespace audio {

//class RenderWindow;
class Sound;

// Pure abstract interface class
// ?? Rename to IAudioDriver
class IAudioDevice
{
  public:
    virtual ~IAudioDevice() {}

    virtual void release() = 0;

    virtual void update(nom::real32 delta) = 0;

    virtual void add_source(ISoundSource* node) = 0;
    virtual void play_sound(Sound* s) = 0;
    virtual void play_sound(Sound* s, const nom::Point3f& pos) = 0;
    virtual void play_sound(Sound* s, SoundPriority p) = 0;
    virtual void play_sound(Sound* s, nom::real32 gain) = 0;

    virtual Matrix4& listener() { return this->listener_; }
    virtual void set_listener(const Matrix4& node) { this->listener_ = node; }
    virtual void set_master_volume(nom::real32 gain) = 0;

  protected:
    IAudioDevice() {}

  private:
    Matrix4 listener_;
};

} // namespace audio
} // namespace nom
