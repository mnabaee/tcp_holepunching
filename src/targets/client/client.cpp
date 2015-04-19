#include "headers.h"

uint8_t m_user_id;
int m_socket_to_server, m_socket_listen;
uint16_t m_local_port_h, m_local_port_n;
uint32_t m_local_addr;
struct sockaddr_in m_sockaddr_local;
pthread_t m_t_wait_server, m_t_accept_peer, m_t_peer[MAX_CLIENTS];
std::map<uint8_t, User_Record> m_peers;

void* thread_Peer(void* in_params){
	pthread_detach(pthread_self());
	uint8_t m_peer_id = *((uint8_t*) in_params);
	free(in_params);
	std::map<uint8_t, User_Record>::iterator m_it_peer = m_peers.find(m_peer_id);
	if(m_it_peer == m_peers.end()) pthread_exit(NULL);
	int m_socket_peer = m_it_peer->second.get_socket_id();
	while(true){
		Req_Message m_req_mess;
		if(m_req_mess.recv_(m_socket_peer) == 0) continue;
		if(m_req_mess.get_index() == 70){
			if(m_it_peer->second.get_user_status() == 20){

			}
			continue;
		}		
	}
}

void* thread_Accept_Peer(void* in_params){
	pthread_detach(pthread_self());
	free(in_params);
	int cnt = 0;
	while(cnt < MAX_CLIENTS){
		struct sockaddr_in m_sockaddr_peer;
		socklen_t m_sockaddr_len_peer = sizeof(m_sockaddr_peer);
		int m_socket_peer = accept(m_socket_listen, (struct sockaddr*)&m_sockaddr_peer, &m_sockaddr_len_peer);
		uint32_t m_peer_global_addr = m_sockaddr_peer.sin_addr.s_addr;
		char m_peer_global_addr_str[128];
		strcpy(m_peer_global_addr_str, inet_ntoa(m_sockaddr_peer.sin_addr));
		uint16_t m_peer_global_port = m_sockaddr_peer.sin_port;
		printf(" *** a new peer is accepted:: global_addr: %s / %d \n", m_peer_global_addr_str, ntohs(m_peer_global_port));
		uint8_t m_peer_id = 0;
		pthread_create(&m_t_peer[cnt], NULL, thread_Peer, (void*) &m_peer_id);
		cnt++;
	}
}

void* thread_Wait_Server(void* in_params){
	pthread_detach(pthread_self());	
	free(in_params);
	Rep_Message m_rep_mess;
	while(true){
		if(m_rep_mess.recv_(m_socket_to_server)==0) continue;
		if(m_rep_mess.get_index()==31){//BYE response
			printf(" * server acepted BYE!\n");
			exit(0);
		}
		if(m_rep_mess.get_index() == 41){ //connect response
			printf(" ** Peer was not found! \n");
			continue;
		}
		if(m_rep_mess.get_index() == 43){
			printf(" ** Peer is offline! \n");
			continue;
		}
		if(m_rep_mess.get_index() == 42){
			printf(" ** can not connect to yourself!\n");
			continue;
		}
		if(m_rep_mess.get_index() == 44){
			User_Record m_peer_rec;
			m_rep_mess.get_peer_record(&m_peer_rec);
			printf(" ** received record of the peer::\n");
			m_peer_rec.print();
			int m_socket_to_peer = socket(AF_INET, SOCK_STREAM, 0);
			int m_peer_id = m_peer_rec.get_user_id();
			int re_use = true;
			setsockopt(m_socket_to_peer, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use));	
			setsockopt(m_socket_to_peer, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use));	
			if(bind(m_socket_to_peer, (struct sockaddr*) &m_sockaddr_local, sizeof(m_sockaddr_local)) < 0)
				printf("Error binding socket to peer!\n");
			struct sockaddr_in m_sockaddr_global, m_sockaddr_local;
			m_peer_rec.get_sockaddr_global(&m_sockaddr_global);
			m_peer_rec.get_sockaddr_local(&m_sockaddr_local);
			int m_stat_local, m_stat_global;
			for(int try_ = 0; try_ < 10; try_++){
				m_stat_global = connect(m_socket_to_peer, (struct sockaddr*) &m_sockaddr_global, sizeof(m_sockaddr_global));
				m_stat_local = connect(m_socket_to_peer, (struct sockaddr*) &m_sockaddr_local, sizeof(m_sockaddr_local));
				if(m_stat_global < 0 && m_stat_local < 0){
					printf(" *** try %d to connect failed! \n", try_);
					usleep(500000);
				}else{
					printf(" *** try %d to connect was successful! \n", try_);
					m_peer_rec.set_socket_id(m_socket_to_peer);
					m_peer_rec.set_user_status(20);
					m_peers.insert(std::pair<uint8_t, User_Record>(m_peer_rec.get_user_id(), m_peer_rec));
					Req_Message m_req_mess(70, m_user_id);
					m_req_mess.send_(m_socket_to_peer);
					pthread_t m_t_peer_;
					uint8_t *peer_id_ = new uint8_t;
					*peer_id_ = m_peer_id;
					pthread_create(&m_t_peer_, NULL, thread_Peer, (void*) peer_id_);
					break;
				}
			}
			continue;
		}
		if(m_rep_mess.get_index() == 45){
			User_Record m_peer_rec;
			m_rep_mess.get_peer_record(&m_peer_rec);
			printf(" ** received record of the peer (passive)::\n");
			m_peer_rec.print();
			int m_socket_to_peer = socket(AF_INET, SOCK_STREAM, 0);
			int re_use = true;
			setsockopt(m_socket_to_peer, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use));	
			setsockopt(m_socket_to_peer, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use));	
			if(bind(m_socket_to_peer, (struct sockaddr*) &m_sockaddr_local, sizeof(m_sockaddr_local)) < 0)
				printf("Error binding socket to peer!\n");
			struct sockaddr_in m_sockaddr_global, m_sockaddr_local;
			m_peer_rec.get_sockaddr_global(&m_sockaddr_global);
			m_peer_rec.get_sockaddr_local(&m_sockaddr_local);
			int m_stat_local, m_stat_global;
			for(int try_ = 0; try_ < 10; try_++){
				m_stat_global = connect(m_socket_to_peer, (struct sockaddr*) &m_sockaddr_global, sizeof(m_sockaddr_global));
				m_stat_local = connect(m_socket_to_peer, (struct sockaddr*) &m_sockaddr_local, sizeof(m_sockaddr_local));
				if(m_stat_global < 0 && m_stat_local < 0){
					printf(" *** try %d to connect failed! \n", try_);
					usleep(400000);
				}else{
					printf(" *** try %d to connect was successful! \n", try_);
					break;
				}
			}
			continue;
		}
		
	}
}

int main(){
	int m_user_id_int;
	printf("Enter user-id:");
	std::cin >> m_user_id_int;
	m_user_id = (uint8_t) m_user_id_int;
	srand(time(NULL));
	m_local_port_h = rand() % 10000 + 10000;
	m_local_port_n = ntohs(m_local_port_h);
	struct sockaddr_in m_sockaddr_to_server;
	memset(&m_sockaddr_to_server, 0, sizeof(m_sockaddr_to_server));
	memset(&m_sockaddr_local, 0, sizeof(m_sockaddr_local));
	m_sockaddr_local.sin_family = AF_INET;
	m_sockaddr_local.sin_addr.s_addr = htonl(INADDR_ANY);
	m_sockaddr_local.sin_port = m_local_port_n;
	m_sockaddr_to_server.sin_family = AF_INET;
	m_sockaddr_to_server.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	m_sockaddr_to_server.sin_port = htons(SERVER_PORT);
	m_socket_to_server = socket(AF_INET, SOCK_STREAM, 0);
	m_socket_listen = socket(AF_INET, SOCK_STREAM, 0);
	int re_use = true;
	setsockopt(m_socket_to_server, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use));	
	setsockopt(m_socket_listen, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(re_use));
	setsockopt(m_socket_to_server, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use));
	setsockopt(m_socket_listen, SOL_SOCKET, SO_REUSEPORT, &re_use, sizeof(re_use));
	if(bind(m_socket_to_server, (struct sockaddr*) &m_sockaddr_local, sizeof(m_sockaddr_local)) < 0){
		printf("Error binding socket to server! \n");
		return 0;
	}
	if(bind(m_socket_listen, (struct sockaddr*) &m_sockaddr_local, sizeof(m_sockaddr_local)) < 0){
		printf("Error binding socket listen! \n");
		return 0;
	}
	if(connect(m_socket_to_server, (struct sockaddr*)& m_sockaddr_to_server, sizeof(m_sockaddr_to_server)) < 0){
		printf("Error connecting to server! \n");
	//	return 0;
	}
	struct ifreq m_ifr;
	m_ifr.ifr_addr.sa_family = AF_INET;
	strncpy(m_ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(m_socket_to_server, SIOCGIFADDR, &m_ifr);
	struct in_addr m_in_addr = ((struct sockaddr_in*) & (m_ifr.ifr_addr))->sin_addr;
	m_local_addr = m_in_addr.s_addr;
	char m_local_ip[128];
	strcpy(m_local_ip, inet_ntoa(m_in_addr));
	printf(" Local Address: %s / %d \n", m_local_ip, m_local_port_h);


	Req_Message m_init_req_mess(10, m_user_id);
	printf(" user-id: %u \n", m_init_req_mess.get_user_id());
	m_init_req_mess.set_local_addr(m_local_addr, m_local_port_n);
	m_init_req_mess.send_(m_socket_to_server);
	Rep_Message m_init_rep_mess;
	m_init_rep_mess.recv_(m_socket_to_server);
	if(m_init_rep_mess.get_index() != 10){
		printf("Error registering the user!\n");
		return 0;
	}
	User_Record m_my_rec;
	m_init_rep_mess.get_peer_record(&m_my_rec);
	printf(" ** connected to the server:: \n");
	m_my_rec.print();
	if(listen(m_socket_listen, MAX_CLIENTS) < 0){
		printf("Error Listening to port!\n");
		return 0;
	}
	pthread_create(&m_t_wait_server, NULL, thread_Wait_Server, NULL);
	pthread_create(&m_t_accept_peer, NULL, thread_Accept_Peer, NULL);

	while(true){
		int cmd_;
		std::cin >> cmd_;
		if(cmd_ >=2000 && cmd_< 3000){
			printf(" * initiating BYE! \n");
			Req_Message m_req_mess(30, m_user_id);
			m_req_mess.send_(m_socket_to_server);
			continue;
		}
		if(cmd_ >= 3000 && cmd_< 4000){
			printf(" * initiating peer search... \n");
			int peer_id = cmd_ % 100;
			Req_Message m_req_mess(40, m_user_id, (uint8_t) peer_id);
			m_req_mess.send_(m_socket_to_server);
			continue;
		}
		printf(" --WRONG COMMAND! \n");
	}

}

