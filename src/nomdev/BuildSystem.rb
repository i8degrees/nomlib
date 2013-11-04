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
require "#{NOMDEV_SRC_DIR}/build_systems/MSBuild.rb"
require "#{NOMDEV_SRC_DIR}/build_systems/UnixMakeFiles.rb"

class BuildSystem

  def initialize( options )
    @options = options

    if ! File.exists? options.build_dir
      FileUtils.mkdir options.build_dir
    end

    FileUtils.cd options.build_dir

    case options.platform
      when 'macosx'
        @build_system = Build::UnixMakeFiles.new( options )
      when 'windows'
        @build_system = Build::MSBuild.new( options )
      else
        puts "Error: Unknown platform: #{options.platform}"
        quit
    end # options.platform
  end # initialize

  def build
    @build_system.build
  end

  def clean
    @build_system.clean
  end

  def install
    @build_system.install
  end

  def uninstall
    @build_system.uninstall
  end

  def docs
    @build_system.docs
  end

end
