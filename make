#!/bin/bash

echo 'building binary files for server and client'
mkdir bin
g++ src/targets/server/server.cpp -o bin/server.out -lpthread
g++ src/targets/client/client.cpp -o bin/client.out -lpthread
g++ src/test/test_unit.cpp -o bin/test_unit.out
g++ src/test/test_p.cpp -o bin/test_p.out
g++ src/test/test_database.cpp src/database/ClientList.cpp src/utils/Logging.cpp -o bin/test_database.out
