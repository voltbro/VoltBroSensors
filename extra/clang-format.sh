find . -iname "*.ino" -o -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
