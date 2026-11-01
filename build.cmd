@echo off
chcp 65001 > nul

echo pull new version from git
git pull

if not exist build mkdir build

echo compile files
make 

echo exec file
build\main.exe