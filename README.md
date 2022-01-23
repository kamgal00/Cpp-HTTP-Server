# Simple HTTP/1.1 Linux server

## Description
Simple HTTP/1.1 Linux server written in C++. Supports GET and HEAD.

### Features
* Using epoll to handle many concurrent connections
* Simple cache

## Usage
* make - build
* make run - run server
* make clean - delete binary file

Change PORT macro in HTTPServer.cpp to change server port; it's 8080 by default

App serves files in a directory where it was run
