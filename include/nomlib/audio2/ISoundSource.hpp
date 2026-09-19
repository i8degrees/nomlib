#pragma once

#include "nomlib/types.hpp"
#include "nomlib/math/Point3.hpp"
#include <vector>


namespace nom {
namespace audio {

// Forward declarations
class Sound;

struct OALSource {
  nom::uint32 source;
  bool used;

  OALSource(nom::uint32 src) {
    source = src;
    used = false;
  }
};

enum SoundPriority: nom::uint32 {
  SOUND_PRIORITY_LOW = 0x00,
  SOUND_PRIORITY_MEDIUM = 0x02,
  SOUND_PRIORITY_HIGH = 0x04,
  SOUND_PRIORITY_ALWAYS = 0x6,
};

// SoundNode
class ISoundSource
{
  public:
    virtual void reset();
    virtual ~ISoundSource();

    Sound* sound();
    SoundPriority priority();

    nom::uint32 state();
    nom::real32 volume();
    nom::real32 pitch();
    nom::real32 radius();
    bool looping();
    bool global();
    nom::real64 time_left();
    nom::Point3f& position();

    void set_priority(SoundPriority p);
    void set_volume(nom::real32 gain);
    void set_pitch(nom::real32 pitch);
    void set_looping(bool state);
    void set_radius(nom::real32 value);
    void set_global(bool state);
    void set_position(const nom::Point3f& pos);
    void set_position(nom::real32 x, nom::real32 y, nom::real32 z);

    // Update(real32);
    virtual void update(nom::real32 delta) = 0;
    virtual void update_state(nom::real32 delta) = 0;
    virtual void attach_source(OALSource* s) = 0;
    virtual void detach_source() = 0;
    virtual OALSource* source() = 0;
    virtual void set_sound(Sound *s) = 0;
  protected:
    ISoundSource();

    Sound* sound_;
    SoundPriority priority_;
    nom::uint32 state_;
    nom::real32 volume_;
    nom::real32 pitch_;
    nom::real32 radius_;
    bool looping_;
    bool global_;
    nom::real64 time_left_;
    nom::Point3f position_;
};

using SoundSourceIterator = std::vector<ISoundSource*>::iterator;

} // namespace audio
} // namespace nom
