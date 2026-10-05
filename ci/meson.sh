#!/bin/bash

set -eux

meson setup \
  --prefix=/tmp/local \
  /tmp/meson-build \
  /source
meson compile -C /tmp/meson-build
meson install -C /tmp/meson-build
