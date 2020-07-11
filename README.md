# ICE ENGINE

![alt text](https://d30y9cdsu7xlg0.cloudfront.net/png/84009-200.png "We are still using this image")

## About this Project

The purpose of this project is to provide a modern C++ application
framework.

## How to build

First, build the common dependencies. Then, run the build scripts.

### Windows

```powershell
build_common.bat
cmake.bat
```

### Linux

```sh
./build_common.sh
./cmake.sh
```
## TODO

- [ ] Design a serializable model for game objects(JSON, CapNProto, etc.)
- [ ] Implement a physics engine to manager object dynamics
- [ ] Implement an interface for input events in SDL
- [ ] Wrap SDL Window, Rendeder, Surface, other graphics APIs in C++
- [x] Implement a secure socket wrapper for UDP and TCP.
    + DTLS example in progress
    + TLS example done
- [ ] Design a file storage solution for game resources with considerations regarding:
    + Hashing for file names
    + Security for resource data(>)
    + Integration with client/server model to synchronize resources and optimize storage footprint
    