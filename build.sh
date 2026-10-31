echo git pull
git pull

mkdir -p build

echo make
make

echo
build/main
