cmake -G Xcode -S . -B ../temp/cmake
cd ../temp/cmake
cmake --build . --config Release
cd ../../source
