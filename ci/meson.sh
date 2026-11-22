#!/bin/bash

set -eux

meson setup \
  --prefix=/tmp/local \
  /tmp/meson-build \
  /source
meson compile -C /tmp/meson-build
meson test -C /tmp/meson-build --print-errorlogs
meson install -C /tmp/meson-build

meson setup \
  -Db_lundef=false \
  -Db_sanitize=address \
  /tmp/meson-build-asan \
  /source
meson test -C /tmp/meson-build-asan --print-errorlogs
