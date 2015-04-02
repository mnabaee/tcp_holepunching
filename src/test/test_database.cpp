#include "../database/ClientList.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
	ClientList m_clients;
	m_clients.setFileName("database.txt");
	int res = m_clients.addClient("mahdy","mahdy");
	printf("%d \n",res);
	return 0;
}
