#!/usr/bin/env bash

SCRIPT_PATH="$(readlink -f "${BASH_SOURCE[0]}")"
SCRIPT_DIR="$(dirname "${SCRIPT_PATH}")"

if [[ ! -v PREFIX_PATH ]]; then
  DONT_EXEC=1 source $SCRIPT_DIR/init_env.bash
fi 

set -euEo pipefail

pushd $SCRIPT_DIR

c++ -Wall -Ofast -march=native -std=c++17 2/a.cc -o 2/a.elf -lfmt
c++ -Wall -Ofast -march=native -std=c++23 4/d.cc -o 4/d.elf -lfmt
c++ -Wall -Ofast -march=native -std=c++23 4/dmd.cc -o 4/dmd.elf -lfmt
c++ -Wall -Ofast -march=native -std=c++23 7/g.cc -o 7/g.elf -lfmt
c++ -Wall -Ofast -march=native -std=c++23 8/h.cc -o 8/h.elf -lfmt
c++ -Wall -Ofast -march=native -std=c++23 9/i.cc -o 9/i.elf -lfmt

c++ -Wall -Ofast -march=native -std=c++23 10/j_silver_only.cc -o 10/j_silver_only.elf -lfmt
c++ -Wall -std=c++23 -Ofast -march=native $(pkg-config --cflags cbc fmt) 10/j.cc -o 10/j.elf $(pkg-config --libs cbc clp osi osi-clp osi-cbc fmt)
