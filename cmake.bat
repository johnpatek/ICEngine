@echo off
if not exist build mkdir build
cd build
cmake ..
msbuild -verbosity:m -m:8 /property:Configuration=Debug /p:PlatformTarget=x86 ALL_BUILD.vcxproj
cd ..