FROM debian:trixie-slim

RUN apt-get update && apt-get upgrade && apt-get install -y \
    build-essential \
    clang \
    clang-format \
    cmake \
    libasound2-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    libwayland-dev \
    libxkbcommon-dev \
    git

ENV RAYLIB_SOURCE_PATH="/opt/raylib"

RUN git clone https://github.com/raysan5/raylib.git $RAYLIB_SOURCE_PATH && \
    cd $RAYLIB_SOURCE_PATH && \
    mkdir build && cd build && \
    cmake -DBUILD_SHARED_LIBS=OFF .. && \
    make && \
    make install && \
    ldconfig