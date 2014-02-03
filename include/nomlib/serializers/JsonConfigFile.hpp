/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_SERIALIZERS_JSON_CONFIG_FILE_HPP
#define NOMLIB_SERIALIZERS_JSON_CONFIG_FILE_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/serializers/IConfigFile.hpp"

namespace nom {

// Forward declarations
class IValueDeserializer;
class IValueSerializer;

/// \brief JSON Configuration file interface
class JsonConfigFile: public IConfigFile
{
  public:
    typedef JsonConfigFile self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    /// \brief Default constructor; initialize the loader and saver interfaces
    /// necessary for JSON I/O.
    JsonConfigFile( void );

    /// \brief Destructor.
    ~JsonConfigFile( void );

    /// \brief Re-implements IConfigFile::flush.
    bool flush( void );

    /// \brief Re-implements IConfigFile::deserializer.
    IValueDeserializer* deserializer( void ) const;

    /// \brief Re-implements IConfigFile::serializer.
    IValueSerializer* serializer( void ) const;

    /// \brief Re-implements IConfigFile::load.
    bool load( void );

    /// \brief Re-implements IConfigFile::save.
    ///
    /// \todo Do not output JSON when the object size of a nom::Value is zero.
    bool save( void );

  private:
    /// \brief Internal helper method for initializing the object state.
    bool initialize( void );

    /// \brief Set the configuration loader.
    ///
    /// \param fp An object deriving from nom::IValueDeserializer.
    ///
    /// \remarks Internal functionality of this class is routed through the
    /// deserializer set here.
    void set_deserializer( IValueDeserializer* fp );

    /// \brief Set the configuration saver.
    ///
    /// \param fp An object deriving from nom::IValueSerializer.
    ///
    /// \remarks Internal functionality of this class is routed through the
    /// serializer set here.
    void set_serializer( IValueSerializer* fp );

    /// \brief Object pointer to an object deriving from nom::IValueDeserializer.
    std::unique_ptr<IValueDeserializer> deserializer_;

    /// \brief Object pointer to an object deriving from nom::IValueSerializer.
    std::unique_ptr<IValueSerializer> serializer_;
};

} // namespace nom

#endif // include guard defined
