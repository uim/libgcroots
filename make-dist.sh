#!/bin/sh

MAKE=make

./autogen.sh \
|| { echo 'autogen failed.' && exit 1; }

./configure --enable-maintainer-mode && $MAKE distcheck sum
