#include "ClientList.h"

ClientList::ClientList(){

}
ClientList::~ClientList(){

}
int ClientList::addClient(std::string userName, std::string passwdHash){
	std::fstream m_file;
	if(this->m_data_file_name.length() == 0) return ClientList::ERROR_FILENAME;
	m_file.open(this->m_data_file_name.c_str(),std::fstream::out);
	if(!m_file.is_open()) return ClientList::ERROR_OPENING;
	std::string buff_;
	while(!getline(m_file,buff_)){
		if(buff_.find((userName+"::").c_str()) >= 0){
			m_file.close();
			return ClientList::ERROR_ADDING;
		}
	}
	std::string new_line = userName + "::" + passwdHash;
	m_file << new_line.c_str() << std::endl;
	m_file.close();
	return ClientList::DONE_ADDING;
}
int ClientList::removeClient(std::string userName){
	std::fstream m_file_in,m_file_out;
	if(this->m_data_file_name.length() == 0) return ClientList::ERROR_FILENAME;
	m_file_in.open(this->m_data_file_name.c_str());
	m_file_out.open(this->m_data_file_name.c_str());
	if(!m_file_in.is_open()) return ClientList::ERROR_OPENING;
	if(!m_file_out.is_open()) return ClientList::ERROR_OPENING;
	std::string buff_;
	bool found = false;
	while(!getline(m_file_in,buff_)){
		if(buff_.find((userName+"::").c_str()) < 0){
			m_file_out << buff_.c_str() << std::endl;
		}else found = true;
	}
	m_file_out.close();
	if(!found) return ClientList::ERROR_NOT_FOUND;
	else return ClientList::DONE_REMOVING;
}
void ClientList::setFileName(std::string fileName){
	this->m_data_file_name = fileName;
}
int ClientList::parseClientPasswd(std::string userName, std::string* clientPasswdHash){
	std::fstream m_file;
	if(this->m_data_file_name.length() == 0) return ClientList::ERROR_FILENAME;
	m_file.open(this->m_data_file_name.c_str());
	if(!m_file.is_open()) return ClientList::ERROR_OPENING;
	std::string buff_;
	while(!getline(m_file,buff_)){
		if(buff_.find((userName+"::").c_str()) >= 0){
			*clientPasswdHash = buff_.substr(buff_.find((userName+"::").c_str()));
			m_file.close();
			return ClientList::DONE_PARSING_PASSWD;
		}
	}
	m_file.close();
	return ClientList::ERROR_NOT_FOUND;
}
int ClientList::writeClientPasswd(std::string userName, std::string clientPasswdHash){
	std::fstream m_file_in,m_file_out;
	if(this->m_data_file_name.length() == 0) return ClientList::ERROR_FILENAME;
	m_file_in.open(this->m_data_file_name.c_str());
	m_file_out.open(this->m_data_file_name.c_str());
	if(!m_file_in.is_open()) return ClientList::ERROR_OPENING;
	if(!m_file_out.is_open()) return ClientList::ERROR_OPENING;
	std::string buff_;
	bool found = false;
	while(!getline(m_file_in,buff_)){
		if(buff_.find((userName+"::").c_str()) < 0){
			m_file_out << buff_.c_str() << std::endl;
		}else{
			found = true;
			std::string new_buff_ = userName + "::" + clientPasswdHash;
			m_file_out << new_buff_.c_str() << std::endl;
		}
	}
	m_file_out.close();
	if(!found) return ClientList::ERROR_NOT_FOUND;
	else return ClientList::DONE_WRITING_PASSWD;
}
bool ClientList::clearDatabase(){

}


