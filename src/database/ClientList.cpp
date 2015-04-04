#include "ClientList.h"

ClientList::ClientList(){

}
ClientList::~ClientList(){

}
int ClientList::addClient(std::string userName, std::string passwdHash){
	std::ifstream m_file_in;
	std::ofstream m_file_out;
	if(this->m_data_file_name.length() == 0) return ClientList::ERROR_FILENAME;
	m_file_in.open(this->m_data_file_name.c_str());
	//m_file_out.open(this->m_data_file_name.c_str());
	//if(!m_file_in.is_open()) return ClientList::ERROR_OPENING;
	//if(!m_file_out.is_open()) return ClientList::ERROR_OPENING;
	std::string buff_;
	while(getline(m_file_in,buff_)){
		m_file_out << buff_ << std::endl;
		size_t pos_ = buff_.find((userName+"::").c_str());
		if(!( pos_ == std::string::npos || pos_ > 0)){
			m_file_in.close();
			return ClientList::ERROR_ADDING;
		}
	}
	m_file_in.close();
	m_file_out.open(this->m_data_file_name.c_str(),std::ofstream::app);
	std::string new_line = userName + "::" + passwdHash;
	m_file_out << new_line.c_str() << std::endl;
	m_file_out.close();
	return ClientList::DONE_ADDING;
}
int ClientList::removeClient(std::string userName){
	std::ifstream m_file_in;
	std::string temp_file = "temp__" + this->m_data_file_name;
	std::ofstream m_file_out(temp_file.c_str());
	if(this->m_data_file_name.length() == 0) return ClientList::ERROR_FILENAME;
	m_file_in.open(this->m_data_file_name.c_str());
	if(!m_file_in.is_open()) return ClientList::ERROR_OPENING;
	std::string buff_;
	bool found = false;
	while(getline(m_file_in,buff_)){
		size_t pos_ = buff_.find((userName+"::").c_str());
		if( pos_ == std::string::npos || pos_ > 0){
			m_file_out << buff_.c_str() << std::endl;
		}else found = true;
	}
	m_file_in.close();
	m_file_out.close();
	if(found){
		system(("mv "+temp_file +" " + this->m_data_file_name).c_str());
		return ClientList::DONE_REMOVING;
	}else{
		system(("mv "+temp_file).c_str());
		return ClientList::ERROR_NOT_FOUND;
	}
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
	while(getline(m_file,buff_)){
		if(buff_.find((userName+"::").c_str()) == 0){
			m_file.close();
			*clientPasswdHash = buff_.substr(userName.length()+2);
			return ClientList::DONE_PARSING_PASSWD;
		}
	}
	m_file.close();
	return ClientList::ERROR_NOT_FOUND;
}
int ClientList::writeClientPasswd(std::string userName, std::string clientPasswdHash){
	std::ifstream m_file_in;
	std::string temp_file = "temp__" + this->m_data_file_name;
	std::ofstream m_file_out(temp_file.c_str());
	if(this->m_data_file_name.length() == 0) return ClientList::ERROR_FILENAME;
	m_file_in.open(this->m_data_file_name.c_str());
	if(!m_file_in.is_open()) return ClientList::ERROR_OPENING;
	std::string buff_;
	bool found = false;
	while(getline(m_file_in,buff_)){
		if(buff_.find((userName+"::").c_str()) == std::string::npos || buff_.find((userName+"::").c_str()) > 0){
			m_file_out << buff_.c_str() << std::endl;
		}else{
			found = true;
			std::string new_buff_ = userName + "::" + clientPasswdHash;
			m_file_out << new_buff_.c_str() << std::endl;
		}
	}
	m_file_in.close();
	m_file_out.close();
	if(found){
		system(("mv "+temp_file +" " + this->m_data_file_name).c_str());
		return ClientList::DONE_WRITING_PASSWD;
	}else{
		system(("rm "+temp_file).c_str());
		return ClientList::ERROR_NOT_FOUND;
	}
}
bool ClientList::clearDatabase(){

}


