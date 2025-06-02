

if not exist "build\" (
    mkdir build
)

cd build

cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

make

GameOfLife.exe -w 40 -h 40 -t 100