#!/bin/bash

set -eux

/source/configure \
  --prefix=/tmp/local

make distcheck
make sum

sudo -H mv *.tar.* *.sum /source/

