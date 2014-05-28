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
#ifndef NOMLIB_SYSTEM_RESOURCE_CACHE_HPP
#define NOMLIB_SYSTEM_RESOURCE_CACHE_HPP

#include <string>
#include <map>
#include <memory>
#include <functional>

#include "nomlib/config.hpp"
#include "nomlib/system/File.hpp"
#include "nomlib/system/Path.hpp"
#include "nomlib/system/ResourceFile.hpp"

namespace nom {

/// \brief Resources management
template <typename ResourceType>
class ResourceCache
{
  public:
    typedef ResourceCache self_type;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

    typedef std::function<void( const ResourceFile&, ResourceType& )> callback_type;

    /// \brief Default constructor.
    ResourceCache( void )
    {
      // NOM_LOG_TRACE( NOM );
    }

    /// \brief Destructor -- free all of the stored resources.
    ~ResourceCache( void )
    {
      // NOM_LOG_TRACE( NOM );

      // Goodbye cruel world!
      this->resources_.clear();
    }

    /// \brief Get the total number of stored resources.
    size_t size( void ) const
    {
      return this->resources_.size();
    }

    /// \brief Search the cache for a resource.
    ///
    /// \param key The resource name for lookup.
    ///
    /// \returns The ResourceFile object reference on success, or
    /// ResourceFile::null on failure.
    const ResourceFile& find_resource( const std::string& key )
    {
      auto res = this->resources_.find( ResourceFile( key ) );

      if( res != this->resources_.end() )
      {
        return res->first;
      }

      return ResourceFile::null;
    }

    /// \brief Set an optional callback to use in creation of the resource
    /// before it is inserted into the cache.
    void set_resource_handler( const callback_type& delegate )
    {
      this->callback_ = delegate;
    }

    /// \brief Add a file-based resource object.
    ///
    /// \remarks The resource object is not instantiated until a call to
    /// ::load_resource is made.
    ///
    /// \note An attempt to append an existing resource will be discarded.
    bool append_resource( const ResourceFile& res )
    {
      File fp;
      ResourceFile member( res.name() );

      auto itr = this->resources_.find( member );

      // Duplicate resource entry; reject
      if( itr != this->resources_.end() )
      {
        // NOM_LOG_ERR( NOM, "Could not add resource: " + res.name() + " already exists." );
        return false;
      }

      if( res.exists() == false )
      {
        NOM_LOG_ERR( NOM, "Could not add resource: " + res.name() + " does not exist at " + res.path() );
        return false;
      }

      // Create the resource before insertion
      ResourceType res_type;

      // Use the load handler callback helper for resource initialization, if
      // it has been set.
      if( this->callback_ != nullptr )
      {
        this->callback_( res, res_type );
      }

      this->resources_.insert( { res, std::make_shared<ResourceType>( ResourceType( res_type ) ) } );

      return true;
    }

    /// \brief Fully initialize a resource.
    ///
    /// \returns A non-owned raw pointer to the fully initialized (loaded)
    /// resource on success, or NULL on failure.
    ///
    /// \remarks A valid rendering context is required before most resource
    /// types can be loaded -- meaning nom::RenderWindow must be initialized.
    /// Initializing a nom::RenderWindow is the end-user's responsibility.
    ResourceType* load_resource( const std::string& key )
    {
      ResourceFile member( key );

      auto res = this->resources_.find( member );

      // Found a match
      if( res != this->resources_.end() )
      {
        // Load the resource only if it has not been yet
        if( res->first.loaded() == false )
        {
          // Err if the resource is invalid
          if( res->second->valid() == false )
          {
            NOM_LOG_ERR( NOM, "Could not load resource: NULL." );
            return nullptr;
          }

          // Err if the resource cannot be loaded
          if( res->second->load( res->first.path() ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not load resource: file path does not exist at " + res->first.path() );
            return nullptr;
          }

          // res->first.set_loaded( true );
          res->first.loaded_ = true;

        } // end if not loaded

        return res->second.get();

      } // end if resource found

      // Err
      return nullptr;
    }

    /// \brief Erase all resource elements in the cache.
    void clear( void )
    {
      this->resources_.clear();
    }

    /// \brief Destroy a resource.
    ///
    /// \param key The resource descriptor (name) to be destroyed.
    void erase( const std::string& key )
    {
      this->resources_.erase( key );
    }

    /// \brief Output the stored resource name and file paths.
    ///
    /// \note Debugging aid.
    void dump( void )
    {
      for( auto itr = this->resources_.begin(); itr != this->resources_.end(); ++itr )
      {
        NOM_LOG_INFO( NOM, "Resource " + itr->first.name() + " at path: " + itr->first.path() + "." );
      }
    }

  private:
    /// \brief The available resources list.
    std::map<ResourceFile, std::shared_ptr<ResourceType>> resources_;

    /// \brief Optional delegate that is executed upon the creation of a
    /// resource.
    callback_type callback_;
};

} // namespace nom

#endif // include guard defined

/// \class nom::ResourceCache
/// \ingroup system
///
/// Note that pure abstract object types are not supported.
///
/// \see http://www.gamedev.net/topic/610582-game-resource-manager-design/#entry4860916
/// \see http://www.gamedev.net/page/resources/_/technical/game-programming/a-simple-fast-resource-manager-using-c-and-stl-r2503
/// \see http://gamedev.stackexchange.com/questions/17066/designing-a-resourcemanager-class
