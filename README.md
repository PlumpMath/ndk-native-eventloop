# Summary

This repository provides some examples of how to combine native C++ with Android code. Starting from the official Android tutorial,
I have implemented a C++ event loop and binded it to Java so that it can be used by Android Application.

The most important files of the project are:

* event-loop.cpp
* EventEmitter.java
* NativeWrapper.java
* MainActivity.java

You can see in the event loop implement how native threads (C++) can safely run JVM code without crashing.
