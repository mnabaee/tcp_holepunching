#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>
#include <utility>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <time.h>


//#define SERVER_ADDR "192.168.2.14"
#define SERVER_ADDR "54.68.127.121"
#define SERVER_PORT 5005
#define MAX_CLIENTS 50
#define BUFF_SIZE 20

class User_Record{
	private:
		uint8_t m_user_id;
		uint16_t m_local_port;
		uint16_t m_global_port;
		uint32_t m_local_addr;
		uint32_t m_global_addr;
		uint8_t m_user_status;
		uint8_t m_socket;
	public:
	User_Record(uint8_t id = 0, uint8_t stat = 0){
		this->m_user_id = id;
		this->m_user_status = stat;
	}
	void set_user_status(uint8_t stat){
		this->m_user_status = stat;
	}
	void set_local_addr(uint32_t addr_, uint16_t port_){
		this->m_local_addr = addr_;
		this->m_local_port = port_;
	}
	void set_global_addr(uint32_t addr_, uint16_t port_){
		this->m_global_addr = addr_;
		this->m_global_port = port_;
	}
	void set_socket_id(uint8_t s_id){
		this->m_socket = s_id;
	}
	uint8_t get_user_id(){return this->m_user_id;}
	uint8_t get_user_status(){return this->m_user_status;}
	uint16_t get_local_port(){return this->m_local_port;}
	uint16_t get_global_port(){return this->m_global_port;}
	uint32_t get_local_addr(){return this->m_local_addr;}
	uint32_t get_global_addr(){return this->m_global_addr;}
	uint8_t get_socket_id(){return this->m_socket;}
	void get_sockaddr_global(struct sockaddr_in* out_sockaddr){
		memset(out_sockaddr, 0, sizeof(struct sockaddr_in));
		out_sockaddr->sin_family = AF_INET;
		out_sockaddr->sin_addr.s_addr = this->m_global_addr;
		out_sockaddr->sin_port = this->m_global_port;
	}
	void get_sockaddr_local(struct sockaddr_in* out_sockaddr){
		memset(out_sockaddr, 0, sizeof(struct sockaddr_in));
		out_sockaddr->sin_family = AF_INET;
		out_sockaddr->sin_addr.s_addr = this->m_local_addr;
		out_sockaddr->sin_port = this->m_local_port;
	}
	void get_local_addr(char* out_){
		uint32_t l_addr = this->get_local_addr();
		strcpy(out_, inet_ntoa(* (struct in_addr*)& l_addr ));
	}
	void get_global_addr(char* out_){
		uint32_t g_addr = this->get_global_addr();
		strcpy(out_, inet_ntoa(* (struct in_addr*)& g_addr));
	}
	void copy_to(User_Record* to_rec){
		to_rec->m_user_id = this->m_user_id;
		to_rec->m_local_port = this->m_local_port;
		to_rec->m_global_port = this->m_global_port;
		to_rec->m_local_addr = this->m_local_addr;
		to_rec->m_global_addr = this->m_global_addr;
		to_rec->m_user_status = this->m_user_status;	
	}
	void copy_from(User_Record* from_rec){
		this->m_user_id = from_rec->m_user_id;
		this->m_local_port = from_rec->m_local_port;
		this->m_global_port = from_rec->m_global_port;
		this->m_local_addr = from_rec->m_local_addr;
		this->m_global_addr = from_rec->m_global_addr;
		this->m_user_status = from_rec->m_user_status;
	}
	void print(){
		char l_addr[128], g_addr[128];
		this->get_local_addr(l_addr);
		this->get_global_addr(g_addr);
		//printf("user-id=%d, user-status=%d, local_addr=%s / %d, global_addr=%s / %d \n", this->get_user_id(), this->get_user_status(), l_addr, this->get_local_port(), g_addr, this->get_global_port());
		printf("user-id=%u, user-status=%u, local_addr=%s / %u, global_addr=%s / %u \n", this->get_user_id(), this->get_user_status(), l_addr, ntohs(this->get_local_port()), g_addr, ntohs(this->get_global_port()));
		//printf("%u .. %u \n",this->get_local_addr(), this->get_global_addr());
	}
	void serialize(char* out_stream){
		char* q = out_stream; //of 14 bytes
		*q = this->m_user_id; q++;
		*q = this->m_local_port; q++;
		*q = this->m_local_port >> 8; q++;
		*q = this->m_global_port; q++;
		*q = this->m_global_port >> 8; q++;
		*q = this->m_local_addr; q++;
		*q = this->m_local_addr >> 8; q++;
		*q = this->m_local_addr >> 16; q++;
		*q = this->m_local_addr >> 24; q++;
		*q = this->m_global_addr; q++;
		*q = this->m_global_addr >> 8; q++;
		*q = this->m_global_addr >> 16; q++;
		*q = this->m_global_addr >> 24; q++;
		*q = this->m_user_status;
	}
	void deserialize(char* in_stream){
		char* q = in_stream;	//of 14 bytes
		this->m_user_id = (uint8_t) *q; q++;
		this->m_local_port = (uint8_t) *q; q++;
		this->m_local_port += (uint8_t) *q << 8; q++;
		this->m_global_port = (uint8_t) *q; q++;
		this->m_global_port += (uint8_t) *q << 8; q++;
		this->m_local_addr =  (uint8_t) *q; q++;
		this->m_local_addr +=  (uint8_t) *q << 8; q++;
		this->m_local_addr +=  (uint8_t) *q << 16; q++;
		this->m_local_addr +=  (uint8_t) *q << 24; q++;
		this->m_global_addr =  (uint8_t) *q; q++;
		this->m_global_addr +=  (uint8_t) *q << 8; q++;
		this->m_global_addr +=  (uint8_t) *q << 16; q++;
		this->m_global_addr +=  (uint8_t) *q << 24; q++;
		this->m_user_status = (uint8_t) *q;
	}
};

class Req_Message{
	private:
		uint8_t m_index;
		uint8_t m_user_id;
		uint8_t m_peer_id;
		
		uint16_t m_local_port;
		uint32_t m_local_addr;
		
	public:
		Req_Message(uint8_t index = 0, uint8_t u_id = 0, uint8_t p_id = 0){
			this->m_index = index;
			this->m_user_id = u_id;
			this->m_peer_id = p_id;
		}
		void set_local_addr(uint32_t addr_, uint16_t port_){
			this->m_local_port = port_;
			this->m_local_addr = addr_;
		}
		uint16_t get_local_port(){return this->m_local_port;}
		uint32_t get_local_addr(){return this->m_local_addr;}
		uint8_t get_index(){return this->m_index;}
		uint8_t get_user_id(){return this->m_user_id;}
		uint8_t get_peer_id(){return this->m_peer_id;}
		void serialize(char* out_stream){
			char* q = out_stream; //15bytes
			*q = this->m_index; q++;
			*q = this->m_user_id; q++;
			*q = this->m_peer_id;	q++;
			*q = this->m_local_port; q++;
			*q = this->m_local_port >> 8; q++;
			*q = this->m_local_addr; q++;
			*q = this->m_local_addr >> 8; q++;
			*q = this->m_local_addr >> 16; q++;
			*q = this->m_local_addr >> 24; 
		}
		void deserialize(char* in_stream){
			char* q = in_stream;
			this->m_index = (uint8_t) *q; q++;
			this->m_user_id = (uint8_t) *q; q++;
			this->m_peer_id = (uint8_t) *q; q++;
			this->m_local_port = (uint8_t) *q; q++;
			this->m_local_port += (uint8_t) *q << 8; q++;
			this->m_local_addr = (uint8_t) *q; q++;
			this->m_local_addr += (uint8_t) *q << 8; q++;
			this->m_local_addr += (uint8_t) *q << 16; q++;
			this->m_local_addr += (uint8_t) *q << 24; 
		}
		int recv_(int m_socket){
			char m_recv_buff[BUFF_SIZE];
			ssize_t m_recv_bytes = recv(m_socket, m_recv_buff, sizeof(m_recv_buff), 0);
			if(m_recv_bytes < BUFF_SIZE) return 0;
			this->deserialize(m_recv_buff);
			return 1;
		}
		void send_(int m_socket){
			char m_send_buff[BUFF_SIZE];
			this->serialize(m_send_buff);
			send(m_socket, m_send_buff, sizeof(m_send_buff), 0);
		}
};

class Rep_Message{
	private:
		uint8_t m_index;
		User_Record m_peer_record;
	public:
		Rep_Message(uint8_t index = 0){
			this->m_index = index;
		}
		uint8_t get_index(){return this->m_index;}
		void serialize(char* out_stream){
			char* q = out_stream; //15bytes
			*q = this->m_index; q++;
			this->m_peer_record.serialize(q);
		}
		void deserialize(char* in_stream){
			char* q = in_stream;
			this->m_index = *q; q++;
			this->m_peer_record.deserialize(q);
		}
		void get_peer_record(User_Record* rec_to){
			this->m_peer_record.copy_to(rec_to);
		}
		void set_peer_record(User_Record* rec_from){
			this->m_peer_record.copy_from(rec_from);
		}
		int recv_(int m_socket){
			char m_recv_buff[BUFF_SIZE];
			ssize_t m_recv_bytes = recv(m_socket, m_recv_buff, sizeof(m_recv_buff), 0);
			if(m_recv_bytes < BUFF_SIZE) return 0;
			this->deserialize(m_recv_buff);
			return 1;
		}
		void send_(int m_socket){
			char m_send_buff[BUFF_SIZE];
			this->serialize(m_send_buff);
			send(m_socket, m_send_buff, sizeof(m_send_buff), 0);
		}
};




