#!/bin/bash
source cbuild/colors.sh
source cbuild/functions.sh
touch ".rebuild_$1.tmp"
myprint "${YELLOW}$1.a will be rebuilt in the next build task"
