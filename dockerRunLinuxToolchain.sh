#!/bin/bash
# Docs can be found https://hub.docker.com/r/ludvighz/efm32gg-devkit

docker run -it --privileged -v $(pwd):/work -v /dev/bus/usb:/dev/bus/usb stemnic/efm32gg-linux-toolchain