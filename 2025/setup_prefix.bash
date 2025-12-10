#!/usr/bin/env bash

set -euEo pipefail

CFLAGS="-march=native -Ofast -pipe -flto=auto -ffinite-math-only -fno-trapping-math -freciprocal-math -fassociative-math -funsafe-math-optimizations -fno-math-errno -fsplit-loops -funswitch-loops -ftree-loop-im  -fivopts -ftree-vectorize -fgraphite-identity -floop-interchange"
CXXFLAGS=$CFLAGS

SCRIPT_PATH="$(readlink -f "${BASH_SOURCE[0]}")"
SCRIPT_DIR="$(dirname "${SCRIPT_PATH}")"

pushd "$SCRIPT_DIR"

mkdir -p PREFIX/pfx

pushd PREFIX
if [ ! -d tmp-repos ] ; then
  mkdir tmp-repos
  pushd tmp-repos
  git clone https://github.com/coin-or/CoinUtils &
  git clone https://github.com/coin-or/osi &
  git clone https://github.com/coin-or/cgl & 
  git clone https://github.com/coin-or/clp &
  git clone https://github.com/coin-or/Cbc &
  git clone https://github.com/fmtlib/fmt &
  git clone https://github.com/kokkos/mdspan &
  wait
  GIT_DIR=clp/.git git checkout 87ffd7a6cd198ba9eaf9fa7a66eb8b1f4aafcaaf
  GIT_DIR=Cbc/.git git checkout b1169dea68d39dfb5c5527ec8e95f41a5c56ab03
  GIT_DIR=CoinUtils/.git git checkout eeaadb027bb8118aee5117152a7383dd8141b23c
  GIT_DIR=fmt/.git git checkout ec73fb72477d80926c758894a3ab2cb3994fd051
  GIT_DIR=cgl/.git git checkout 6ddd499c109ecf58ead9f7f7a1aeade6a150860a
  GIT_DIR=mdspan/.git git checkout 537053f83150f1b6f7528c1d961866d8628abc2e
  GIT_DIR=osi/.git git checkout cb16146c414843076e397370d051a168b7c9215d
  popd
  pushd tmp-repos/clp
  git apply ../../fix-ClpCholeskyUfl.patch
  popd
fi
popd


export PREFIX_PATH=$(realpath $SCRIPT_DIR/PREFIX/pfx)
export PKG_CONFIG_PATH="$PREFIX_PATH/lib/pkgconfig/"
export CPATH="$PREFIX_PATH/include/"
export LIBRARY_PATH="$PREFIX_PATH/lib/"
export LD_LIBRARY_PATH="$LIBRARY_PATH "

pushd PREFIX/tmp-repos

pushd CoinUtils
./configure --prefix=$PREFIX_PATH --without-lapack --without-netlib --without-sample --without-asl --without-glpk
make -j 20
make install
popd

pushd osi
./configure --prefix=$PREFIX_PATH
make -j 20
make install
popd

pushd clp
./configure --prefix=$PREFIX_PATH
make -j 20
make install
popd

pushd cgl
./configure --prefix=$PREFIX_PATH
make -j 20
make install
popd

pushd Cbc
./configure --prefix=$PREFIX_PATH
make -j 20
make install
popd

pushd fmt/
cmake -B build -DCMAKE_INSTALL_PREFIX=$PREFIX_PATH -DCMAKE_BUILD_TYPE=Release -DFMT_TEST=OFF -DFMT_DOC=OFF
cmake --build build -t install -j20
popd

pushd mdspan/
cmake -B build -DCMAKE_INSTALL_PREFIX=$PREFIX_PATH -DMDSPAN_ENABLE_OPENMP=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build -t install -j20
popd

popd
