#!/bin/bash

echo 'building binary files for server and client'
mkdir bin
g++ src/server.cc -o bin/server.out -lpthread
g++ src/client.cc -o bin/client.out -lpthread
g++ src/test.cc -o bin/test.out
g++ src/test_1.cc -o bin/test_1.out