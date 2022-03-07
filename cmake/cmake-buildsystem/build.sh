#!/usr/bin/env bash
# ==============================================================================
# This to build project
# ==============================================================================

RDIR=$(dirname $(readlink -f $0))
BUILDDIR=${RDIR}/build

function build ()
{
  mkdir -p $BUILDDIR  \
    && cd $BUILDDIR \
    && cmake .. -DCMAKE_BUILD_TYPE=Release $@ \
    		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    #&& cmake --build . --config Release -- -j4

  # For cmake <= 2.6
  make -j4

  return $?
}

build $@

exit $?

