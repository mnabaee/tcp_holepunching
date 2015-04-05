#include "../database/ClientList.h"
#include <stdlib.h>
#include <stdio.h>
#include "../utils/Logging.h"

int main(){
	ClientList m_clients;
	m_clients.setFileName("database.txt");
	//printf("%d \n",m_clients.addClient("mahdy","mahdy"));
	printf("%d \n",m_clients.addClient("mahdy1","mahdy1"));
	printf("%d \n",m_clients.addClient("1mahdy","1mahdy"));


	printf("%d \n",m_clients.removeClient("1mahdy"));
	printf("%d \n",m_clients.removeClient("mahdy"));

	std::string pass;
	printf("%d  \n",m_clients.parseClientPasswd("mahdy1",&pass));
	printf("%s \n",pass.c_str());

	//LOG_MESSAGE("INFO","HI! %d",33);

	pass = "heyy";
	printf("%d  \n",m_clients.writeClientPasswd("mahdy1",pass));

	std::string format = "HI %d";
	Logging::getInstance()->setLevel(Logging::INFO,true);

	LOG_MESSAGE(Logging::INFO,"hey %d",34);
	LogEvt(Logging::INFO,"hey %d",55);

	return 0;

}
