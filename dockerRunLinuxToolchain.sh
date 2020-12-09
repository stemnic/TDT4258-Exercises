#!/bin/bash
# Docs can be found https://github.com/stemnic/efm32gg-linux-toolchain or https://hub.docker.com/r/stemnic/efm32gg-linux-toolchain

docker run -it --privileged -v $(pwd):/work -v /dev/bus/usb:/dev/bus/usb stemnic/efm32gg-linux-toolchain:latest
