
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio2/SoundManager.hpp"
#include "nomlib/audio2/Sound.hpp"
#include "nomlib/audio2/StreamSound.hpp"

#if defined(USE_LIBOGG)
  #include "nomlib/audio2/OggSound.hpp"
#endif

#include <map>

using namespace std;
using namespace nom;
// using namespace nom::priv;

using SoundIterator = std::map<std::string, nom::audio::Sound*>::iterator;

namespace nom {
namespace audio {

// static
std::map<std::string, Sound*> SoundManager::sounds;

// static
void SoundManager::add_sound(const std::string& name, bool streaming) {
  Sound *s = SoundManager::find_sound(name);

  if(!s) {
    //std::string extension = name.substr(name.length()-3,3);

    if(streaming == false) {
      s = new Sound();
      s->open(name);

      AL_CLEAR_ERR();
      AL_CHECK_ERR(alGenBuffers(1, &s->buffer()));
#if defined(VERBOSE)
      std::cout << "SoundManager::add_sound(" << s->buffer() <<
        ") - Creating a new Sound with " << "the name of " << name <<
      std::endl;
#endif

#if 0
#if defined(DEBUG) && defined(VERBOSE)
      auto buffer = s->data();
      for(auto idx = 0; idx != 32; ++idx) {
        std::cout << NOM_SCAST(int16*, buffer)[idx] << "\n";
      }
#endif
#endif

      AL_CLEAR_ERR();
      AL_CHECK_ERR(alBufferData(s->buffer(), s->format(), s->data(),
        s->total_bytes(), (ALsizei)s->frequency()));
    } else if(streaming == true) {

#if defined(USE_LIBOGG)
      OggSound* stream = new OggSound();
      stream->open(name);
#else
      StreamSound* stream = new StreamSound();
      stream->open(name);
#endif

#if defined(VERBOSE)
      std::cout << "SoundManager::add_sound(" << stream->buffer() <<
        ") - Creating a new streaming " <<
      "Sound with " << "the name of " << name << std::endl;
#endif
      s = stream;
    }

    sounds.insert(std::make_pair(name, s));
  } else {
#if defined(VERBOSE)
    std::cout << "SoundManager::add_sound() - The sound " << name <<
      " to be added already exists (cached)" << std::endl;
#endif
  }
}

// static
Sound* SoundManager::find_sound(const std::string& name) {
  SoundIterator s = sounds.find(name);
  return (s != sounds.end() ? s->second : NULL);
}

// static
void SoundManager::remove_sound(const std::string& name) {
  SoundIterator s = sounds.find(name);
  if(s != sounds.end()) {
#if defined(VERBOSE)
    std::cout << "Removing sound " << name << " from the cache..." <<
      std::endl;
#endif
    sounds.erase(s);
  }
}

// static
void SoundManager::free_sounds() {
  std::cout << "SoundManager::free_sounds()" << std::endl;

  for(SoundIterator i = sounds.begin(); i != sounds.end(); ++i) {
#if defined(DEBUG)
    std::cout << "Goodbye " << i->second->filename_ << std::endl;
#endif
    delete i->second;
  }
}

} // namespace audio
} // namespace nom
