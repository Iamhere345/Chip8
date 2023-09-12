#!/bin/bash

INCLUDE_DIR='include'
SOURCE='src/*'
LIBS='-lsdl2'
OUT_BIN='build/chip8'

g++ $SOURCE -I$INCLUDE_DIR $LIBS -o $OUT_BIN

if [[ ${1} ]]
then
    ./$OUT_BIN
fi