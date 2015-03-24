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
#include "nomlib/tests/VisualUnitTest/MinimalTestResultWriter.hpp"

// Forward declarations
#include "nomlib/tests/VisualUnitTest/ImageTestSet.hpp"

namespace nom {

MinimalTestResultWriter::MinimalTestResultWriter  (
                                                    const ImageTestSet& set1,
                                                    const ImageTestSet& set2,
                                                    const ImageDiffResultBatch&
                                                    results
                                                  ) :
  TestResultWriter( set1, set2, results )
{
  // NOM_LOG_TRACE( NOM );
}

MinimalTestResultWriter::~MinimalTestResultWriter( void )
{
  // NOM_LOG_TRACE( NOM );
}

std::string MinimalTestResultWriter::output( void )
{
  std::stringstream os;

  for( auto i = 0; i < this->results_.size(); ++i )
  {
    os  <<  this->results_[i].test_name
        <<  "="
        << ( this->results_[i].passed ? "Passed" : "Failed" )
        << "\n";
  }

  return os.str();
}

} // namespace nom
