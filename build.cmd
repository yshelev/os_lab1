@echo off
chcp 65001 > nul

echo git pull
git pull

echo cmake
cmake -G "MinGW Makefiles" -S . -B build

echo compile
cmake --build build