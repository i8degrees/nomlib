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

- [ ] OS X Universal Binary option switch for 'macosx' platform

# References #

* [GetoptLong API Docs](http://www.ruby-doc.org/stdlib-2.0.0/libdoc/getoptlong/rdoc/GetoptLong.html)
* [OptionParser How To](http://ruby.about.com/od/advancedruby/a/optionparser2.htm)
http://www.stelfox.net/blog/2012/12/02/rubys-option-parser-a-more-complete-example/
* [How do I make a command-line tool in Ruby?](http://rubylearning.com/blog/2011/01/03/how-do-i-make-a-command-line-tool-in-ruby/)

=end

require 'ostruct'
require 'optparse'
require 'fileutils'
require "#{NOMDEV_SRC_DIR}/config.rb"

class NomDevOptions

  def self.parse( args )
    options = OpenStruct.new

    # Option defaults
    options.version = NOMDEV_VERSION
    options.arch = :x86
    options.platform = platform
    options.build_dir = "build"
    options.build_debug = false
    options.build_examples = false
    options.build_tests = false
    options.build_docs = false
    options.install_prefix = "#{NOMDEV_WORKING_DIRECTORY}/#{options.build_dir}"
    options.developer = false
    options.dry_run = false

    if platform["macosx"]
      options.threads = ENV['MAKEFLAGS']
    elsif platform["windows"]
      options.threads = ENV['NUMBER_OF_PROCESSORS']
    else
      options.threads = ENV['MAKEFLAGS']
    end

    # Platform-dependent style used for help output for 'example usage'
    if platform["macosx"]
      nomdev_cmd = "$ ./nomdev.rb"
      nomdev_prefix = "~/Library/Frameworks"
    else # Assume MS DOS prompt
      nomdev_cmd = "nomdev.rb"
      nomdev_prefix = "C:/Users#{ENV['HOME']}/libs/nomlib"
    end

    opts = OptionParser.new do |opt|
      opt.set_summary_indent( " " )

      opt.banner = "Usage: #{NOMDEV_APP_NAME} COMMAND [OPTIONS]"

      opt.separator "" # Newline

      opt.separator <<-EOF

Available Commands:

  gen, generate
    Generate build scripts in #{options.build_dir} using CMake

  build
    Build the project

  clean
    Clean the project

  install
    Install the project

  uninstall, remove
    Remove the project

  wipe
    Purge the #{options.build_dir} directory

  analyze
    Run static analysis on the project

  archive
    Archive third-party dependencies

  LOC, loc
    Report current LOC (lines of code) of project
EOF
      opt.separator "" # Newline

      opt.separator "Available Options: "

      opt.separator "" # Newline
    end

    opts.on( '-h', '--help', '--usage', "Show #{NOMDEV_APP_NAME} help message" ) do
      puts opts
      exit 0
    end

    opts.separator "" # Newline

    opts.on( '-v', '--version', "Show version string for #{NOMDEV_APP_NAME}" ) do
      puts options.version
      exit 0
    end

    opts.separator "" # Newline

    opts.on( '-b', '--build_dir BUILD', "Directory used for building a project", "The default is: #{options.build_dir}" ) do |build_dir|
      options.build_dir = build_dir
    end

    opts.separator "" # Newline

    opts.on( '-a', '--arch ARCH', [:x86, :x64], "Generate build scripts for a specific architecture", "The default is: #{options.arch}" ) do |arch|
      options.arch = arch
    end

    opts.separator "" # Newline

    opts.on( '--[no-]debug', "Enable building of debugging features", "The default is: #{options.build_debug}" ) do |boolean|
      options.build_debug = boolean
    end

    opts.separator "" # Newline

    opts.on( '--[no-]examples', "Enable building of examples", "The default is: #{options.build_examples}" ) do |boolean|
      options.build_examples = boolean
    end

    opts.separator "" # Newline

    opts.on( '--[no-]tests', "Enable building of unit tests", "The default is: #{options.build_tests}" ) do |boolean|
      options.build_tests = boolean
    end

    opts.separator "" # Newline

    opts.on( '--[no-]docs', "Enable building of documentation", "The default is: #{options.build_docs}" ) do |boolean|
      options.build_docs = boolean
    end

    opts.separator "" # Newline

    opts.on( '-i', '--prefix INSTALL_PREFIX', "Installation path", "The default is: #{NOMDEV_WORKING_DIRECTORY}/#{options.build_dir}" ) do |install_prefix|
      options.install_prefix = install_prefix
    end

    opts.separator "" # Newline

    opts.on( '--developer', "Enable developer options;", "This implies '--debug, --examples'", "The default is: #{options.developer}" ) do |boolean|
      options.build_debug = boolean
      options.build_examples = boolean
      #options.build_tests = boolean
      #options.build_docs = boolean
    end

    opts.separator "" # Newline

    opts.on( '-p', '--platform PLATFORM', "Override the automatic platform detection", "The default is: #{options.platform}" ) do |platform|
      options.platform = platform
    end

    opts.separator "" # Newline

    opts.on( '--threads THREADS', "Override the automatic number of CPU threads to build with", "The default is: #{options.threads}" ) do |threads|
      options.threads = threads
    end

    opts.separator "" # Newline

    opts.on( '-n', '--dry-run', "Show what would be done, but do not do it", "The default is: #{options.dry_run}" ) do |boolean|
      options.dry_run = boolean
    end

    opts.separator "" # Newline

    opts.separator "Example Usage: "

    opts.separator "" # Newline

    opts.separator "#{nomdev_cmd} gen --arch x64 --build_dir build-x64 --debug --examples"
    opts.separator "#{nomdev_cmd} build --build_dir build-x64"
    opts.separator "#{nomdev_cmd} install --prefix #{nomdev_prefix}"

    # Additional help reference invocation;
    # OptParse does not support these non-POSIX arguments!
    #
    # But I insist on providing what I deem 'user-friendly', so...
    if ARGV[0] == 'help' || ARGV[0] == '/?' || ARGV[0] == "usage" || ARGV.empty?
      puts opts
      exit 0
    end

    opts.parse!( args )

    options

  end # parse

end
