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
#ifndef NOMLIB_SERIALIZERS_ICONFIG_FILE_HPP
#define NOMLIB_SERIALIZERS_ICONFIG_FILE_HPP

#include <memory>
#include <map>

#include "nomlib/config.hpp"
#include "nomlib/ptree/Value.hpp"

#include "nomlib/math/Point2.hpp"

// #define NOM_DEBUG_SERIALIZERS_ICONFIG_FILE

namespace nom {

// Forward declarations
class IValueDeserializer;
class IValueSerializer;

/// \brief Abstract base class for configuration file types.
///
/// \remarks http://www.gamedev.net/page/resources/_/technical/game-programming/configuration-and-tweaking-r3154
/// \remarks http://docs.wxwidgets.org/trunk/classwx_config_base.html
class IConfigFile
{
  public:
    typedef IConfigFile self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    typedef std::pair<std::string, Value> Pair;
    typedef std::map<std::string, Value> value_type;

    /// \brief Access to our (private) get method; returns value_type container.
    friend class JsonConfigFile;

    /// \brief Default constructor.
    IConfigFile( void );

    /// \brief Destructor.
    virtual ~IConfigFile( void );

    /// \remarks This function provides type-safe casting.
    virtual IValueDeserializer* deserializer( void ) const = 0;

    /// \remarks This function provides type-safe casting.
    virtual IValueSerializer* serializer( void ) const = 0;

    /// \brief Force an output (save) of the current configuration.
    virtual bool flush( void ) = 0;

    /// \brief Deserialize the configuration from the set file path.
    virtual bool load( void ) = 0;

    /// \brief Serialize the current configuration to the set file path.
    virtual bool save( void ) = 0;

    /// \brief Obtain the set file path.
    ///
    /// \remarks This is the file path used for I/O during a call to load or
    /// save.
    const std::string& filename( void ) const;

    /// \brief Check to see if an existing configuration key exists.
    bool exists( const std::string& key ) const;

    /// \returns The signed integer of the configuration key.
    int get_int( const std::string& key, int default_value ) const;

    /// \returns The unsigned integer of the configuration key.
    uint get_uint( const std::string& key, uint default_value ) const;

    /// \returns The double value of the configuration key.
    double get_double( const std::string& key, double default_value ) const;

    /// \returns The string of the configuration key.
    const std::string get_string( const std::string& key, const std::string& default_value ) const;

    /// \returns The boolean value of the configuration key.
    bool get_bool( const std::string& key, bool default_value ) const;

    /// \brief Write a value to the configuration, mapped by key.
    ///
    /// \remarks If the existing item's key exists, it will be overwritten by
    /// the new value.
    void set_property( const std::string& key, const Value& value );

    /// \brief Destroy a configuration item.
    ///
    /// \param key The configuration item, referenced by member key.
    bool erase( const std::string& key );

    /// \brief Set the file I/O path.
    ///
    /// \remarks The command to load the file is given after the filename has
    /// been set.
    void set_filename( const std::string& filename );

  private:
    /// \brief Internal getter method for the storage container.
    const value_type& get( void ) const;

    /// \brief File I/O path.
    std::string filename_;

    /// \brief The internal configuration storage map.
    value_type config_;
};

} // namespace nom

#endif // include guard defined
