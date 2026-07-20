FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libsqlite3-dev \
    libasio-dev \
    nlohmann-json3-dev

WORKDIR /agricy

COPY . .

RUN mkdir build && \
    cd build && \
    cmake ../app && \
    make

CMD ["./build/agricy"]