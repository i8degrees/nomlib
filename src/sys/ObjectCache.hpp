/******************************************************************************

    Object Cache

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved

******************************************************************************/
#ifndef NOMLIB_OBJECT_CACHE_HEADERS
#define NOMLIB_OBJECT_CACHE_HEADERS

#include <string>
#include <memory>
#include <map>

#include "config.hpp"
#include "sys/Logger.hpp"

namespace nom {
  namespace priv {

class ObjectCache
{
  public:
    ObjectCache ( void );
    ~ObjectCache ( void );

    std::shared_ptr<void> addObject ( const std::string& key,
                                      std::shared_ptr<void> object
                                    );

    /// \todo TEST ME
    bool removeObject ( const std::string& key );

    std::shared_ptr<void> getObject ( const std::string& key );

  private:
    static std::map <std::string, std::shared_ptr<void>> cache;
};


  } // namespace priv
} // namespace nom

#endif // NOMLIB_OBJECT_CACHE_HEADERS defined
