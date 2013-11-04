#!/usr/bin/env ruby

=begin

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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

=end

=begin

# TODO #

- [ ] Stub

# References #

* Stub

=end

require 'fileutils'

require "#{NOMDEV_SRC_DIR}/config.rb"

module Build

  class UnixMakeFiles

    def initialize( options )
      @options = options
    end

    def build
      run( "make", @options.threads, @options )
    end

    def clean
      run( "make", "clean", @options )
    end

    def install
      run( "make", "install", @options )
    end

    def uninstall
      run( "make", "uninstall", @options )
    end

    def docs
      run( "make docs", @options )
    end

    def tests
      run( "make tests", @options )
    end

    def analyze
      puts "TODO: implement me!"
      quit
    end

    def package
      puts "TODO: implement me!"
      quit
    end

  end # UnixMakeFiles

end # Build
