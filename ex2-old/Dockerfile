FROM ubuntu:20.10
RUN apt-get update -qq && apt-get install -y \
    build-essential \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    gdb-arm-none-eabi \
    git \
    mercurial \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /code
COPY . /code/


RUN make


