#!/bin/sh

echo "$(basename $1)_dep = declare_dependency("
echo "include_directories: '.'"
echo "sources: ["
for file in *.c; do
    echo "'${file}',"
done
echo '])'
