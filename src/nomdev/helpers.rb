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

* [Detecting Operating Systems in Ruby](http://stackoverflow.com/questions/11784109/detecting-operating-systems-in-ruby/13586108#13586108)

=end

require 'rbconfig'

require "#{NOMDEV_SRC_DIR}/config.rb"

def platform
  os ||= (
    host_os = RbConfig::CONFIG['host_os']

    case host_os
      when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
        "windows" #:windows
      when /darwin|mac os/
        "macosx" #:macosx
      when /linux/
        "linux" #:linux
      when /solaris|bsd/
        "unix" #:unix
      else
        "unknown" #:unknown
    end
  )
end # platform

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

def run( cmd, *args, opts )
  if opts.dry_run
    puts( cmd, *args )
  else
    system( cmd, *args ) or quit
  end
end
