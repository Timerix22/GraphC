#!/bin/bash

# compile fonts
if [ ! -f libs/fonts_embedded.a ]; then
    if ! make embed_fonts; then
        exit 1
    fi
fi

# copy all precompiled libs
cp libs/* $OBJDIR/libs/
