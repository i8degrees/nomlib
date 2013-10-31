#!/usr/bin/env ruby
#
#   TODO
#
# * add checks for existence of external binaries: tar and zip;
#   consider distributing the OSS 7-zip cmd-line binary under windows?
#
# * ensure that we are always in the project root of nomlib
#
# * verify integrity of third-party files downloaded with a sha1 checksum
#
# * Research using vcvarsall.bat script for setting up paths for visual studio;
#   this ought to free us from figuring things out ourselves!
#
# * 'init' command that permanently records the working directory it is
#   initialized with, to always work from (think: a prefix). In other words:
#   a simple jail or sandbox to minimize potentially dangerous modifications,
#   such as when we blow away an entire 'build' directory.
#
require 'fileutils'
require 'open-uri'
require 'rbconfig'

#WORKING_DIRECTORY = FileUtils.pwd

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

# Graceful exit; do not close out of our console windows until we have been
# given a chance to read the error messages!
#
# FIXME: I'm unwisely assuming that we are being ran inside an interactive
# terminal (user controlled) unless we are on the Windows platform.
def quit
  if platform["windows"]
    system "pause"
  end

  exit 1
end

class ThirdPartyDeps #ExternalDeps < NomDev
  #attr_writer :save_path, :deps_path
  #attr_accessor :save_path, :deps_path
  @debug = false
  def initialize
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

    open(@save_path, 'wb') do |file|
      file << open(@deps_path).read
    end

    if self.debug?
      puts "File has been saved at " << @pwd << "/" << @save_path
    end
  end

  def archive(os)
    case os
      when 'osx'
        if File.exists? @osx_deps_filename
          puts "ERR: " << @pwd << "/" << @osx_deps_filename << " " << "already exists!"
          exit 1
        else
          system @osx_deps_cmd or quit
        end # file exists
      when 'windows', 'win'
        if File.exists? @windows_deps_filename
          puts "ERR: " << @pwd << "/" << @windows_deps_filename << " " << "already exists!"
          exit 1
        else
          system @windows_deps_cmd or quit
        end # file exists
      else
        puts "ERR: You must specify the platform third-party dependencies you wish to archive."
        exit 1
    end # case

    exit 0
  end # archive

  def debug?
    return @debug
  end

  def debug=(toggle)
    @debug = toggle
  end
end

#class CMakeGenerator < CMakeBuild
  #def initialize
  #end

  #def build
  #end

  #def clean
  #end

  #def install
  #end

  #def remove
  #end

  #def package
  #end

  #def analyze
  #end

  #def test
  #end

  #def docs
  #end
#end

#class UnixMakeFiles < CMakeGenerator
#class VisualStudio12 < CMakeGenerator

class VisualStudio #< CMakeGenerator
  def initialize
    # Setup the development environment for CMake with Visual Studio (2013 RC1)
    existing_path = ENV['path']

    msvcpp_path = ENV['PROGRAMFILES'] + "/" + "Microsoft Visual Studio 12.0/VC/bin"

    if File.exists? msvcpp_path
      ENV['PATH'] = msvcpp_path + ";" + existing_path
    end

    @build_dir = "build" # current cmake build directory

    # A complete record of the project compile, when invoked by the 'build'
    # command, saved at the project's build dir -- overwritten with the latest
    # build log.
    @build_opts = "/fileLogger /fileLoggerParameters:LogFile=#{@build_dir}\\nomlib.log"
    #@build_opts = "/p:RunCodeAnalysis=False"
  end

  def build
    system "msbuild #{@build_opts} /t:build #{@build_dir}\\ALL_BUILD.vcxproj" or quit
    system "msbuild #{@build_opts} /t:build #{@build_dir}\\INSTALL.vcxproj" or quit
  end # build

  def clean
    if File.exists? "#{@build_dir}\\ALL_BUILD.vcxproj"
      system "msbuild /t:clean #{@build_dir}\\ALL_BUILD.vcxproj" or quit
    end

    if File.exists? "#{@build_dir}\\INSTALL.vcxproj"
      system "msbuild /t:clean #{@build_dir}\\INSTALL.vcxproj" or quit
    end
  end # clean

end # End class VisualStudio

class CMakeDev #CMakeBuild < NomDev
  def initialize
    if ! File.exists? 'build'
      FileUtils.mkdir 'build'
    end

    FileUtils.cd 'build' #if WORKING_DIRECTORY != "build"

    # Clean out CMake variable cache
    FileUtils.rm 'CMakeCache.txt', :force => true
    FileUtils.rm 'CMakeFiles', :force => true
  end

  def generate
    if ARGV[0] == "gen" and ARGV[1] == "x64" # 64-bit build
      system "cmake -G \"Visual Studio 12 Win64\" -D DEBUG=on -D EXAMPLES=on -D ARCH_64=on .." or quit
    else # Assume x86 build by default
      system "cmake -G \"Visual Studio 12\" -D DEBUG=on -D EXAMPLES=on -D ARCH_32=on .." or quit
    end
  end # generate

end # CMakeDev

command = ARGV[0]

if command == "gen"
  cmake = CMakeDev.new
  cmake.generate
elsif command == "build"
  cxx = VisualStudio.new
  cxx.build
elsif command == "clean"
  cxx = VisualStudio.new
  cxx.clean
elsif command == "archive"
  deps = ThirdPartyDeps.new
  deps.archive(argument)
else
  exit 0
end

# Return to where we came from
FileUtils.cd ".." if FileUtils.pwd == 'build'

exit 0 # ...for added peace of mind =P
