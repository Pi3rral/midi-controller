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
ARG MICROPYTHON_VERSION=v1.19.1

RUN git clone -b ${MICROPYTHON_VERSION} https://github.com/micropython/micropython.git ${MICROPYTHON_DIR}

WORKDIR ${MICROPYTHON_DIR}

RUN source tools/ci.sh && ci_esp32_idf44_setup

COPY src/midicontroller ${MICROPYTHON_DIR}/ports/esp32/modules/
COPY src/webserver ${MICROPYTHON_DIR}/ports/esp32/modules/
COPY src/wifi.py ${MICROPYTHON_DIR}/ports/esp32/modules/
COPY src/main.py ${MICROPYTHON_DIR}/ports/esp32/modules/boot.py

RUN source tools/ci.sh && ci_esp32_build
