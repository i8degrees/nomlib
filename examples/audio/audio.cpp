/******************************************************************************

    nomlib Audio Playback Demo

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/audio.hpp>
#include <nomlib/system.hpp>

class App
{
  public:
    App ( void )
    {
      //
    }
    ~App ( void )
    {
      //
    }

    nom::int32 Run ( void )
    {
      std::cout << "Device Name: " << this->dev.getDeviceName() << std::endl;

      return 0;
    }

  private:
    nom::OpenAL::AudioDevice dev;
    std::string dev_name;
};

int main ( int argc, char* argv[] )
{
  using namespace nom::OpenAL;
  App engine;
  AudioDevice::AudioDevice dev;
  std::string dev_name;
  std::string path = nom::OSXFS::getDirName ( argv[0] ) + "/";

  dev_name = dev.getDeviceName();

  std::cout << "Device Name: " << dev.getDeviceName() << std::endl;

  engine.Run();

  SoundFile snd;

  if ( argv[1] != nullptr )
  {
    snd.loadFromFile ( argv[1] );
  }
  else
  {
    snd.loadFromFile ( path + "data/cursor_wrong.wav" );
  }

  std::cout << "Sample Count: " << snd.getSampleCount() << std::endl;
  std::cout << "Channel Count: " << snd.getChannelCount() << std::endl;
  std::cout << "Sample Rate: " << snd.getSampleRate() << std::endl;

  return EXIT_SUCCESS;
}
