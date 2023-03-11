@echo off

mkdir ..\build
pushd ..\build
g++ -g -fuse-ld=lld ..\code\test.cpp -o test.exe -lgdi32 -I"C:/Program Files (x86)/Microsoft GDK/221002/GRDK/GameKit/Include" -L"C:/Program Files (x86)/Microsoft GDK/221002/GRDK/GameKit/Lib/amd64" -lxgameruntime -lgameinput
popd