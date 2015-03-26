#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;
int main(){

	string command = "echo HI1 >> test.log";
	system(command.c_str());
	command = "./test_1.out >> test.log &";
	system(command.c_str());

	return 0;
}
