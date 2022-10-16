FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive

SHELL ["/bin/bash", "-c"] 

USER root

RUN apt-get update \
    && apt-get install -y \
    cmake \
    git \
    python3 \
    python3-pip \
    libusb-1.0-0

ARG MICROPYTHON_DIR=/micropython

RUN git clone https://github.com/micropython/micropython.git ${MICROPYTHON_DIR}

WORKDIR ${MICROPYTHON_DIR}

RUN source tools/ci.sh && ci_esp32_idf44_setup
RUN source tools/ci.sh && ci_esp32_build
