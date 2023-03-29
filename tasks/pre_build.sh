#!/bin/bash

# check if kerep static lib exists or kerep_rebuild task was executed
if [ ! -f "$OBJDIR/libs/kerep.a" ] || [ -f .rebuild_kerep.tmp ]; then
    [[ -z "$KEREP_BUILD_TASK" ]] && error "KEREP_BUILD_TASK is empty" 
    myprint "${BLUE}making kerep task <$KEREP_BUILD_TASK>"

    cd kerep
    if ! make "$KEREP_BUILD_TASK"; then
        exit 1
    fi
    cd ..

    cp kerep/bin/kerep.a $OBJDIR/libs/
    myprint "${GREEN}copied ${CYAN}kerep.a"
    rm -f .rebuild_kerep.tmp
fi
