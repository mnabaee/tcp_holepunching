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
		enum RESPONSE_VAL{
			ERROR_FILENAME,
			ERROR_OPENING,
			ERROR_ADDING,
			DONE_ADDING,
			DONE_REMOVING,
			ERROR_NOT_FOUND,
			DONE_PARSING_PASSWD,
			DONE_WRITING_PASSWD
		};
		ClientList();
		~ClientList();
		int addClient(std::string userName, std::string passwdHash);
		int removeClient(std::string userName);
		void setFileName(std::string fileName);
		int parseClientPasswd(std::string userName, std::string* clientPasswdHash);
		int writeClientPasswd(std::string userName, std::string clientPasswdHash);
		bool clearDatabase();
	private:
		std::string m_data_file_name;
};


#endif /* CLIENTLIST_H_ */
