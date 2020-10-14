#!/bin/zsh
# Docs can be found https://hub.docker.com/r/ludvighz/efm32gg-devkit
docker run -it --privileged -v $(pwd):/work -v /dev/bus/usb:/dev/bus/usb ludvighz/efm32gg-devkit:codesourcery

