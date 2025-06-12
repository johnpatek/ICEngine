# ICE ENGINE

![alt text](https://d30y9cdsu7xlg0.cloudfront.net/png/84009-200.png "We are still using this image")

3D multiplayer game engine optimized for security and performance.

## About

This project is intended to provide a foundation for the development 
of 3D multiplayer games. It is secure by design, providing robust and 
unobtrusive solutions to mitigate cheating and tampering. The engine 
operates on a client-server model, with decoupled logic to handle the 
common functions such as rendering, input, and game state management, 
providing developers with a secure and performant solution that does 
not jeopardize the integrity of a user's system by requiring invasive 
kernel modules.

## Requirements

Both client and server have the following requirements:

+ C++17
+ CMake
+ vcpkg

vcpkg is provided as a submodule, but it can be overrided using the 
`CMAKE_TOOLCHAIN_FILE` variable during configuration.