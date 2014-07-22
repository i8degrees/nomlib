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
#include "nomlib/tests/common/VisualUnitTestResultWriter.hpp"

// Private headers
#include "nomlib/system/Path.hpp"
#include "nomlib/system/File.hpp"
#include "nomlib/serializers/IValueSerializer.hpp"
#include "nomlib/serializers/IValueDeserializer.hpp"
#include "nomlib/serializers/JsonCppSerializer.hpp"
#include "nomlib/serializers/JsonCppDeserializer.hpp"

#include "nomlib/tests/common/VisualUnitTest.hpp"
#include "nomlib/tests/common/ImageTestSet.hpp"
#include "nomlib/tests/common/ImageDiffResult.hpp"
// #include "nomlib/tests/common/MinimalTestResultWriter.hpp"
#include "nomlib/tests/common/HTMLTestResultWriter.hpp"

namespace nom {

// Static initializations
bool VisualUnitTestResultWriter::initialized_ = false;
std::string VisualUnitTestResultWriter::ref_dir_ = "\0";
std::string VisualUnitTestResultWriter::output_dir_ = "\0";

void VisualUnitTestResultWriter::initialize (
                                              const std::string& ref_path,
                                              const std::string& output_path
                                            )
{
  // Ensure one-time initialization of input & output directories
  if( VisualUnitTestResultWriter::initialized_ == false )
  {
    VisualUnitTestResultWriter::ref_dir_ = ref_path;
    VisualUnitTestResultWriter::output_dir_ = output_path;
    VisualUnitTestResultWriter::initialized_ = true;
  }
}

void VisualUnitTestResultWriter::OnTestCaseEnd( const ::testing::TestCase& test_info )
{
  Path p;
  File fp;
  ImageTestSet set1;
  ImageTestSet set2;
  IValueDeserializer* in = nullptr;
  TestResultWriter* os = nullptr;

  // We cannot generate test results output if we do not have reference images
  // for comparison!
  if( NOM_TEST_FLAG(reference_screenshot) == true )
  {
    return;
  }

  if( NOM_TEST_FLAG(disable_comparison) == true )
  {
    return;
  }

  // User explicitly requested for no HTML test result output processing
  if( NOM_TEST_FLAG(no_html_output) == true )
  {
    return;
  }

  in = new JsonCppDeserializer();

  if( fp.is_dir( VisualUnitTestResultWriter::ref_dir_ ) == false )
  {
    NOM_LOG_ERR( NOM, "Reference directory path does not exist; did you forget to generate a reference image set?" );
    return;
  }

  set1.set_directory( VisualUnitTestResultWriter::ref_dir_ );
  set2.set_directory( VisualUnitTestResultWriter::output_dir_ );

  if( set1.load_file( in ) == false || set2.load_file( in ) == false )
  {
    return;
  }

  os = new HTMLTestResultWriter( set1, set2, VisualUnitTest::results_ );
  // os = new MinimalTestResultWriter( set1, set2, VisualUnitTest::results_ );

  // NOM_DUMP( os->output() );

  std::string fp_ext = ".html";
  // std::string fp_ext = ".txt";

  std::string results_filename = "results_" + VisualUnitTest::timestamp() + fp_ext;

  std::string results_output =  nom::test_output_directory + p.native() +
                                test_info.name() + p.native() +
                                results_filename;

  os->save_file( results_output );
}

} // namespace nom
