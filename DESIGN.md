# Design Proposal

### Client

+ Start:
    + Initialize system services:
        + OpenSSL, SDL, threadpool, etc.
    + Start subsystems:
        + Graphics: textures, windows
        + Physics: collisions, friction, particles
        + Network: encryption keys, sockets
        + Devices: keyboard, mouse, game controller 
+ Run:
    + Event loop
        + Each subsystem is polled for events
        + Events are submitted with priority
        + Priority threadpool executes callbacks
    + Objects
        + Visibility: unique, shared
        + Lifecycle: temporary, persistent
+ Stop:
    + Quit system services
    + Delete unique objects

### Server

+ Start
    + Initialize system services:
        + OpenSSL
+ Run
    + Listen for incoming connections
    + Handle requests
+ Stop
    + Delete shared objects

## Protocol Design

### Request Header
+ Command: post, poll
+ Path/key: Up to 255 bytes
+ Timestamp: time of last local update
+ Length
### Request Body
+ byte buffer

### Response Header
+ Status: post, poll
+ Path/key: Up to 255 bytes
+ Timestamp: time of last remote update
+ Length