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
    + Efficient, consistent, and portable design for file and network IO
    + Flexible design to allow audio, 3D mesh, video, and other multimedia files
- [ ] Implement a physics engine to manager object dynamics
    + Collision logic and behavior for fixed objects
    + Particle physics
    + Ragdoll physics(graphics compatibilty and modelling 3D required)
- [ ] Implement an interface for input events in SDL
    + Allow engine to bind callback functions to key and mouse events
    + Optionally support more advanced input devices such as xbox controllers
- [ ] Wrap SDL Window, Rendeder, Surface, other graphics APIs in C++
    + Allow for compatibility with OpenGL
    + Possibly generalize API for DirectX, Vulkan, etc.
- [ ] Implement a secure socket wrapper for UDP and TCP.
    + DTLS example in progress
    + TLS example done
- [ ] Design a file storage solution for game resources with considerations regarding:
    + Hashing for file names
    + Security for resource data(?)
    + Integration with client/server model to synchronize resources and optimize storage footprint
    + A cross platform file IO API that can bypass the buffer delays of std::fstream when needed
- [ ] Implement memory management to allow granular control of memory
    + Memory pool to move memory management into user space
    + Custom allocators to allow pool to be used with STL and other std C++ classes
    + Memory buffers to provide a clean and consistent C++ API for reading and writing memory blocks
- [ ] Utilize a thread pool to improve throughput on parallelizable tasks
    + Allow default optimization based on system hardware
    + Provide both a synchronized and asynchronous interface for submitting work to thread pool