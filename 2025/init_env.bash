#!/usr/bin/env bash

SCRIPT_PATH="$(readlink -f "${BASH_SOURCE[0]}")"
SCRIPT_DIR="$(dirname "${SCRIPT_PATH}")"

pushd $SCRIPT_DIR

if [ ! -d PREFIX/pfx ]; then
  echo "[ERROR] Could not find PREFIX/pfx directory. Exiting..."
  echo "[Hint] Did you run 'setup_prefix.bash' to generate PREFIX/pfx?"
  popd
  exit -1
fi

export PREFIX_PATH=$(realpath $SCRIPT_DIR/PREFIX/pfx)
export PKG_CONFIG_PATH="$PREFIX_PATH/lib/pkgconfig/"
export CPATH="$PREFIX_PATH/include/"
export LIBRARY_PATH="$PREFIX_PATH/lib/"
export LD_LIBRARY_PATH="$LIBRARY_PATH "

popd

echo 'YOU ARE NOW INSIDE AOC/2025 PREFIX!'
echo "PREFIX_PATH=$PREFIX_PATH"
echo "------------------------------------------------------------------"
if [[ ! -v DONT_EXEC ]]; then
  exec $SHELL
fi
