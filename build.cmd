@echo off
chcp 65001 > nul

echo git pull
git pull

echo cmake
cmake -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=D:/gcc/bin/make.exe -S . -B build

echo compile
cmake --build build