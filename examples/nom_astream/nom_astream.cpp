
#include <iostream>
#include <vector>

#include "nomlib/audio2/audio.hpp"

using namespace std;
using namespace nom;
using namespace nom::audio;

#define MAX_COUNT 120 // 120s

std::vector<std::string> sounds = {
  "Resources/tests/audio/ALAudioTest/laura_ff1-s16-mono.ogg",
};

#include <chrono>

class NonBlockingTicker
{
  private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point last_tick_time;
    std::chrono::milliseconds interval;
  public:
    // Initialize with a tick interval (e.g., 500ms)
    explicit NonBlockingTicker(std::chrono::milliseconds tick_interval)
      : interval(tick_interval)
    {
      last_tick_time = Clock::now();
    }

    // Call this inside your main loop. Returns true if a tick occurred.
    bool update() {
      auto now = Clock::now();
      auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick_time);

      if(elapsed >= interval) {
        // Option A: Reset to current time (drops extra frame time)
        // last_tick_time = now;

        // Option B: Add interval (maintains strict timing cadence)
        last_tick_time += interval;
        return true;
      }

      return false;
    }
};

int main(int argc, char* argv[])
{
  NonBlockingTicker ticker(std::chrono::milliseconds(1000));

  if(nom::audio::init_audio(32) == false) {
    std::cerr << "Failed to init audio -- exiting!" << std::endl;
    return 1;
  }

  atexit(nom::audio::shutdown_audio);

  for(auto itr = sounds.begin(); itr != sounds.end(); ++itr) {
    auto name = *itr;
    bool streaming = true;
#if 0
    std::string ext = name.substr(name.length()-3, 3);
    if(ext == "ogg") {
      streaming = true;
    }
#endif
    SoundManager::add_sound(name, streaming);
  }

  Sound* media = SoundManager::find_sound("Resources/tests/audio/ALAudioTest/laura_ff1-s16-mono.ogg");
  NOM_ASSERT(media != nullptr && "media should NOT be nullptr");
  if(media != nullptr) {
    cout << "total_bytes():" << media->total_bytes() << endl;
    cout << "channels():" << media->channels() << endl;
    cout << "frequency():" << media->frequency() << endl;
    cout << "sample_count():" << media->sample_count() << endl;
    cout << "duration():" << media->duration() << endl;
    cout << "format():" << media->format() << endl;
    cout << "buffer():" << media->buffer() << endl;
    //cout << "samples3->volume():" << media->volume() << endl;
    cout << "bitrate():" << media->bitrate() << endl;
  }

  auto audio_handle = AudioDeviceLocator::instance();
  NOM_ASSERT(audio_handle);
  if(audio_handle == nullptr) {
    std::cerr << "Failed to find audio device... exiting now!" << std::endl;
    return 99;
  }

  audio_handle->set_master_volume(1.0f);

  using Clock = std::chrono::steady_clock;
  std::chrono::milliseconds interval;
  int64 counter = 0;

  if(media) {
    audio_handle->play_sound(media, 0.15f);
  }

  while(counter < MAX_COUNT) {
    Clock::time_point last_tick_time;
    auto delta = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(delta - last_tick_time);
    if(ticker.update()) {
      counter++;

      std::cout << "T " << counter << "\n"; // seconds
      std::cout << "E " << elapsed.count() << "\n"; // ns
      audio_handle->update(counter);
    } // end if
  } // end loop

  // using atexit makes the following unnecessary, but alas
  // nom::audio::shutdown_audio();

  return 0;
}
