# ICEngine Protocol

Transport agnostic protocol layer.

## About

The purpose of this module is to facilitate communication. It passes 
data between client and server without regard for transport protocol 
or backend implementation details. It is designed to work with gRPC, 
but can be ported to support REST/websockets or raw TCP. Abstractions 
have been introduced such that the only requirement is the ability to 
process messages asynchronously, as to prevent traffic handling from 
introducing excessive latency to the renderer, or other components of 
the engine with high performance expectations.

## Structure

The protocol can be logically divided into a few sections:

`auth` - Handles authentication and client ID management. At the most 
basic level, it provides IDs to identify clients, but can be extended 
to perform actual authentication through user defined messages.

`nsg` - Scene graph synchronization between the client and server. It 
is built on top of the `nsg` module, using its IO interface to allow 
the producer and consumer objects to work directly with the protocol.

`evstream` - Event handling based on the `evstream` module.