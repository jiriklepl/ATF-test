#!/bin/sh

HERE=$(pwd)

if DIR="/tmp/$(uuid)"; then
    mkdir -p "$DIR"
    echo "$DIR"

    {
        cd "$DIR" && {
            ln -s "$HERE/kernels" kernels
            c++ -oautotuner -O3 -std=c++17 "$HERE/autotuner.cpp"
            ./autotuner
        }
    }
fi
