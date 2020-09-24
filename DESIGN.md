# Design Proposal

## Protocol

All data is in the form of a message.

### Message Structure

Each message will start with a header containing at least:
+ Message type
+ Message length

There are 5 message types:
+ Join: Client joins game
+ Create: Client creates new resource
+ Update: Client changes resource property
+ Delete: Client removes resource
+ Quit: Client leaves game

## Client

The client will form persistent connections to the server and will manage all of its own resource data. If it is modifying a shared resource, it must create a message and send it to the server.

## Server

The server is responsible for receiving messages from the clients, and handling them appropriately:
