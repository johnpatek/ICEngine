@echo off
if not exist build mkdir build
cd build
cmake ..
msbuild -verbosity:m ALL_BUILD.vcxproj
cd ..