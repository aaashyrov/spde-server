FROM ubuntu:20.04

MAINTAINER Alisher Ashyrov <a.ashyrov@icol.com>

WORKDIR /

RUN apt-get update && \
    apt-get install -y  apt-utils tzdata dirmngr gnupg2 lsb-release sudo
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get full-upgrade -y && \
    apt-get dist-upgrade -y && \
    rm -rf /var/lib/apt/lists/* && \
    apt-get update && \
    apt-get install -y  build-essential autoconf automake cmake curl gcc g++ git libtool make unzip wget python3 mc \
                        libprotobuf-dev protobuf-compiler libgoogle-glog-dev libgrpc++-dev libgrpc++1 libgrpc-dev \
                        protobuf-compiler-grpc

WORKDIR /opt/
COPY ./ ./spde-server/

WORKDIR /opt/spde-server/
RUN pwd && ls -la && mkdir -p -v build && cd build && cmake .. && make

EXPOSE 50051

CMD ["/opt/spde-server/build/spde-server", "0.0.0.0:50051"]