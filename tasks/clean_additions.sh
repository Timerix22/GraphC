#!/usr/bin/bash
for tmpfile in $(ls .rebuild_*.tmp); do
    try_delete_dir_or_file "$tmpfile"
done
try_delete_dir_or_file fonts/generated
try_delete_dir_or_file libs/fonts_embedded.a
