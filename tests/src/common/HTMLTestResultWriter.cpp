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
#include "nomlib/tests/common/HTMLTestResultWriter.hpp"

// Private headers
#include "nomlib/serializers/MiniHTML.hpp"

// Forward declarations
#include "nomlib/tests/common/ImageTestSet.hpp"

namespace nom {

HTMLTestResultWriter::HTMLTestResultWriter  (
                                              const ImageTestSet& set1,
                                              const ImageTestSet& set2,
                                              const ImageDiffResultBatch& results
                                            ) :
  TestResultWriter( set1, set2, results )
{
  // NOM_LOG_TRACE( NOM );

  // NOM_DUMP( results.size() );

  // for( auto itr = results.begin(); itr != results.end(); ++itr )
  // {
  //   NOM_DUMP( (*itr).image_filename );
  //   NOM_DUMP( (*itr).incorrect_pixels );
  // }
}

HTMLTestResultWriter::~HTMLTestResultWriter( void )
{
  // NOM_LOG_TRACE( NOM );
}

std::string HTMLTestResultWriter::output( void )
{
  std::stringstream output;

  // Dump the doctype in beforehand, since it's formatted strangely
  output  <<  "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" "
          <<  "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";

  // root 'html' tag
  HTMLElement html = HTMLElement("html");
  html.append_attribute( "xmlns", "http://www.w3.org/1999/xhtml" );

  // add the head
  HTMLElement* head = html.append_element("head");
  head->append_element("title")->append_text("nom::VisualUnitTest Results");

  // link the stylesheet
  HTMLElement* css = head->append_element("link");
  css->append_attribute("rel","stylesheet");

  // For the moment it's hosted on my personal site, for convenience
  css->append_attribute( "href", "http://nomlib.sourceforge.net/tests/results.css" );
  css->append_attribute("type", "text/css");

  // link a little javascript
  HTMLElement* js = head->append_element("script");
  js->append_attribute("src", "http://nomlib.sourceforge.net/tests/results.js");
  js->append_attribute("type", "text/javascript");
  js->append_text("");// so it doesn't self close

  // </head>

  // add body
  HTMLElement* body = html.append_element("body");

  // title
  body->append_element("h1")->append_text("nom::VisualUnitTest Results");

  // div for summary
  HTMLElement* summary_div = body->append_element("div");
  summary_div->append_element("h2")->append_text("Overall:");
  HTMLElement* content_div = summary_div->append_element("div");
  content_div->append_attribute("class", "contentarea");
  content_div->append_element("hr");

  // add info tables about the sets
  content_div->append_child( output_info_table(this->set1_, "Reference Set:") );
  content_div->append_child( output_info_table(this->set2_, "Test Set:") );
  content_div->append_element("hr");

  // summarize results
  uint num_passed = 0;
  for( auto i = 0; i < this->results_.size(); ++i )
  {
    if( this->results_[i].passed )
    {
      ++num_passed;
    }
  }

  content_div->append_element("h3")->append_text(
    std::to_string(num_passed) + " of "
    + std::to_string( this->results_.size() ) + " tests passed.");

  content_div->append_element("hr");

  // add thumbnails
  HTMLElement* thumbs = content_div->append_element("p");
  for( auto i = 0; i < this->results_.size(); ++i )
  {
    HTMLElement* anchor = thumbs->append_element("a");

    anchor->append_attribute  ( "href", std::string("#") +
                                this->results_[i].test_name + "_" +
                                std::to_string(this->results_[i].frame)
                              );

    anchor->append_attribute("title", this->results_[i].test_name );

    // Placeholder:
    HTMLElement* img = anchor->append_element("img");
    img->append_attribute ( "class",
                            this->results_[i].passed ? "thumb" : "thumb_fail"
                          );

    img->append_attribute ( "src",
                            this->set2_.test_set() + "_" +
                            this->set2_.timestamp() + "/" +
                            this->results_[i].image_filename
                          );

    img->append_attribute("alt", "Comparison Image" );
  }

  // add side-by-side images and summary for each test
  for( auto i = 0; i < this->results_.size(); ++i )
  {
    body->append_child  (
                          summarize_single_result (
                                                    this->results_[i],
                                                    this->set1_,
                                                    this->set2_
                                                  )
                        );
  }

  // print to the stream and return
  output << html.print();

  return output.str();
}

HTMLElement*
HTMLTestResultWriter::summarize_single_result (
                                                const ImageDiffResult& result,
                                                const ImageTestSet& set1,
                                                const ImageTestSet& set2
                                              )
{
  // container and header
  HTMLElement* container = new HTMLElement("div");
  container->append_attribute ( "id",
                                result.test_name + "_" +
                                std::to_string(result.frame)
                              );

  container->append_element("h2")->append_text( result.test_name
      + " (frame " + std::to_string( result.frame ) + ")");
  HTMLElement* content = container->append_element("div");

  // if failed, we give it a different class, and make it red
  content->append_attribute("class", std::string("contentarea")
      + (result.passed ? "" : " failed_test"));

  // first image
  HTMLElement* column1 = content->append_element("div");
  column1->append_attribute("class", "img_column");
  column1->append_element("h3")->append_text("Original:");
  HTMLElement* img = column1->append_element("img");

  img->append_attribute("alt", result.test_name + " original");
  img->append_attribute("src", "Reference/" + result.image_filename );

  // second image
  HTMLElement* column2 = content->append_element("div");
  column2->append_attribute("class", "img_column");
  column2->append_element("h3")->append_text("New:");
  img = column2->append_element("img");

  img->append_attribute("alt", result.test_name + " new");
  img->append_attribute ( "src",
                          set2.test_set() + "_" +
                          this->set2_.timestamp() + "/" +
                          result.image_filename
                        );

  // summary
  content->append_element("hr");
  HTMLElement* status = content->append_element("h3");
  status->append_text("Status: ");
  HTMLElement* span = status->append_element("span");
  span->append_text(result.passed ? "Passed" : "Failed");
  span->append_attribute("class", result.passed ? "passed" : "failed");
  content->append_element("hr");

  content->append_element("h4")->append_text("Comparison Summary:");

  if( result.incorrect_pixels )
  {
    HTMLElement* abs_diff = content->append_element("p");
    abs_diff->append_attribute("class", "diffreport");
    abs_diff->append_text( std::to_string( result.incorrect_pixels ) +
        " pixels differed.");

    HTMLElement* mse = content->append_element("p");
    mse->append_attribute("class", "diffreport");
    mse->append_element("strong")->append_text(" MSE | ");
    mse->append_text("Overall: " + format_float(result.mse) + " | ");
    mse->append_text("R: " + format_float( result.mse_channels.r ) + " | ");
    mse->append_text("G: " + format_float( result.mse_channels.g ) + " | ");
    mse->append_text("B: " + format_float( result.mse_channels.b ) + " |");

    HTMLElement* psnr = content->append_element("p");
    psnr->append_attribute("class", "diffreport");
    psnr->append_element("strong")->append_text("PSNR| ");
    psnr->append_text("Overall: " + format_float( result.psnr ) + " | ");
    psnr->append_text("R: " + format_float( result.psnr_channels.r ) + " | ");
    psnr->append_text("G: " + format_float( result.psnr_channels.g ) + " | ");
    psnr->append_text("B: " + format_float( result.psnr_channels.b ) + " |");

    HTMLElement* ssim = content->append_element("p");
    ssim->append_attribute("class", "diffreport");
    ssim->append_text ( "Structural Similarity Index: " +
                        format_float( result.ssim )
                      );
  }
  else
  {
    content->append_element("p")->append_text("Images are identical.");
  }

  return container;
}

HTMLElement*
HTMLTestResultWriter::output_info_table (
                                          const ImageTestSet& set,
                                          const std::string& name
                                        )
{
  // main div
  HTMLElement* column = new HTMLElement("div");
  column->append_attribute("class", "img_column");

  // add a bit of header text
  column->append_element("h3")->append_text(name);

  // make the table, and rows for each stat
  HTMLElement* table = column->append_element("table");
  table->append_attribute( "summary", "Image meta-data" );

  HTMLElement* row = table->append_element("tr");
  row->append_element("th")->append_text("Time:");
  row->append_element("td")->append_text( set.timestamp() );
  row = table->append_element("tr");
  row->append_element("th")->append_text("Version:");
  row->append_element("td")->append_text( set.version() );
  row = table->append_element("tr");

  row->append_element("th")->append_text("Resolution:");
  row->append_element("td")->append_text( std::to_string( set.resolution().w )
      + " x " + std::to_string( set.resolution().h ) );

  row = table->append_element("tr");
  row->append_element("th")->append_text("Comment:");
  row->append_element("td")->append_text( set.comment() );

  // return the whole thing, ready to be attached into a larger document
  return column;
}

std::string HTMLTestResultWriter::format_float( float num, int length )
{
  std::stringstream ss;

  ss.setf( std::ios::fixed, std::ios::floatfield );
  ss.setf( std::ios::showpoint );
  ss.precision( 6 );
  ss << num;

  std::string out = "\0";
  ss >> out;
  out = out.substr( 0, length );

  if( out.size() < length )
  {
    while( out.size() < length )
    {
      out += "0";
    }
  }

  return out;
}

} // namespace nom
