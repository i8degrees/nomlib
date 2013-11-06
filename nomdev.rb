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

NOMDEV_WORKING_DIRECTORY = File.expand_path( File.dirname(__FILE__) )
NOMDEV_SRC_DIR = NOMDEV_WORKING_DIRECTORY + "/src/nomdev"
$LOAD_PATH.unshift( NOMDEV_SRC_DIR ) unless $LOAD_PATH.include?( NOMDEV_SRC_DIR )

require 'yaml'

require "#{NOMDEV_SRC_DIR}/config.rb"
require "#{NOMDEV_SRC_DIR}/helpers.rb"
require "#{NOMDEV_SRC_DIR}/NomDevOptions.rb"
require "#{NOMDEV_SRC_DIR}/CMakeGenerator.rb"
require "#{NOMDEV_SRC_DIR}/BuildSystem.rb"
require "#{NOMDEV_SRC_DIR}/ExternalDeps.rb"

options = NomDevOptions.parse( ARGV )

if NOMDEV_DEBUG
  p "options: #{options}"
  p "ARGV: #{ARGV}"
end

command = ARGV[0]

case command
  when "gen", "generate"
    generator = CMakeGenerator.new( options )
    generator.generate
  when "build"
    build_system = BuildSystem.new( options )
    build_system.build
  when "clean"
    build_system = BuildSystem.new( options )
    build_system.clean
  when "rebuild"
    if platform["windows"]
      puts "Error: feature currently disabled; broken on Windows platform."
      quit
    else
      build_system = BuildSystem.new( options )
      build_system.clean
      build_system.build
    end
  when "install"
    build_system = BuildSystem.new( options )
    build_system.install
  when "uninstall", "remove"
    build_system = BuildSystem.new( options )
    build_system.uninstall
  when "wipe"
    # By using the NOMDEV_WORKING_DIRECTORY constant, we are (hopefully!)
    # limiting potential destruction to the directory (and its children) in
    # which nomdev.rb resides in!
    pwd = NOMDEV_WORKING_DIRECTORY
    if options.dry_run
      FileUtils.rmtree "#{pwd}/#{options.build_dir}", :verbose => true, :noop => true
    else
      FileUtils.rmtree "#{pwd}/#{options.build_dir}", :verbose => false, :noop => false
    end
    exit 0
  when "docs", "documentation" # experimental
    build_system = BuildSystem.new( options )
    build_system.docs
  when "analyze"
    build_system = BuildSystem.new( options )
    build_system.analyze
  when "get" # experimental
    deps = ExternalDeps.new( options )
    deps.get
  when "archive" # experimental
    deps = ExternalDeps.new( options )
    deps.archive
  when "LOC", "loc" # Uses CLOC: http://cloc.sourceforge.net/
    loc_filename = "#{NOMDEV_WORKING_DIRECTORY}/.loc.yml"

    args = Array.new
    args << '--quiet'
    args << "--ignored=#{NOMDEV_WORKING_DIRECTORY}/.cloc_ignored.log" if NOMDEV_DEBUG
    args << "--out=#{loc_filename}"
    args << '--yaml'
    args << '--exclude-dir=scale2x,hqx,jsoncpp,build,extra,third-party,tmp,wiki'
    args << "#{NOMDEV_WORKING_DIRECTORY}"

    if ! run( 'cloc', *args, options )
      puts "Error: Could not save #{loc_filename} using cloc."
      quit
    end

    if File.exists? loc_filename
      buffer = YAML.load_file( loc_filename )
    else
      puts "Error: Could not open #{loc_filename}."
      quit
    end

    config = buffer["SUM"]["code"]

    if config
      puts "\n" # Clear the buffer that CLOC seems to leave behind
      puts config
    else
      puts "Error: Could not parse #{loc_filename}."
      quit
    end
  else
    puts "Error: Unknown command #{ARGV[0]}"
    quit
end

exit 0
