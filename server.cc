#include "headers.h"

int m_socket_server;
struct sockaddr_in m_sockaddr_server;
pthread_t m_t_accept;
pthread_t m_t_users[MAX_CLIENTS];
std::map<uint8_t, User_Record> m_users;

void* thread_Handle_Client(void* in_params){
	pthread_detach(pthread_self());
	uint8_t m_user_id = * ((int*) in_params);
	std::map<uint8_t, User_Record>::iterator m_it = m_users.find(m_user_id);
	if(m_it == m_users.end()){
		printf("Error 1001\n");
		pthread_exit(NULL);
	}
	uint8_t m_socket_client = m_it->second.get_socket_id();
	free(in_params);
	while(true){
		Req_Message m_req_mess;
		if(m_req_mess.recv_(m_socket_client)==0) continue;
		std::map<uint8_t, User_Record>::iterator m_it_user = m_users.find(m_req_mess.get_user_id());
		if(m_it_user == m_users.end()){
			printf("Error 1002\n");
			continue;
		}
		if(m_req_mess.get_index() == 10) continue;
		if(m_req_mess.get_index() == 30){ //BYE!
			m_it->second.set_user_status(3);
			Rep_Message m_rep_mess(31);
			m_rep_mess.send_(m_socket_client);
			printf(" -- Client id=%d left the server. \n", m_user_id);
			pthread_exit(NULL);
		}
		if(m_req_mess.get_index() == 40){ //initiate P2P
			uint8_t m_peer_id = m_req_mess.get_peer_id();
			std::map<uint8_t, User_Record>::iterator m_it_peer = m_users.find(m_peer_id);
			if(m_it_peer == m_users.end()){
				Rep_Message m_rep_mess(41);
				m_rep_mess.send_(m_socket_client);
				printf(" ** Not found client %d for client %d \n", m_peer_id, m_user_id);
				continue;
			}else{
				if(m_it_peer == m_it){
					Rep_Message m_rep_mess(42);
					m_rep_mess.send_(m_socket_client);
					printf(" ** can not connect to yourself! id=%d \n",m_user_id);
					continue;
				}else{
					if(m_it_peer->second.get_user_status() != 1){
						Rep_Message m_rep_mess(43);
						m_rep_mess.send_(m_socket_client);
						printf(" ** peer %d was offline for user %d ! \n",m_peer_id, m_user_id);
					}else{
						Rep_Message m_rep_mess(44);
						m_rep_mess.set_peer_record(&m_it_peer->second);
						m_rep_mess.send_(m_socket_client);
						Rep_Message m_rep_mess_peer(45);
						m_rep_mess.set_peer_record(&m_it->second);
						uint8_t m_socket_peer = m_it_peer->second.get_socket_id();
						m_rep_mess.send_(m_socket_peer);
						printf(" ** Found client %d for client %d \n", m_peer_id, m_user_id);
						m_it_peer->second.print();
						continue;
					}
				}
			}
		}
	}
	
}

void* thread_Accept(void* in_params){
	pthread_detach(pthread_self());
	//free(in_params);
	int cnt_client = 0;
	while(cnt_client < MAX_CLIENTS){
		struct sockaddr_in m_sockaddr_new_client;
		socklen_t m_sockaddr_len_new_client = sizeof(m_sockaddr_new_client);
		int socket_new_client = accept(m_socket_server, (struct sockaddr*) &m_sockaddr_new_client, &m_sockaddr_len_new_client);
		uint16_t g_p = m_sockaddr_new_client.sin_port;
		uint32_t g_a = m_sockaddr_new_client.sin_addr.s_addr;
		Req_Message m_init_req_mess;
		if(m_init_req_mess.recv_(socket_new_client)==0) continue;
		if(m_init_req_mess.get_index()==10){
			User_Record m_user_rec(m_init_req_mess.get_user_id(), 1);
			m_user_rec.set_local_addr(m_init_req_mess.get_local_addr(), m_init_req_mess.get_local_port());
			m_user_rec.set_global_addr(g_a, g_p);
			m_user_rec.set_socket_id(socket_new_client);
			Rep_Message m_init_rep_mess(10);
			m_init_rep_mess.set_peer_record(&m_user_rec);
			m_init_rep_mess.send_(socket_new_client);
			uint8_t *user_id = new uint8_t;
			*user_id = m_user_rec.get_user_id();
			pthread_create(&m_t_users[cnt_client], NULL, thread_Handle_Client, (void*)user_id);
			cnt_client++;
			m_users.insert(std::pair<uint8_t, User_Record>(*user_id, m_user_rec));
			printf(" ++ New Client joined the server:: \n");
			m_user_rec.print();
		}
	}
	printf("Can not accept any more clients! \n");
}

int main(){
	m_socket_server = socket(AF_INET, SOCK_STREAM, 0);
	memset(&m_sockaddr_server, 0, sizeof(m_sockaddr_server));
	m_sockaddr_server.sin_family = AF_INET;
	m_sockaddr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	m_sockaddr_server.sin_port = htons(SERVER_PORT);
	bind(m_socket_server, (struct sockaddr*) &m_sockaddr_server, sizeof(m_sockaddr_server));
	listen(m_socket_server, MAX_CLIENTS);
	pthread_create(&m_t_accept, NULL, thread_Accept, NULL);
	printf("Waiting for clients... \n");
	while(true){
		int in_com;
		std::cin >> in_com;
		
	}

}

