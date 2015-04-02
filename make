#!/bin/bash

echo 'building binary files for server and client'
mkdir bin
g++ src/server.cc -o bin/server.out -lpthread
g++ src/client.cc -o bin/client.out -lpthread
g++ src/test/test_unit.cpp -o bin/test_unit.out
g++ src/test/test_p.cpp -o bin/test_p.out
g++ src/test/test_database.cpp src/database/ClientList.cpp -o bin/test_database.out
