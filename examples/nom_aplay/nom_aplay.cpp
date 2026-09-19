
#include <iostream>
#include <vector>

#include "nomlib/audio2/audio.hpp"

using namespace std;
using namespace nom;
using namespace nom::audio;

#define MAX_COUNT 16 // seconds

std::vector<std::string> sounds = {
  "Resources/tests/audio/ALAudioTest/4s.wav",
  "Resources/tests/audio/ALAudioTest/audio-channel-front-center.wav",
  "Resources/tests/audio/ALAudioTest/audio-channel-rear-center.wav",
  // "Resources/tests/audio/ALAudioTest/laura_ff1-s16-mono.ogg",
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
    bool streaming = false;

    std::string ext = name.substr(name.length()-3, 3);
    if(ext == "ogg") {
      streaming = true;
    }

    SoundManager::add_sound(name, streaming);
  }

  cout << "Hello, there!" << endl;

  Sound* sample_null = SoundManager::find_sound("sample1.wav");
  NOM_ASSERT(sample_null == nullptr && "sample_null should be nullptr");
  Sound* sample1 = SoundManager::find_sound("Resources/tests/audio/ALAudioTest/4s.wav");
  //NOM_ASSERT(sample1 != nullptr && "sample1 should NOT be nullptr");
  if(sample1 != nullptr) {
    cout << "samples1->total_bytes():" << sample1->total_bytes() << endl;
    cout << "samples1->channels():" << sample1->channels() << endl;
    cout << "samples1->frequency():" << sample1->frequency() << endl;
    cout << "samples1->sample_count():" << sample1->sample_count() << endl;
    cout << "samples1->duration():" << sample1->duration() << endl;
    cout << "samples1->format():" << sample1->format() << endl;
    cout << "samples1->buffer():" << sample1->buffer() << endl;
    cout << "samples1->bitrate():" << sample1->bitrate() << endl;
    //cout << "samples1->volume():" << sample1->volume() << endl;
  }

  Sound* sample2 = SoundManager::find_sound("Resources/tests/audio/ALAudioTest/audio-channel-rear-center.wav");
  //NOM_ASSERT(sample2 != nullptr && "sample2 should NOT be nullptr");
  if(sample2 != nullptr) {
    cout << "samples2->total_bytes():" << sample2->total_bytes() << endl;
    cout << "samples2->channels():" << sample2->channels() << endl;
    cout << "samples2->frequency():" << sample2->frequency() << endl;
    cout << "samples2->sample_count():" << sample2->sample_count() << endl;
    cout << "samples2->duration():" << sample2->duration() << endl;
    cout << "samples2->format():" << sample2->format() << endl;
    cout << "samples2->buffer():" << sample2->buffer() << endl;
    //cout << "samples2->volume():" << sample2->volume() << endl;
    cout << "samples2->bitrate():" << sample2->bitrate() << endl;
  }

  //Sound* sample3 = SoundManager::find_sound("Resources/tests/audio/ALAudioTest/laura_ff1-s16-mono.15s.ogg");
  //NOM_ASSERT(sample3 != nullptr && "sample3 should NOT be nullptr");
  // Sound* sample3 = nullptr;
  Sound* sample3 = SoundManager::find_sound("Resources/tests/audio/ALAudioTest/laura_ff1-s16-mono.ogg");
  //NOM_ASSERT(sample3 != nullptr && "sample3 should NOT be nullptr");
  if(sample3 != nullptr) {
    cout << "samples3->total_bytes():" << sample3->total_bytes() << endl;
    cout << "samples3->channels():" << sample3->channels() << endl;
    cout << "samples3->frequency():" << sample3->frequency() << endl;
    cout << "samples3->sample_count():" << sample3->sample_count() << endl;
    cout << "samples3->duration():" << sample3->duration() << endl;
    cout << "samples3->format():" << sample3->format() << endl;
    cout << "samples3->buffer():" << sample3->buffer() << endl;
    //cout << "samples3->volume():" << sample3->volume() << endl;
    cout << "samples3->bitrate():" << sample3->bitrate() << endl;
  }

  Sound* sample4 = SoundManager::find_sound("Resources/tests/audio/ALAudioTest/laura_ff1-s16-mono.ogg");
  //NOM_ASSERT(sample3 != nullptr && "sample3 should NOT be nullptr");
  if(sample4 != nullptr) {
    cout << "sample4->total_bytes():" << sample4->total_bytes() << endl;
    cout << "sample4->channels():" << sample4->channels() << endl;
    cout << "sample4->frequency():" << sample4->frequency() << endl;
    cout << "sample4->sample_count():" << sample4->sample_count() << endl;
    cout << "sample4->duration():" << sample4->duration() << endl;
    cout << "sample4->format():" << sample4->format() << endl;
    cout << "sample4->buffer():" << sample4->buffer() << endl;
    //cout << "sample4->volume():" << sample4->volume() << endl;
    cout << "sample4->bitrate():" << sample4->bitrate() << endl;
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
  while(counter < MAX_COUNT) {
    Clock::time_point last_tick_time;
    auto delta = Clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(delta - last_tick_time);
    if(ticker.update()) {
      counter++;

      // Switch on count (1000ms = 1 count)
      switch(counter) {
        default: {} break;

        case 2: {
          if(sample1) {
            audio_handle->play_sound(sample1, 1.0f);
          }
        } break;

        case 4: {
          if(sample2) {
            audio_handle->play_sound(sample2, 0.15f);
          }
        } break;

        // End at 24s
        case 24: {
          exit(0);
        } break;
      } // end switch counter

      std::cout << "T " << counter << "\n"; // seconds
      std::cout << "E " << elapsed.count() << "\n"; // ns
      audio_handle->update(counter);
    } // end if
  } // end loop

  // using atexit makes the following unnecessary, but alas
  // nom::audio::shutdown_audio();

  return 0;
}
