#!/bin/bash

# Install cmake and git
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    sudo apt-get update
    sudo apt-get install -y cmake git libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev
elif [[ "$OSTYPE" == "darwin"* ]]; then
    brew install cmake sdl2 sdl2_ttf sdl2_image sdl2_mixer
else
    echo "Unsupported OS"
    exit 1
fi


mkdir build
cd build || exit
cmake ..
make
