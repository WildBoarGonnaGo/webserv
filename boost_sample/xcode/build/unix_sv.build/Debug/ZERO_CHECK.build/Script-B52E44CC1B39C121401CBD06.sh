#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/mzavelskiy/webserv/boost_sample/xcode
  make -f /Users/mzavelskiy/webserv/boost_sample/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/mzavelskiy/webserv/boost_sample/xcode
  make -f /Users/mzavelskiy/webserv/boost_sample/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/mzavelskiy/webserv/boost_sample/xcode
  make -f /Users/mzavelskiy/webserv/boost_sample/xcode/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/mzavelskiy/webserv/boost_sample/xcode
  make -f /Users/mzavelskiy/webserv/boost_sample/xcode/CMakeScripts/ReRunCMake.make
fi

