#!/usr/bin/env ruby
require 'fileutils'
require 'rbconfig'

# http://stackoverflow.com/questions/11784109/detecting-operating-systems-in-ruby/13586108#13586108
def platform
  @os ||= (
    host_os = RbConfig::CONFIG['host_os']

    case host_os
    when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
      :windows
    when /darwin|mac os/
      :macosx
    when /linux/
      :linux
    when /solaris|bsd/
      :unix
    else
      :unknown
    end
  )
end

class VisualStudio
  def init
    # Setup the development environment for CMake with Visual Studio (2013 RC1)
    existing_path = ENV['path']

    msvcpp_path = ENV['PROGRAMFILES'] + "/" + "Microsoft Visual Studio 12.0/VC/bin"

    if File.exists? msvcpp_path
      ENV['PATH'] = msvcpp_path + ";" + existing_path
    end
  end

  def build
    if ! system "msbuild /t:build ALL_BUILD.vcxproj"
      system "pause"
      exit 1
    end
    if ! system "msbuild /t:build INSTALL.vcxproj"
      system "pause"
      exit 1
    end
  end
end

class CMakeDev
  def init
    if ! File.exists? 'build'
      FileUtils.mkdir 'build'
    end

    FileUtils.cd 'build'

    # Clean out CMake variable cache
    FileUtils.rm 'CMakeCache.txt', :force => true
    FileUtils.rm 'CMakeFiles', :force => true
  end

  def generate
    if ! system "cmake -D DEBUG=on -D EXAMPLES=on .."
      system "pause"
      exit 1
    end
  end
end

if ! platform["windows"]
  puts "ERR: Detected platform is not Windows."
  exit 1
end

cxx = VisualStudio.new
cxx.init

cmake = CMakeDev.new
cmake.init

# Commence build configuration of project;
#
# If all goes well, we ought to have generated Visual Studio project files under
# the 'build' dir!
cmake.generate

cxx.build

# Return to where we came from
FileUtils.cd ".."
