#!/bin/sh

echo "$1_dep = declare_dependency("
echo "include_directories: '.'"
echo "sources: ["
for file in *.c; do
    echo "'${file}',"
done
echo '])'
