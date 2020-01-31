#!/bin/bash
# -------------------------------------------------------------------
# Required packages for the VST 3 SDK under Linux Ubuntu distribution
# -------------------------------------------------------------------

# -------------------------------------------------------------------
# Install all needed libs
echo "=>Libs for VSTSDK -------------------------"
sudo apt-get install cmake git "libstdc++6" -y

# -------------------------------------------------------------------
# Install all needed libs for VSTGUI
echo "=>Libs for VSTGUI -------------------------"
sudo apt-get install libx11-xcb-dev \
libxcb-util-dev \
libxcb-cursor-dev \
libxcb-xkb-dev \
libxkbcommon-dev \
libxkbcommon-x11-dev \
libfontconfig1-dev \
libcairo2-dev \
libgtkmm-3.0-dev \
libsqlite3-dev \
libxcb-keysyms1-dev \
-y

# -------------------------------------------------------------------
# Install needed lib for Jack Audio
echo "=>Libs for Jack Audio ---------------------"
sudo apt-get install libjack-jackd2-dev -y

# Add current user to 'audio' group. Use 'deluser' to remove user again.
THIS_USER=${USER:-${USERNAME:-${LOGNAME}}}
sudo adduser ${THIS_USER} audio
# -------------------------------------------------------------------
