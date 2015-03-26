#!/bin/bash

echo 'building binary files for server and client'
g++ src/server.cc -o bin/server.out -lpthread
g++ src/client.cc -o bin/client.out -lpthread
