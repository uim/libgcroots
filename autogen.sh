#! /bin/sh

set -e

${AUTORECONF:-autoreconf} --force --install "$@"
echo
echo "Ready to run './configure'."
echo
