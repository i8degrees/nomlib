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
#include "nomlib/tests/VisualUnitTest/TestResultWriter.hpp"

// Forward declarations
#include "nomlib/tests/VisualUnitTest/ImageTestSet.hpp"

namespace nom {

TestResultWriter::TestResultWriter  (
                                      const ImageTestSet& set1,
                                      const ImageTestSet& set2,
                                      const ImageDiffResultBatch& results
                                    ) :
  set1_{ set1 },
  set2_{ set2 },
  results_{ results }
{
  // NOM_LOG_TRACE( NOM );
}

TestResultWriter::~TestResultWriter( void )
{
  // NOM_LOG_TRACE( NOM );
}

void TestResultWriter::save_file( const std::string& filename )
{
  std::ofstream fp;

  fp.open( filename );

  if( fp.is_open() )
  {
    fp << output();

    fp.close();
  }
}

std::string TestResultWriter::output( void )
{
  return "\0";
}

} // namespace nom
