MD build
cd ./build
MD mvs19
cd ../
copy "nvToolsExt32_1.dll" "./build/mvs19/nvToolsExt32_1.dll"
cd ./build/mvs19
cmake ../../ -G "Visual Studio 16 2019" -A Win32 -T v142 -DCMAKE_CONFIGURATION_TYPES="Release;Debug;"
cd ../../