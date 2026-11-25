echo git pull
git pull

echo cmake
cmake -S . -B build

echo compile
cmake --build build

echo run
build/main
