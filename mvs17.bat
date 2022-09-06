MD build
cd ./build
MD mvs17
cd ../
copy "nvToolsExt32_1.dll" "./build/mvs17/nvToolsExt32_1.dll"
cd ./build/mvs17
cmake ../../ -G "Visual Studio 15 2017" -A Win32 -T v141 -DCMAKE_CONFIGURATION_TYPES="Release;Debug;"
cd ../../