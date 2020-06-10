@echo off
cd common
git submodule update --init --recursive
if not exist build mkdir build
cd build
cmake ..
msbuild -verbosity:m /property:Configuration=Release INSTALL.vcxproj
cd ..\..