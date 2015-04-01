#include "run_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>

int main(){

	pid_t id_ = system2("./test_unit.out ",NULL,NULL);
	printf("id_ = %d \n",id_);
	sleep(10);
	std::stringstream out;
	out << (int)id_;
	std::string str_ = "kill " + out.str();
	system(str_.c_str());

	return 0;
}
