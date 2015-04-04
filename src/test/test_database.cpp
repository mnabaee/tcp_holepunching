#include "../database/ClientList.h"
#include <stdlib.h>
#include <stdio.h>

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

	pass = "heyy";
	printf("%d  \n",m_clients.writeClientPasswd("mahdy1",pass));


	return 0;

}
