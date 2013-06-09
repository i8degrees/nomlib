/******************************************************************************
    Transformable.cpp

  Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Transformable.hpp"

nom::Transformable::Transformable ( void )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::Transformable (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  // Init vars
}

nom::Transformable::~Transformable ( void )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::~Transformable (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Cleanup vars
}
