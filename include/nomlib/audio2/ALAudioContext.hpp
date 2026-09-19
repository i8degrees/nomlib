#pragma once

#include "nomlib/types.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio2/IAudioContext.hpp"

namespace nom {
namespace audio {

class ALAudioContext: public IAudioContext
{
  public:
    ALAudioContext();
    ALAudioContext(ALCdevice* dev);

    virtual ~ALAudioContext();

    ALCcontext* context() const;
    void set_context(ALCdevice* dev);

    // FIXME(JEFF): Investigate why we see warning messages regarding our
    // context closure when ALSOFT_LOGLEVEL=3 is set
    // Shutdown our OpenAL context
    // This call can fail when a) one or more buffers are still allocated; b)
    // the presently allocated OpenAL context is ...
    virtual void release() override;

    virtual void resume() override; // unwind (begin)
    virtual void pause() override; // suspsned
    virtual bool connected() override; // ALC_CONNECTED extension
    virtual bool valid() override; // ctx_ != nullptr

    virtual nom::real32 master_volume() override;
    virtual void set_master_volume(nom::real32 gain) override;

    virtual void update(nom::real32 delta) override;
    //virtual void render(nom::real32 delta) override;
  private:
    ALCdevice* device() const;

    void initialize(ALCdevice* dev);

    ALCcontext* ctx_;
};

bool context_extension(const char* key, ALCdevice* target);
int /*ALenum*/ enum_available(const char* key);
void* context_function(ALCdevice* target, const ALCchar* fn);

} // namespace audio
} // namespace nom
