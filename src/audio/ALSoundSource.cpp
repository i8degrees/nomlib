
#include "nomlib/audio/ALSoundSource.hpp"
#include "nomlib/audio/AudioDeviceLocator.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"

using namespace nom;
using namespace nom::priv;

namespace nom {
namespace audio {

void ALSoundSource::reset()
{
  NOM_LOG_TRACE(NOM);

  super::reset();
  this->oalSource_ = nullptr;
}

ALSoundSource::ALSoundSource()
{
  NOM_LOG_TRACE(NOM);

  this->stream_pos_ = 0;
  for(uint32 i = 0; i < NUM_STREAM_BUFFERS; ++i) {
    this->stream_buffers_[i] = 0;
    this->stream_buffer_durations_[i] = 0.0;
  }

  this->oalSource_ = nullptr;
}

OALSource* ALSoundSource::source()
{
  return this->oalSource_;
}

ALSoundSource::ALSoundSource(Sound* s)
{
  this->reset();
  this->set_sound(s);
}

ALSoundSource::~ALSoundSource()
{
  NOM_LOG_TRACE(NOM);
  this->detach_source();
}

// static
bool ALSoundSource::compare(ISoundSource *lhs, ISoundSource* rhs)
{
  return (lhs->priority() > rhs->priority()) ? true : false;
}

// SoundNode::SetSound
void ALSoundSource::set_sound(Sound *s)
{
  NOM_LOG_TRACE(NOM);

  this->sound_ = s;
  this->detach_source();

  if(this->sound_) {
    this->time_left_ = this->sound_->duration();

    // Part 2:
    if(this->sound_->streaming()) {
      AL_CLEAR_ERR();
      AL_CHECK_ERR(alGenBuffers(NUM_STREAM_BUFFERS, this->stream_buffers_));
    } else {
      AL_CLEAR_ERR();
      AL_CHECK_ERR(alDeleteBuffers(NUM_STREAM_BUFFERS, this->stream_buffers_));
    }
  }
}

void ALSoundSource::attach_source(OALSource* s)
{
  NOM_LOG_TRACE(NOM);

  this->oalSource_ = s;

  if(!this->oalSource_) {
    return;
  }

  this->oalSource_->used = true;

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alSourceStop(this->oalSource_->source));

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_MAX_DISTANCE,
    radius()));

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_REFERENCE_DISTANCE,
      (radius() * 0.2f)));
#if 0
  // Rewind source position
  alSourceRewind(this->oalSource_->source);
#endif
  //if(this->time_left_ > 0) {
  if(this->sound_->streaming()) { // Part 2
    // this->stream_pos_ = this->time_left_;
    this->stream_pos_ = this->sound_->duration();
    this->time_left_ = this->sound_->duration();
    int numBuffered = 0;
    while(numBuffered < NUM_STREAM_BUFFERS) {
      // stream in more data
      real64 streamed =
        this->sound_->stream(this->stream_buffers_[numBuffered],
          this->stream_pos_);

      if(streamed) {
        this->stream_buffer_durations_[numBuffered] = streamed;
        ++numBuffered; // Another buffer filled
      } else {
        break; // No more buffers to fill
      }
    }

    AL_CLEAR_ERR();
    AL_CHECK_ERR(alSourceQueueBuffers(this->oalSource_->source, numBuffered,
      &stream_buffers_[0]));

    // Safety fallback: ensure playback resumes if the source was previously
    // starved and stopped.
    ALint state;
    AL_CLEAR_ERR();
    AL_CHECK_ERR(alGetSourcei(this->oalSource_->source, AL_SOURCE_STATE, &state));
    if(state == AL_STOPPED) {
      alSourcePlay(this->oalSource_->source);
    }
#if 0
    // Safety fallback; ensure that playback resumes if starved
    // (and stopped).
    ALint state;
    AL_CLEAR_ERR();
    AL_CHECK_ERR(alGetSourcei(this->oalSource_->source, AL_SOURCE_STATE, &state));
    if(state != AL_PLAYING) {
      alSourcePlay(this->oalSource_->source);
    }
#endif
  } else {
#if defined(DEBUG)
    std::cout << "ALSoundSource::attach_source(" << this->sound_->filename_
      << ", " << this->sound_->buffer() << ")" << std::endl;
#endif
    // Not a streaming sound type
    AL_CLEAR_ERR();
    AL_CHECK_ERR(alSourcei(this->oalSource_->source, AL_BUFFER,
      this->sound_->buffer()));

    this->time_left_ = this->sound_->duration();
    //this->time_left_ =
      //(this->sound_->duration() - (this->time_left() / 1000.0f));

    // last
    //this->time_left_ =
      //(this->sound_->duration()) - (this->time_left() / 1000.0f);

    //auto time_left = ((this->sound_->duration()) -
      //(this->time_left_));
    //auto time_left = ((this->sound_->duration() / 1000.0f) -
      //(this->time_left_ / 1000.0f));

#if defined(VERBOSE)
    auto time_left = this->time_left_;
    std::cout << "Attaching source(" << this->oalSource_->source << ") " <<
      "Time remaining.. " << time_left << std::endl;
#endif

    AL_CLEAR_ERR();
    AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_SEC_OFFSET,
      this->time_left_));

    AL_CLEAR_ERR();
    AL_CHECK_ERR(alSourcePlay(this->oalSource_->source));
  } // end if non streaming audio type

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alSourcePlay(this->oalSource_->source));
}

void ALSoundSource::detach_source()
{
  NOM_LOG_TRACE(NOM);
  if(!this->oalSource_) {
    return;
  }

#if defined(DEBUG)
  std::cout << "ALSoundSource::detach_source(" <<
    this->sound_->buffer() << ") - " << this->sound_->filename_ << ")" <<
  std::endl;
#endif
  this->oalSource_->used = false;

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_GAIN, 0.0f));

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alSourceStop(this->oalSource_->source));

  AL_CLEAR_ERR();
  AL_CHECK_ERR(alSourcei(this->oalSource_->source, AL_BUFFER, 0));

  if(this->sound_ && this->sound_->streaming()) { // Part 2
    int numProcessed = 0;
    ALuint tempBuffer;

    AL_CLEAR_ERR();
    AL_CHECK_ERR(alGetSourcei(this->oalSource_->source, AL_BUFFERS_PROCESSED,
      &numProcessed));
    while(numProcessed--) {
      AL_CLEAR_ERR();
      AL_CHECK_ERR(alSourceUnqueueBuffers(this->oalSource_->source, 1,
        &tempBuffer));
    }

    // Ensure that we free the buffers that were allocated for streaming...
    AL_CLEAR_ERR();
    AL_CHECK_ERR(alDeleteBuffers(NUM_STREAM_BUFFERS, this->stream_buffers_));
  }

  this->oalSource_ = nullptr;
}

void ALSoundSource::update_state(real32 delta)
{
  std::cout << "ALSoundSource::update_state(" << delta << ")" << std::endl;

  if(this->sound_) {
    // Streaming sources track remaining time via stream_pos_; non-streaming
    // sources count down by elapsed wall-clock time.
    if(this->sound_->streaming() == false) {
      this->time_left_ -= (delta * this->pitch());

      if(this->looping_) {
        while(this->time_left_ < 0) {
          this->time_left_ += this->sound_->duration();
        }
      }
    }

    if(this->oalSource_) {
#if defined(DEBUG)
      std::cout << "ALSoundSource::update_state(" << delta << ") - " <<
        this->sound_->filename_ << " - volume(" << this->volume() << ")"
      << std::endl;
#endif
      AL_CLEAR_ERR();
      AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_GAIN,
        this->volume()));

      AL_CLEAR_ERR();
      AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_PITCH,
        this->pitch()));

      AL_CLEAR_ERR();
      AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_MAX_DISTANCE,
        this->radius()));

      AL_CLEAR_ERR();
      AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_REFERENCE_DISTANCE,
        this->radius() * 0.2f));

      // Part 2
      Point3f position = {0.0f, 0.0f, 0.0f};

      if(this->global_) {
        // FIXME
        //position = this->impl_->listener();
        //position = SoundSystem::GetSoundSystem()->GetListener()->GetWorldTransform().GetPositionVector();
      } else {
        // FIXME
        //position = GetWorldTransform().GetPositionVector();
      }

      AL_CLEAR_ERR();
      AL_CHECK_ERR(alSourcefv(this->oalSource_->source, AL_POSITION,
        (real32*)&position));

      if(this->sound_->streaming()) {
        int numProcessed;
        AL_CLEAR_ERR();
        AL_CHECK_ERR(alGetSourcei(this->oalSource_->source,
          AL_BUFFERS_PROCESSED, &numProcessed));

        AL_CLEAR_ERR();
        AL_CHECK_ERR(alSourcei(this->oalSource_->source, AL_LOOPING, 0));

        // The && prevents clipping at the end of sounds!
        while(numProcessed--) {
          ALuint freeBuffer;

          AL_CLEAR_ERR();
          AL_CHECK_ERR(alSourceUnqueueBuffers(this->oalSource_->source, 1,
            &freeBuffer));

          // Find the slot that holds this buffer's queued duration and
          // subtract it from the remaining stream time.
          int slot = -1;
          for(int s = 0; s < NUM_STREAM_BUFFERS; ++s) {
            if(this->stream_buffers_[s] == freeBuffer) {
              slot = s;
              break;
            }
          }
          if(slot >= 0) {
            this->stream_pos_ -= this->stream_buffer_durations_[slot];
          }

          real64 streamed =
            this->sound_->stream(freeBuffer, this->stream_pos_);
          if(streamed > 0.0) {
            this->stream_buffer_durations_[slot] = streamed;

#if defined(DEBUG)
            std::cout << "ALSoundSource::update_state(" << delta << ") - " <<
              this->sound_->filename_ << " at spos: " << this->stream_pos_ <<
            std::endl;
#endif
            AL_CLEAR_ERR();
            AL_CHECK_ERR(alSourceQueueBuffers(this->oalSource_->source, 1,
              &freeBuffer));
          }

          if(this->stream_pos_ < 0 && this->looping_) {
            this->stream_pos_ += this->sound_->duration();
          }

          // !! Potential fix for streaming sounds being ejected early from
          // !! the ALAudioRenderer::update loop
          // if(this->stream_pos_ >= this->sound_->duration()) {
            // alSourceRewind(this->oalSource_->source);
          // }
        }

        // Floating-point rounding can leave stream_pos_ as a tiny positive
        // value once the file is fully consumed. Clamp it so cleanup triggers.
        if(this->stream_pos_ < 0.001 && this->stream_pos_ > -0.001) {
          this->stream_pos_ = 0.0;
        }

        this->time_left_ = this->stream_pos_;

        // If the source starved and stopped, resume playback now that buffers
        // have been refilled.
        ALint state;
        AL_CLEAR_ERR();
        AL_CHECK_ERR(alGetSourcei(this->oalSource_->source, AL_SOURCE_STATE, &state));
        if(state == AL_STOPPED && this->stream_pos_ > 0.0) {
          alSourcePlay(this->oalSource_->source);
        }
      } else {
        AL_CLEAR_ERR();
        AL_CHECK_ERR(alSourcei(this->oalSource_->source, AL_LOOPING,
          this->looping_ ? 1 : 0));
      }
    }
  }
}

void ALSoundSource::update(real32 delta)
{
  // NOM_LOG_TRACE(NOM);
  auto handle = AudioDeviceLocator::instance();
  if(handle != nullptr) {
    AudioDeviceLocator::instance()->add_source(this);
  }
  //IEntityObject::update(delta);
}

void ALSoundSource::set_volume(real32 gain)
{
  super::set_volume(gain);

  if(this->oalSource_ != nullptr) {
    AL_CLEAR_ERR();
    AL_CHECK_ERR(alSourcef(this->oalSource_->source, AL_GAIN,
      this->volume()));
  }
}

} // namespace audio
} // namespace nom
