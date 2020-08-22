#!/bin/bash

set -eux

/source/configure \
  --prefix=/tmp/local

make distcheck
make sum

mv *.tar.* *.sum /source/

