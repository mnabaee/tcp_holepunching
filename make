#!/bin/bash

echo 'building binary files for server and client'
g++ server.cc -o server -lpthread
g++ client.cc -o client -lpthread
