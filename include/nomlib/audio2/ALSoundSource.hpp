#pragma once

#include "nomlib/types.hpp"
#include "nomlib/audio2/Sound.hpp"
#include "nomlib/audio2/ISoundSource.hpp"

#define NUM_STREAM_BUFFERS 3

namespace nom {
namespace audio {

// Forward declarations
struct OALSource;

// SoundNode
class ALSoundSource: public virtual ISoundSource
{
  public:
    typedef ISoundSource super;

    ALSoundSource();
    ALSoundSource(Sound* s);

    virtual ~ALSoundSource();
    virtual void reset() override;

    OALSource* source() override;
    void set_sound(Sound *s) override;

    // stl sort
    static bool compare(ISoundSource* lhs, ISoundSource* rhs);

    void attach_source(OALSource* s) override;
    void detach_source() override;

    // Update(real32);
    virtual void update(nom::real32 delta) override;
  private:
    // UpdateSoundState(real32);
    void update_state(nom::real32 delta) override;
  protected:
    OALSource*  oalSource_;
    // Part 2
    nom::real64 stream_pos_;
    nom::uint32 stream_buffers_[NUM_STREAM_BUFFERS];
};

} // namespace audio
} // namespace nom
