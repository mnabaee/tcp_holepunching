/*
 * ClientList.h
 *
 *  Created on: 2015-03-28
 *      Author: mahdy
 */

#ifndef CLIENTLIST_H_
#define CLIENTLIST_H_

#include <inttypes.h>
#include <string>

class ClientList{
	public:
		ClientList();
		~ClientList();
		bool addClient(uint32_t address, uint16_t port, std::string userName, std::string passwdHash, int peerId);
		bool removeClient(std::string userName);
		bool initDatabase(std::string fileName);
	private:
		std::string m_data_file_name;
		bool parseClientPasswd(std::string userName, std::string* clientPasswdHash);
		int writeClientPasswd(std::string userName, std::string clientPasswdHash);
};


#endif /* CLIENTLIST_H_ */
