echo pull new version from git
git pull

mkdir -p build

echo compile files
make

echo exec file
build/main
