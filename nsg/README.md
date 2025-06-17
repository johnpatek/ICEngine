# NetworkSceneGraph

Abstract framework for managing scene graphs.

## About

This module is designed to introduce producer/consumer logic to scene 
graph management. By providing abstractions for IO, buffer management, 
and command processing, the interface is suitable for any system that 
requires decoupled scene graph management. The client/server model is 
the main use case, but it can be used with to other decoupled systems, 
such as IPC or threads.