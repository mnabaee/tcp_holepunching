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
#include <fstream>

class ClientList{
	public:
		ClientList();
		~ClientList();
		bool addClient(std::string userName, std::string passwdHash);
		bool removeClient(std::string userName);
		bool initDatabase(std::string fileName);
		bool closeDatabase();
		bool parseClientPasswd(std::string userName, std::string* clientPasswdHash);
		int writeClientPasswd(std::string userName, std::string clientPasswdHash);
		bool clearDatabase();
	private:
		std::string m_data_file_name;
		std::fstream m_fstream;
};


#endif /* CLIENTLIST_H_ */
