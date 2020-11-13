#!/bin/sh

PWD="`dirname \"$0\"`"
PWD="`(cd \"$PWD\" && pwd)`"
if [ -z "$PWD" ]; then
    echo null pwd
    exit -1
fi

LD_LIBRARY_PATH=$PWD/bin:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
