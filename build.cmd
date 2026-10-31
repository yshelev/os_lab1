@echo off
chcp 65001 > nul

echo git pull
git pull

if not exist build mkdir build

echo make
make 

echo 
build/main.exe