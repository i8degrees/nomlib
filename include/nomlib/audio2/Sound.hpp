#pragma once

#include <string>
#include "nomlib/types.hpp"

namespace nom {
namespace audio {

// Forward declarations
class ISoundFileReader;

class ISound {
  public:
    ISound() {};
    virtual ~ISound() {}
    // TODO(JEFF): Change return type of bool to void?
    virtual bool open(const std::string& filename) = 0;

    // Not implemented
    virtual double stream(nom::uint32 buffer, nom::real64 pos) = 0;
    //virtual bool stream__(uint32 buffer, real64 pos) = 0;
  protected:
};

//class Sound: public ISound
class Sound
{
  public:
    // Default init ctr for fp_
    Sound();
    // init with fp_ and filename
    Sound(const std::string& name);
    virtual ~Sound();

    // getters for the data fields of each sound read in by init method
    /// \brief Identifier number for the implementation-defined buffer system
    nom::uint&         buffer() /*const*/;
    nom::int16*        data() /*const*/;
    nom::uint64        bitrate() const;

    /// \brief    Obtain the audio frequency in Hertz of this sound instance.
    nom::real32        frequency() const;

    /// \brief    Obtain the number of audio channels of this sound instance.
    int           channels() const;

    /// \brief    Obtain the total number of bytes for this sound instance.
    int           total_bytes() const;

    /// \brief    Obtain the audio channel format as expressed in
    /// AL_FORMAT_XXX.
    int           format() const;

    /// \brief    Obtain the total length of this sound instance.
    nom::real64        duration() const;

    /// \brief    Obtain the total sample count of this sound instance.
    nom::int64         sample_count() const;

    // Part 2 -- file chunk playback
    bool streaming() const;

    // TODO(JEFF): Change return type of bool to void?
    virtual bool open(const std::string& filename);

    // Not implemented
    virtual double stream(nom::uint32 buffer, nom::real64 pos);
    virtual bool stream__(nom::uint32 buffer, nom::real64 pos);

#if defined(DEBUG)
    std::string filename_;
#endif
  protected:
    // init with filename and fp_
    void init(const std::string& filename);
    // Init with sample data using fp_
    void init(nom::int16* samples, nom::uint32 format, int num_channels);

    nom::audio::ISoundFileReader* fp_; // sound file reader impl pointer
    nom::int16*    samples_; // samples in 16 bit (native)
    nom::int64     sample_count_; // total number of frames (samples)
    nom::uint      buffer_id_; // >= 0 && <= UINT_MAX
    nom::uint64    bitrate_; // bits per second
    nom::real32    frequency_; // sample rate (frequency)
    nom::real64    duration_; // time_left
    int       total_bytes_; // storage size on disk
    int       num_channels_; // MONO (1) || STEREO (2)
    nom::uint32    format_; // AUDIO_FORMAT_XXX
    bool      streaming_; // AL_STATIC || AL_STREAMING
  private:
    // Non-member functions
    void* allocate_samples(nom::size_type bytes, int num_channels, nom::uint32 fmt);
    nom::int16* allocate_samples(nom::size_type bytes, int channels);
};

//void* allocate_samples(size_type bytes, int num_channels, uint32 fmt);

} // namespace audio
} // namespace nom
