#!/bin/sh

while getopts :c: flag
do
    case "${flag}" in
        c) config=${OPTARG};;
    esac
done

config="${config:-Debug}"
cmake -S . -B build -DCMAKE_BUILD_TYPE=$config
cp build/compile_commands.json compile_commands.json
