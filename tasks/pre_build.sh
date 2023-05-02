#!/bin/bash

# if $lib_project.a doesn't exist or rebuild_* task was executed, builds static lib
function handle_static_dependency {
    local lib_project="$1"
    local lib_build_task="$2"
    if [ ! -f "$OBJDIR/libs/$lib_project.a" ] || [ -f .rebuild_$lib_project.tmp ]; then
        [[ -z "$lib_build_task" ]] && error "lib_build_task is empty" 
        myprint "${BLUE}making $lib_project task <$lib_build_task>"

        cd $lib_project
        if ! make "$lib_build_task"; then
            exit 1
        fi
        cd ..

        cp $lib_project/bin/$lib_project.a $OBJDIR/libs/
        myprint "${GREEN}copied ${CYAN}$lib_project.a"
        rm -f .rebuild_$lib_project.tmp
    fi
}

if [ ! -f libs/fonts_embedded.a ]; then
    if ! make embed_fonts; then
        exit 1
    fi
fi

handle_static_dependency kerep $DEPS_BUILD_TASK
handle_static_dependency imgui $DEPS_BUILD_TASK
handle_static_dependency imgui-node-editor $DEPS_BUILD_TASK
handle_static_dependency imnodes $DEPS_BUILD_TASK

# copy all precompiled libs
cp libs/* $OBJDIR/libs/
