#include "nomlib/math/math_helpers.hpp"
#include "nomlib/audio2/ISoundSource.hpp"
#include "nomlib/audio2/Sound.hpp"

using namespace nom;
using namespace nom::audio;

namespace nom {
namespace audio {

ISoundSource::ISoundSource()
{
  NOM_LOG_TRACE(NOM);
  this->reset();
}

ISoundSource::~ISoundSource()
{
  NOM_LOG_TRACE(NOM);
  this->reset();
}

Sound* ISoundSource::sound()
{
  return this->sound_;
}

void ISoundSource::set_priority(SoundPriority p)
{
  this->priority_ = p;
}

SoundPriority ISoundSource::priority()
{
  return this->priority_;
}

void ISoundSource::set_volume(real32 gain)
{
  gain = nom::clamp_max<real32>(0.0f, gain);
  gain = nom::clamp_min<real32>(1.0f, gain);
  this->volume_ = gain;
  std::cout << "ISoundSource::set_volume(" << this->volume_ << ")" << std::endl;
}

nom::uint32 ISoundSource::state()
{
  return this->state_;
}

real32 ISoundSource::volume()
{
  return this->volume_;
}

void ISoundSource::set_looping(bool state)
{
  this->looping_= state;
}

bool ISoundSource::looping()
{
  return this->looping_;
}

void ISoundSource::set_radius(real32 value)
{
  this->radius_ = std::max(0.0f, value);
}

real32 ISoundSource::radius()
{
  return this->radius_;
}

real32 ISoundSource::pitch()
{
  return this->pitch_;
}

void ISoundSource::set_pitch(real32 value)
{
  this->pitch_ = value;
}

bool ISoundSource::global()
{
  return this->global_;
}

void ISoundSource::set_global(bool value)
{
  this->global_ = value;
}

real64 ISoundSource::time_left()
{
  return this->time_left_;
}

Point3f& ISoundSource::position()
{
  return this->position_;
}

void ISoundSource::set_position(const nom::Point3f& pos)
{
  this->position_ = pos;
}

void ISoundSource::set_position(real32 x, real32 y, real32 z)
{
  this->position_ = Point3f(x, y, z);
}

void ISoundSource::reset()
{
  this->state_ = 0; /*AL_STOPPED*/;
  this->priority_ = SOUND_PRIORITY_LOW;
  this->pitch_ = 1.0f;
  this->volume_ = 0.25f; // FIXME
  this->radius_ = 500.0f;
  this->time_left_ = 0.0f;
  this->looping_ = /*true*/ false;
  this->position_ = Point3f(0.0f, 0.0f, 0.0f);
  this->sound_ = nullptr;

  //Part 2
  this->global_ = false;
}

} // namespace audio
} // namespace nom
