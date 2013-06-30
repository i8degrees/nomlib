/******************************************************************************
    Logger.h

  File I/O Logging

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMEAPP_LOGGER_HEADERS
#define GAMEAPP_LOGGER_HEADERS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define DEBUG_LOGGER
#define DEBUG_LOGGER_OBJ

class Logger
{
  public:
    Logger ( void );
    ~Logger ( void );

    std::vector<int> Read ( std::string filename );
    static void List ( std::vector<int> values );

  private:
    std::ifstream fp;
};

/*  Example Usage:

#include "Logger.h"

// Declare
Logger logDebug;

// Init
logger = logDebug.Read( "./data/offsets.val" );

// Helper method for debug logger
void reloadDebugFile ( void )
{
  logger.clear();
  logger = logDebug.Read ( "./data/offsets.val" );
}

*/

#endif // GAMEAPP_GAMESTATE_MANAGER_HEADERS defined
