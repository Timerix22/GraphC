#!/bin/bash

# compile fonts
if [ ! -f libs/fonts_embedded.a ]; then
    if ! make embed_fonts; then
        exit 1
    fi
fi

set -x
# copy precompiled static libs to objects
cp libs/*.a "$OBJDIR/libs/"

# copy precompiled shared libs to outdir
if [ $OS == 'WINDOWS' ]; then
    cp libs/*.dll "$OUTDIR"
else
    cp libs/*.so "$OUTDIR"
fi
set +x
