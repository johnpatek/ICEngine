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

- [ ] Design a serializable model for game objects
    + Select a framework for object marshalling(JSON, CapNProto, etc.)
    + Efficient, consistent, and portable design for file and network IO
    + Flexible design to allow audio, 3D mesh, video, and other multimedia files
- [ ] Implement a physics engine to manager object dynamics
    + Collision logic and behavior for fixed objects
    + Particle physics
    + Ragdoll physics(graphics compatibilty required)
- [ ] Implement an interface for input events in SDL
    + Allow engine to bind callback functions to key and mouse events
    + Optionally support more advanced input devices such as xbox controllers
- [ ] Wrap SDL Window, Rendeder, Surface, other graphics APIs in C++
    + Create interface for reading renderable game objects
    + Allow for compatibility with OpenGL
    + Possibly generalize API for DirectX, Vulkan, etc.
- [ ] Add networking features
    + TLS for secure TCP and UDP sockets.
    + Basic http(s) functionality for REST API, webpages etc.
    + Consider bluetooth and other protocols to allow for wireless controllers, microphones, etc.
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
- [ ] Documentation
    + Add doxygen comments to header files
    + Add document generation to CMakeLists
- [ ] Packaging and Installation
    + Add CPack and intall directives in CMakeLists
    + Find a way to generate installer files for Windows
- [ ] Unit Testing
    + Select a test framework(ctest, gtest, etc.)
    + Write a set of tests to cover API
    + Add build instructions for unit test
    + CI/CD scripts for running unit test in pipelines