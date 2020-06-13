@echo off
if not exist build mkdir build
cd build
cmake ..
msbuild -verbosity:m -m:8 /property:Configuration=Release ALL_BUILD.vcxproj
cd ..