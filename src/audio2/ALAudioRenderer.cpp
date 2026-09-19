
#include "nomlib/math/math_helpers.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/types.hpp"

#include "nomlib/audio2/ALAudioRenderer.hpp"
#include "nomlib/audio2/IAudioDevice.hpp" // IAudioContext, IAudioRenderer
#include "nomlib/audio2/ALSoundSource.hpp" // ISoundSource, ALSoundSource
#include "nomlib/audio2/Sound.hpp"

using namespace nom;
using namespace nom::audio;
//using namespace std;

namespace nom {
namespace audio {

ALAudioRenderer::ALAudioRenderer()
{
  NOM_LOG_TRACE(NOM);

  this->ctx_ = nullptr;
  // this->listener_ =

  // FIXME(JEFF): Use a pre-defined constant value here that is reasonable,
  // i.e.: 64 or so?
  this->initialize(32);
}

ALAudioRenderer::ALAudioRenderer(uint32 channels)
{
  this->initialize(channels);
}

ALAudioRenderer::~ALAudioRenderer()
{
  NOM_LOG_TRACE(NOM);
  this->release();

  NOM_ASSERT(this->ctx_ != nullptr);

  if(this->ctx_) {
    this->ctx_->release();
  }

  AL_CLEAR_ERR();
  ALboolean result = alcCloseDevice(this->impl_);
  if(result == AL_FALSE) {
    std::cerr << "Failed to close audio device. " <<
      "Either a context is still open, or buffers are still in use." <<
    std::endl;
  } else {
    std::cout << "Successfully closed audio device." << std::endl;
  }

  this->impl_ = nullptr;
}

void ALAudioRenderer::release()
{
  for(SoundSourceIterator i =
    this->temporary_emitters_.begin(); i != this->temporary_emitters_.end();
    ++i)
  {
    delete (*i);
  }

  for(std::vector<OALSource*>::iterator i = this->sources_.begin();
    i != this->sources_.end(); ++i)
  {
    AL_CLEAR_ERR();
    AL_CHECK_ERR(alDeleteSources(1, &(*i)->source));
    delete (*i);
  }
}

void ALAudioRenderer::set_master_volume(real32 gain)
{
  NOM_ASSERT(this->ctx_ != nullptr);
  if(this->ctx_ == nullptr) {
    std::cerr << "ERROR: Audio context is NULL (not initialized)." <<
      std::endl;
    return;
  }

  if(this->ctx_) {
    this->ctx_->set_master_volume(gain);
  }
}

// Public scope -- member methods

OALSource* ALAudioRenderer::source()
{
  for(std::vector<OALSource*>::iterator i = sources_.begin();
    i != sources_.end(); ++i)
  {
    OALSource* s = *i;
    if(!s->used) {
      return s;
    }
  }

  return nullptr;
}

void ALAudioRenderer::add_source(ISoundSource* s)
{
  this->emitters_.push_back(s);
}

void ALAudioRenderer::play_sound(Sound* s)
{
  ISoundSource* n = new ALSoundSource();
  if(n == nullptr) {
    // ERROR: handle out of memory
    std::cerr << "CRITICAL: Failed to allocate memory for audio source!" <<
      std::endl;
    return;
  }

  n->set_looping(false);
  n->set_global(false);
  n->set_sound(s);
  this->temporary_emitters_.push_back(n);
}

void ALAudioRenderer::play_sound(Sound* s, const nom::Point3f& pos)
{
  ISoundSource* n = new ALSoundSource();
  if(n == nullptr) {
    // ERROR: handle out of memory
    std::cerr << "CRITICAL: Failed to allocate memory for audio source!" <<
      std::endl;
    return;
  }

  n->set_looping(false);
  // n->set_position(Matrix4::Translation(pos));
  n->set_position(pos);
  n->set_sound(s);
  this->temporary_emitters_.push_back(n);
}

void ALAudioRenderer::play_sound(Sound* s, SoundPriority p)
{
  ISoundSource* n = new ALSoundSource();
  if(n == nullptr) {
    // ERROR: handle out of memory
    std::cerr << "CRITICAL: Failed to allocate memory for audio source!" <<
      std::endl;
    return;
  }

  n->set_looping(false);
  n->set_sound(s);
  n->set_global(true);
  n->set_priority(p);
  this->temporary_emitters_.push_back(n);
}

void ALAudioRenderer::play_sound(Sound* s, real32 gain)
{
  ISoundSource* n = new ALSoundSource();
  if(n == nullptr) {
    // ERROR: handle out of memory
    std::cerr << "CRITICAL: Failed to allocate memory for audio source!" <<
      std::endl;
    return;
  }

  n->set_looping(false);
  n->set_sound(s);
  n->set_global(false); // ???
  //n->set_priority(p); // ???
  gain = nom::clamp_max<real32>(0.0f, gain);
  gain = nom::clamp_min<real32>(1.0f, gain);
  n->set_volume(gain);

  this->temporary_emitters_.push_back(n);
}

void ALAudioRenderer::update(real32 delta)
{
  std::cout << "ALAudioRenderer::update(" << delta << ")" << std::endl;

  this->update_listener();

  // Update temporary emitters
  for(SoundSourceIterator i =
    this->temporary_emitters_.begin(); i != this->temporary_emitters_.end(); )
  {
    //ALint state;
    //alSourcei( (*i)->source(), AL_SOURCE_STATE, &state);
    //if( ( (*i)->time_left() < 0.0f) && !(*i)->looping()) {
    // if( ((*i)->state_ - (*i)->time_left()) < 0.0f && !(*i)->looping()) {
    if((*i)->state() == 1 & (*i)->time_left() < 0.0f && !(*i)->looping()) {
      // FIXME(JEFF): The remaining time from the sound source is not right,
      // due to the fact that the time remaining calculation is being updated
      // every frame from the moment that the ALSoundSource is attached.
      //
      // Instead, it needs to be from the moment that ???
      std::cout << "ALAudioRenderer::update(" << delta << ") - time left " <<
        (*i)->time_left() << std::endl;
      delete (*i);
      i = this->temporary_emitters_.erase(i);
    } else {
      // ALSoundSource::update(delta)
      (*i)->update(delta);
      ++i;
    }
  }

  // Update values for every node, whether in range or not
  for(SoundSourceIterator i = this->emitters_.begin();
    i != this->emitters_.end(); ++i)
  {
    (*i)->update_state(delta);
  }

  // First off, remove nodes that are too far away
  //this->cull_nodes();

  // Then sort by priority
  if(this->emitters_.size() > this->sources_.size()) {
    std::sort(this->emitters_.begin(), this->emitters_.end(),
      ALSoundSource::compare);

    // Detach sources from nodes that won't be covered this frame
    this->detach_sources(this->emitters_.begin() + (this->sources_.size()+1),
      this->emitters_.end());

    // And attach sources to nodes that WILL be covered this frame
    this->attach_sources(this->emitters_.begin(), this->emitters_.begin() +
      (this->sources_.size()));
  } else {
    // And attach sources to nodes that WILL be covered this frame
    this->attach_sources(this->emitters_.begin(), this->emitters_.end());
  }

  // We're done for the frame! empty the emitters list
  this->emitters_.clear();
}

#if 0
void ALAudioRenderer::render(/*const RenderWindow& target*/)
{
}
#endif

// Private scope -- class methods

void ALAudioRenderer::cull_nodes() {
  for(SoundSourceIterator i = this->emitters_.begin();
    i != this->emitters_.end();)
  {
    real32 length;

    if((*i)->global()) {
      length = 0.0f;
    } else {
      length = 0.0f;
#if defined(FIXME)
      length = (this->listener_->GetWorldTransform().GetPositionVector() -
      (*i)->GetWorldTransform().GetPositionVector()).Length();
#endif
    }

    if(length > (*i)->radius() || !(*i)->sound() || (*i)->time_left() < 0.0f) {
      // Important!
      (*i)->detach_source();
      i = this->emitters_.erase(i);
    } else{
      ++i;
    }
  }
}

void ALAudioRenderer::update_listener()
{
#if defined(FIXME)
  Matrix4 worldMat = listener_->GetWorldTransform();
  Vector3 worldPos = worldMat.GetPositionVector();

  Point3f dirup[2];
  //forward
  dirup[0].x = -worldMat.values[2];
  dirup[0].y = -worldMat.values[6];
  dirup[0].z = -worldMat.values[10];
  //Up
  dirup[1].x = worldMat.values[1];
  dirup[1].y = worldMat.values[5];
  dirup[1].z = worldMat.values[9];

  alListenerfv(AL_POSITION,(real32*)&worldPos);
  alListenerfv(AL_ORIENTATION,(float*)&dirup);
#endif
}

// Private scope -- class methods

void ALAudioRenderer::detach_sources(SoundSourceIterator from,
  SoundSourceIterator to)
{
  for(SoundSourceIterator i = from; i != to; ++i) {
    (*i)->detach_source();
  }
}

void ALAudioRenderer::attach_sources(SoundSourceIterator from,
  SoundSourceIterator to)
{
  for(SoundSourceIterator i = from; i != to; ++i) {
    // Don't attach a new source if we already have one!
    if(!(*i)->source()) {
      (*i)->attach_source(this->source());
    }
  }
}

// Non-member functions

bool extension_available(const char* key) {
  bool result = false;
  if(key == nullptr) {
    return result; // FALSE
  }

  ALboolean ext_result = alIsExtensionPresent(key);
  if(ext_result == AL_TRUE) {
    result = true;
  }

  return result;
}

int /*ALenum*/ enum_available(const char* key) {
  int result = 0;
  ALenum enum_result = AL_NONE;
  if(key != nullptr) {
    enum_result = alGetEnumValue(key);

    // IMPORTANT(JEFF): Fix a bug regarding Apple's OpenAL return value of
    // negative one (-1).
    if(enum_result == AL_INVALID) {
      enum_result = 0;
    }

    result = enum_result;
  }

  return result;
}

void* device_function(const ALCchar* fn) {
  return alGetProcAddress(fn);
}

// Convert nomlib enum to OpenAL format equivalent
// ?? Rename to openal_format
ALenum channel_format(uint32 num_channels, AudioFormat channel_format) {
  ALenum format = 0;
  auto channel_count = num_channels;

  // FIXME(jeff): I'm unable to test 8-bit audio formats yet, due to a lacking
  // implementation of data handling within nom::SoundFileReader. We are unable
  // to interpret the 8-bit data natively using libsndfile.
  if(channel_format == AUDIO_FORMAT_U8 || channel_format == AUDIO_FORMAT_S8) {
    // channel_format = AUDIO_FORMAT_S16;
  }

  switch(channel_format) {
    default:
    case AUDIO_FORMAT_UNKNOWN: {
      return format;
    } break;

    case AUDIO_FORMAT_S8:
    case AUDIO_FORMAT_U8: {
      if(channel_count == 1) {
        format = enum_available("AL_FORMAT_MONO8");
      } else if(channel_count == 2) {
        format = enum_available("AL_FORMAT_STEREO8");
      }
    } break;

    case AUDIO_FORMAT_S16: {
      if(channel_count == 1) {
        format = enum_available("AL_FORMAT_NONO16");
      } else if(channel_count == 2) {
        format = enum_available("AL_FORMAT_STEREO16");
      }
    } break;

    case AUDIO_FORMAT_S24: {
      if(channel_count == 1) {
        format = enum_available("AL_FORMAT_MONO24");
      } else if(channel_count == 2) {
        format = enum_available("AL_FORMAT_STEREO24");
      }
    } break;

    case AUDIO_FORMAT_S32: {
      if(channel_count == 1) {
        format = enum_available("AL_FORMAT_MONO32");
      } else if(channel_count == 2) {
        format = enum_available("AL_FORMAT_STEREO32");
      }
    } break;

    case AUDIO_FORMAT_R32: {
      if(channel_count == 1) {
        format = enum_available("AL_FORMAT_MONO_FLOAT32");
      } else if(channel_count == 2) {
        format = enum_available("AL_FORMAT_STEREO_FLOAT32");
      }
    } break;

    case AUDIO_FORMAT_R64: {
      if(channel_count == 1) {
        format = enum_available("AL_FORMAT_MONO_DOUBLE_EXT");
      } else if(channel_count == 2) {
        format = enum_available("AL_FORMAT_STEREO_DOUBLE_EXT");
      }


    } break;
  }

  // NOM_ASSERT(channel_format != NOM_UINT32_MAX);

  // NOTE(jeff): Apple's OpenAL implementation returns negative one (-1), as
  // opposed to zero (0), when an unknown enumeration has been given.
  //
  // This is a last ditch efforts to offer a valid channel format for OpenAL
  // processing
  if(format == AL_INVALID || format == AL_NONE) {
    format = enum_available("AL_FORMAT_MONO16");
  }

  return format;
}

// Private scope -- class methods

// void ALAudioRenderer::initialize(uint32 channels, uint32 audio_caps = 0)
void ALAudioRenderer::initialize(uint32 channels)
{
  std::cout << "ALAudioRenderer::initialize(" << channels << ")" << std::endl;

  // real32 matrices[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  // this->listener_ = new Matrix4(matrices);
  // this->listener_ = new Point4f(matrices);

  // outputs all OAL devices
  std::cout << "Found the following devices: " <<
    alcGetString(nullptr, ALC_DEVICE_SPECIFIER) << std::endl;

  // Open the 'best' device
  this->impl_ = alcOpenDevice(nullptr);

  NOM_ASSERT(this->impl_ != nullptr);
  if(this->impl_ == nullptr) {
    std::cerr << "Failed to open a valid audio device handle!" << std::endl;
    return;
  }

  this->ctx_ = new ALAudioContext(this->impl_);

  NOM_ASSERT(this->ctx_ != nullptr);
  if(!this->ctx_) {
    std::cerr << "CRITICAL: Failed to allocate memory for audio context!" <<
      std::endl;
    return;
  }

  this->ctx_->set_master_volume(1.0f);

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED));

  for(uint32 i = 0; i < channels; ++i) {
    uint32 source;
    ALint error;

    error = AL_CLEAR_ERR();
    AL_CHECK_ERR(alGenSources(1, &source));

    if(error == AL_NO_ERROR) {
      this->sources_.push_back(new OALSource(source));
    } else {
      break;
    }
  }

#if defined(VERBOSE)
  std::cout << "ALAudioRenderer::initialize(" << this->sources_.size() <<
    ")" << " channels allocated" << std::endl;
#endif
}

} // namespace audio
} // namespace nom
