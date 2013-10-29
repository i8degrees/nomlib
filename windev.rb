#!/usr/bin/env ruby
require 'fileutils'
require 'open-uri'
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

# Do not close out of our console window until we have been given a chance to
# read error messages.
def quit
  system "pause"
  exit 1
end

class ThirdPartyDeps
  #attr_writer :save_path, :deps_path
  #attr_accessor :save_path, :deps_path
  @debug = false
  def initialize
    @save_path = "nomlib_windows-dependencies.zip"
    @deps_path = "http://sourceforge.net/projects/nomlib/files/nomlib_windows-dependencies.zip/download"
    @pwd = FileUtils.pwd
  end

  def get
    if File.exists? @save_path
      FileUtils.rm @save_path
    end
    puts "Downloading dependencies for nomlib... "

    open(@save_path, 'wb') do |file|
      file << open(@deps_path).read
    end

    if self.debug?
      puts "File has been saved at " << @pwd << "/" << @save_path
    end

    # TODO; verify integrity of file with sha1sum
  end

  def debug?
    return @debug
  end

  def debug=(toggle)
    @debug = toggle
  end
end

class VisualStudio
  def initialize
    # Setup the development environment for CMake with Visual Studio (2013 RC1)
    existing_path = ENV['path']

    msvcpp_path = ENV['PROGRAMFILES'] + "/" + "Microsoft Visual Studio 12.0/VC/bin"

    if File.exists? msvcpp_path
      ENV['PATH'] = msvcpp_path + ";" + existing_path
    end
    @build_opts = ""
    #@build_opts = "/p:RunCodeAnalysis=False"
  end

  def build
    system "msbuild" + " " + @build_opts + " " + "/t:build ALL_BUILD.vcxproj" or quit
    system "msbuild" + " " + @build_opts + " " + "/t:build INSTALL.vcxproj" or quit
  end # build

  def clean
    if File.exists? 'ALL_BUILD.vcxproj'
      system "msbuild /t:clean ALL_BUILD.vcxproj" or quit
    end

    if File.exists? 'INSTALL.vcxproj'
      system "msbuild /t:clean INSTALL.vcxproj" or quit
    end
  end # clean

end # End class VisualStudio

class CMakeDev
  def initialize
    if ! File.exists? 'build'
      FileUtils.mkdir 'build'
    end

    FileUtils.cd 'build'

    # Clean out CMake variable cache
    FileUtils.rm 'CMakeCache.txt', :force => true
    FileUtils.rm 'CMakeFiles', :force => true
  end

  # TODO; we should probably generate x86 and x64 project files in separate
  # build dirs -- "build-x86" and "build-x64".
  def generate
    if ARGV[0] == "gen" and ARGV[1] == "x64" # 64-bit build
      system "cmake -G \"Visual Studio 12 Win64\" -D DEBUG=on -D EXAMPLES=on -D ARCH_64=on .." or quit
    else # Assume x86 build by default
      system "cmake -G \"Visual Studio 12\" -D DEBUG=on -D EXAMPLES=on -D ARCH_32=on .." or quit
    end
  end # generate

end # CMakeDev

command = ARGV[0]

#deps = ThirdPartyDeps.new
#deps.debug = true
#deps.get

if ! platform["windows"]
  puts "ERR: Detected platform is not Windows."
  exit 1
end

cxx = VisualStudio.new
cmake = CMakeDev.new

if command == "gen"
  cmake.generate
elsif command == "build"
  cxx.build
elsif command == "clean"
  cxx.clean
else
  exit 0
end

# Return to where we came from
FileUtils.cd ".." if FileUtils.pwd == 'build'
