@echo off
cd common
git submodule update --init --recursive
if not exist build mkdir build
cd build
cmake ..
msbuild -verbosity:m /property:Configuration=Debug /p:PlatformTarget=x86 INSTALL.vcxproj
cd ..\..