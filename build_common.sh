cd common
git submodule update --init --recursive
if [ ! -d "build" ]; then
    mkdir build
fi
cd build
cmake ..
make install
cd ../..