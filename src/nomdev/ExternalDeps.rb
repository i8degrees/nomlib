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

- [ ] add checks for existence of external binaries: tar and zip;
      consider distributing the OSS 7-zip cmd-line binary under windows?

- [ ] verify integrity of third-party files downloaded with a sha1 checksum

- [ ] support dry_run option

# References #

* Stub

=end

require 'fileutils'
require 'open-uri'
require 'net/http'

require "#{NOMDEV_SRC_DIR}/config.rb"

class ExternalDeps

  def initialize( options )
    @options = options
    @save_path = "nomlib_windows-dependencies.zip"
    @deps_path = "http://sourceforge.net/projects/nomlib/files/nomlib_windows-dependencies.zip/download"
    @pwd = FileUtils.pwd
    @osx_deps_filename = 'nomlib_osx-dependencies.tar.gz'
    @windows_deps_filename = 'nomlib_windows-dependencies.zip'
    @osx_deps_cmd = 'tar czvf ./nomlib_osx-dependencies.tar.gz --exclude="*.DS_Store*" --exclude="windows" --exclude="linux" third-party/'
    @windows_deps_cmd = 'zip -r ./nomlib_windows-dependencies.zip third-party/ -x \*.DS_Store* \*osx\* \*linux\*'
  end

  def get
    if File.exists? @save_path
      FileUtils.rm @save_path
    end
    puts "Downloading dependencies for nomlib... "

    open( @save_path, 'wb' ) do |file|
      file << open( @deps_path ).read
    end

    puts "File has been saved at " << @pwd << "/" << @save_path
  end

  def archive
    case @options.platform
      when 'macosx'
        if File.exists? @osx_deps_filename
          puts "Error: " << @pwd << "/" << @osx_deps_filename << " " << "already exists!"
          quit
        else
          run( @osx_deps_cmd, @options )
        end # file exists
      when 'windows'
        if File.exists? @windows_deps_filename
          puts "Error: " << @pwd << "/" << @windows_deps_filename << " " << "already exists!"
          quit
        else
          run( @windows_deps_cmd, @options )
        end # file exists
      else
        puts "Error: Unknown platform: #{options.platform}."
        quit
    end # case options.platform
  end # archive

  def clean
    if File.exists? @osx_deps_filename
      FileUtils.rm @osx_deps_filename, :verbose => true
    end

    if File.exists? @windows_deps_filename
      FileUtils.rm @windows_deps_filename, :verbose => true
    end
  end
end
