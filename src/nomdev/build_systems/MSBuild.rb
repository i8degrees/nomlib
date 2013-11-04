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

- [ ] Use vcvarsall.bat script for setting up paths for build tool chain

# References #

* [MSBuild Command Line Reference](http://msdn.microsoft.com/en-us/library/ms164311(v=vs.90).aspx)
* [Setting the Path and Environment Variables for Command-Line Builds](http://msdn.microsoft.com/en-us/library/f2ccy3wt(v=vs.110).aspx)
* [How to: Enable a 64-Bit Visual C++ Toolset on the Command Line](http://msdn.microsoft.com/en-us/library/x4d2c09s(v=vs.110).aspx)

=end

require 'fileutils'

require "#{NOMDEV_SRC_DIR}/config.rb"

module Build

  class MSBuild # MS VC++ 2013 RC1

    def initialize( options )
      @options = options
      @args = Array.new
      @projects = Array.new

=begin
      if @options.platform["windows"] && options.dry_run == false
        # Setup the development environment for CMake with Visual Studio (2013 RC1)
        #existing_path = ENV['PATH']
        #msvcpp_path = ENV['PROGRAMFILES'] + "/" + "Microsoft Visual Studio 12.0/VC/bin"
        #if File.exists? msvcpp_path
          #ENV['PATH'] = msvcpp_path + ";" + existing_path
        #end
      end
=end

      @projects = [ "ALL_BUILD.vcxproj", "INSTALL.vcxproj" ]

      # Number of threads to run concurrently
      @args << "/maxcpucount:#{@options.threads}"

      # File Logger for Errors
      @args << "/fl1"
      @args << "/flp1:LogFile=nomlib_err.log;errorsonly"

      # File Logger for Warnings
      @args << "/fl2"
      @args << "/flp2:LogFile=nomlib_warn.log;warningsonly"

      @args << "/Verbosity:minimal"
      #@args << "/p:RunCodeAnalysis=False"

    end # initialize

    def build
      @args << "/t:build"
      for project in @projects
        run( "msbuild", *@args, project, @options )
      end
    end # build

    def clean
      @args << "/t:clean"
      for project in @projects
        run( "msbuild", *@args, project, @options )
      end
    end # clean

    def install
      self.build
    end

    def uninstall
      self.build
    end

    def docs
      puts "TODO: implement me!"
      quit
    end

    def tests
      puts "TODO: implement me!"
      quit
    end

    def analyze
      puts "TODO: implement me!"
      quit
    end

    def package
      puts "TODO: implement me!"
      quit
    end

  end # MSBuild

end # namespace Build
