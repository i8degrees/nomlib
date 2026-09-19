
#include "nomlib/types.hpp"
#include "nomlib/audio2/ALAudioContext.hpp"
#include "nomlib/math/math_helpers.hpp"

using namespace std;
using namespace nom;

namespace nom {
namespace audio {

ALAudioContext::ALAudioContext()
{
  NOM_LOG_TRACE(NOM);
  this->ctx_ = nullptr;
  this->initialize(nullptr);
}

ALAudioContext::ALAudioContext(ALCdevice* dev)
{
  NOM_LOG_TRACE(NOM);
  this->initialize(dev);
}

ALAudioContext::~ALAudioContext()
{
  NOM_LOG_TRACE(NOM);
  this->release();
  NOM_DELETE_VOID_PTR(this->ctx_);
  this->ctx_ = nullptr;
}

void ALAudioContext::resume() // unwind (begin)
{
  auto dev = this->device();

  ALC_CLEAR_ERR(dev);
  ALC_CHECK_ERR(alcProcessContext(this->ctx_), dev);
}

void ALAudioContext::pause()
{
  auto dev = this->device();

  ALC_CLEAR_ERR(dev);
  ALC_CHECK_ERR(alcSuspendContext(this->ctx_), dev);
}

void ALAudioContext::release()
{
  NOM_LOG_TRACE(NOM);

  if(this->ctx_ == nullptr) {
    // NO-OP -- our context is not present!
    return;
  }

  // Used by error checking macros
  auto dev = this->device();

  // A context which is not current can be destroyed at any time (all
  // sources within that context will also be deleted).
  // alcMakeContextCurrent should be used to make sure the context to
  // be destroyed is not current (NULL is valid for alcMakeContextCurrent).
  ALCboolean result = alcMakeContextCurrent(NULL);
  if(result == ALC_TRUE) {
    ALC_CLEAR_ERR(dev);
    ALC_CHECK_ERR(alcDestroyContext(this->ctx_), dev);
  } else {
    std::cerr << "Failed to destroy our audio context handle!" << std::endl;
  }
}

void ALAudioContext::update(real32 delta)
{
  // NO-OP
}
#if 0
void ALAudioContext::render(real32 delta)
{
  // NO-OP
}
#endif
bool ALAudioContext::connected()
{
  return false;
}

bool ALAudioContext::valid()
{
  bool result = false;
  if(this->ctx_ != nullptr) {
    result = true;
  }

  return result;
}

// public scope

ALCcontext* ALAudioContext::context() const
{
  return this->ctx_;
}

void ALAudioContext::set_context(ALCdevice* dev)
{
  this->initialize(dev);
}

real32 ALAudioContext::master_volume()
{
  return this->master_gain_;
}

void ALAudioContext::set_master_volume(real32 gain)
{
  gain = nom::clamp_max<real32>(0.0f, gain);
  gain = nom::clamp_min<real32>(1.0f, gain);
  this->master_gain_ = gain;

#if defined(DEBUG) && defined(VERBOSE)
  std::cout << "ALAudioContext::set_master_volume(" << this->master_gain_ <<
    ")" << std::endl;
#endif

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alListenerf(AL_GAIN, this->master_gain_));
}

// Private scope
ALCdevice* ALAudioContext::device() const
{
  ALCdevice* result = alcGetContextsDevice(this->ctx_);
  return result;
}

// Private scope -- member methods

void ALAudioContext::initialize(ALCdevice* dev/*, std::map<int, >*/)
{
  NOM_LOG_TRACE(NOM);

  ALC_CLEAR_ERR(dev);
  this->ctx_ = alcCreateContext(dev, nullptr);
  if(this->ctx_ == nullptr) {
    std::cerr << "Failed to create an audio context." <<
      std::endl;
    return;
  }

  ALC_CLEAR_ERR(dev);
  ALC_CHECK_ERR(alcMakeContextCurrent(this->ctx_), dev);
}

// Non-member functions

bool context_extension(const char* key, ALCdevice* target) {
  bool result = false;
  if(key != nullptr) {
    ALCboolean ext_result = alcIsExtensionPresent(target, key);
    if(ext_result == ALC_TRUE) {
      result = true;
    }
  }

  return result;
}

int /*ALenum*/ enum_available(const char* key, ALCdevice* target) {
  ALenum enum_result = AL_INVALID;
  if(key != nullptr) {

    enum_result = alcGetEnumValue(target, key);
    return enum_result;
  }

  return enum_result;
}

void* context_function(ALCdevice* target, const ALCchar* fn) {
  return alcGetProcAddress(target, fn);
}

} // namespace audio
} // namespace nom
