@echo off

mkdir ..\build
pushd ..\build
g++ -g -fuse-ld=lld -Wmultichar ..\code\test.cpp -o test.exe -lgdi32 -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64" -lxaudio2 -I"C:/Program Files (x86)/Microsoft GDK/221002/GRDK/GameKit/Include" -L"C:/Program Files (x86)/Microsoft GDK/221002/GRDK/GameKit/Lib/amd64" -lxgameruntime -lgameinput -lole32 
popd