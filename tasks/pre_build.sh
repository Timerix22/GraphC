#!/bin/bash

# compile fonts
if [ ! -f libs/fonts_embedded.a ]; then
    if ! make embed_fonts; then
        exit 1
    fi
fi

set -x
# copy precompiled static libs to objects
static_libs=$(find "libs/" -name '*.a')
[[ -n "$static_libs" ]] && cp $static_libs "$OBJDIR/libs/"

# copy precompiled shared libs to outdir
if [ "$OS" == 'WINDOWS' ]; then
    dynamic_libs=$(find "libs/" -name '*.dll')
else
    dynamic_libs=$(find "libs/" -name '*.so')
fi
[[ -n "$dynamic_libs" ]] && cp $dynamic_libs "$OUTDIR/"
set +x
