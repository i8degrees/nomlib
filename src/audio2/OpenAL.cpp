#include "OpenAL.hpp"

// OpenAL Error Handling

ALenum
al_err(const std::string& func, const std::string& file, uint32_t line)
{
  ALenum error_code = alGetError();

  if(error_code != AL_NO_ERROR) {
    const char* err_cstr = nullptr;

    switch(error_code) {
      default: {
        err_cstr = "!! There is an undocumented error case !!";
      } break;

      case AL_NO_ERROR: {
        err_cstr = "AL_NO_ERROR: There is not currently an error";
      } break;

      case AL_INVALID_NAME: {
        err_cstr = "AL_INVALID_NAME: A bad name (ID) was passed";
      } break;

      case AL_INVALID_ENUM: {
        err_cstr = "AL_INVALID_ENUM: An unknown enum value was passed";
      } break;

      case AL_INVALID_VALUE: {
        err_cstr = "AL_INVALID_VALUE: An invalid value was passed";
      } break;

      case AL_INVALID_OPERATION: {
        err_cstr =
          "AL_INVALID_OPERATION: The requested operation is not valid";
      } break;

      case AL_OUT_OF_MEMORY: {
        err_cstr =
          "AL_OUT_OF_MEMORY: The requested operation resulted in OpenAL running out of memory";
      } break;
    } // end switch

    std::cerr << "NOM_LOG_CATEGORY_AUDIO: " << err_cstr << " at " << file <<
      " : " << line << " in " << func;
    return error_code;
  } // end if != AL_NO_ERROR
  return AL_NO_ERROR;
}

ALenum
alc_err(const std::string& func, const std::string& file, uint32_t line,
  ALCdevice* dev)
{
  ALenum error_code = alcGetError(dev);

  if(error_code != ALC_NO_ERROR) {
    const char* err_cstr = nullptr;

    switch(error_code) {
      default: {
        err_cstr = "!! There is an undocumented error case !!";
      } break;

      case ALC_NO_ERROR: {
        err_cstr = "AL_NO_ERROR: There is not currently an error";
      } break;

      case ALC_INVALID_DEVICE: {
        err_cstr = "ALC_INVALID_DEVICE: A bad device was given";
      } break;

      case ALC_INVALID_CONTEXT: {
        err_cstr = "ALC_INVALID_CONTEXT: A bad context was given";
      } break;

      case ALC_INVALID_ENUM: {
        err_cstr = "ALC_INVALID_ENUM: An unknown enum value was passed";
      } break;

      case ALC_INVALID_VALUE: {
        err_cstr = "ALC_INVALID_VALUE: An invalid value was passed";
      } break;

      case ALC_OUT_OF_MEMORY: {
        err_cstr =
          "ALC_OUT_OF_MEMORY: The requested operation resulted in OpenAL running out of memory";
      } break;
    } // end switch

    std::cerr << "NOM_LOG_CATEGORY_AUDIO: " << err_cstr << " at " << file <<
      " : " << line << " in " << func;
    return error_code;
  } // end if != ALC_NO_ERROR
  return ALC_NO_ERROR;
}

