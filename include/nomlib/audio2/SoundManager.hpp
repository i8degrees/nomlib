#pragma once

#include <string>
#include <map>
#include "nomlib/types.hpp"
#include "nomlib/audio2/Sound.hpp"

namespace nom {
namespace audio {

class Sound;

enum SoundTraits: nom::uint32
{
  SOUND_NONE = 0x00,
  SOUND_STREAM_OGG = 0x02,
  SOUND_STREAM_SOUND = 0x04
};

// Put in nom::SDLApp?
class SoundManager
{
  public:
    static void free_sounds();

    //static void add_sound(const std::string& n, uint32 state = SOUND_NONE);
    static void add_sound(const std::string& n, bool streaming = false);
    static Sound* find_sound(const std::string& name);
    static void remove_sound(const std::string& name);

  protected:
    SoundManager();
    ~SoundManager();

    static std::map<std::string, Sound*> sounds;
};

} // namespace audio
} // namespace nom
