# evstream

Decoupled event management framework.

## About

This module provides an interface for device and UI event processing, 
with a decoupled interface to logically separate event detection from 
event handling. It specifically targets applications that wish to log 
their events and handle them elsewhere, such as a threadpool, IPC, or 
a remote device. The high level architecture is based on an enhanced 
sender/receiver model.