@echo off
cd common
git submodule update --init --recursive
if not exist build mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
msbuild -verbosity:m INSTALL.vcxproj
cd ..\..