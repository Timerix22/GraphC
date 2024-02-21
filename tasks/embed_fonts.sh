#!/bin/bash
cd fonts
SRC_C=""
HEADER="generated/fonts_embedded.h"

function append_def_to_header {
    local c_src_file=$1
    local c_var_name=$2
    local header_file=$3
    echo "#define $c_var_name $(echo $c_var_name | sed 's/font_//')" >> "$header_file"
    sed '3!d;' $c_src_file | sed 's/const/static const/' >> "$header_file"
    sed '4!d;' $c_src_file | sed 's/const/extern const/' | sed 's/\[.*/[];/' >> "$header_file"
}

rm -rf generated
mkdir generated
mkdir generated/src
$CMP_CPP binary_to_compressed_c.cpp -o generated/binary_to_compressed_c.exe
for ttf_file in $(ls *.ttf); do
    c_var_name="font_$(basename $ttf_file .ttf | tr ' ' '_' | tr '-' '_')"
    c_src_file="generated/src/$c_var_name.c"
    myprint "$ttf_file : $c_var_name > c_src_file"
    generated/binary_to_compressed_c.exe -nostatic "$ttf_file" "$c_var_name" > "$c_src_file"
    SRC_C="$SRC_C fonts/$c_src_file"
    append_def_to_header "$c_src_file" "$c_var_name" "$HEADER"
done
cd ..
mkdir -p src/generated/
cp -v fonts/$HEADER src/$HEADER
myprint "${GREEN}font arrays external definitions have been written to ${CYAN}src/$HEADER"

# delete old objects
clean_dir "$OBJDIR/objects"
# compiling
compile_c "$C_ARGS" "$SRC_C"
pack_static_lib "$STATIC_LIB_FILE"
# moving lib file
mv "$OUTDIR/$STATIC_LIB_FILE" libs/
mkdir -p "libs"
myprint "$OUTDIR/$STATIC_LIB_FILE -> libs/"
