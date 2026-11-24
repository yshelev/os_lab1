echo обновление из git
git pull

echo настройка сборки
cmake -S . -B build

echo компиляция
cmake --build build

echo запуск
build/main
