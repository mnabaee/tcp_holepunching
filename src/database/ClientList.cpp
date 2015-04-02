#include "ClientList.h"

ClientList::ClientList(){

}
ClientList::~ClientList(){

}
bool ClientList::addClient(std::string userName, std::string passwdHash){
	if(this->m_fstream){
		std::string new_line = userName + "::" + passwdHash;
		this->m_fstream << new_line.c_str() << std::endl;
		return true;
	}else return false;
}
bool ClientList::removeClient(std::string userName){
	std::fstream new_fstream;
	if(this->m_fstream){
		std::string line_;
		while(std::getline(this->m_fstream,line_)){
			int pos_separator = line_.find(userName+"::");
			if(pos_separator == 0){
				std::string passwd = line_.substr(userName.length()+2);
				if(passwd != ""){
					continue;
				}
			}
			new_fstream << line_ << std::endl;
		}

	}else return false;
}
bool ClientList::initDatabase(std::string fileName){
	this->m_data_file_name = fileName;
	this->m_fstream.open(fileName.c_str());
	if(!this->m_fstream){
		return false;
	}else{
		return true;
	}
}
bool ClientList::parseClientPasswd(std::string userName, std::string* clientPasswdHash){

}
int ClientList::writeClientPasswd(std::string userName, std::string clientPasswdHash){

}
bool ClientList::clearDatabase(){
	if(this->m_ofstream){
		this->m_ofstream.close();
		this->m_ofstream.open(this->m_data_file_name.c_str(),std::ofstream::out || std::ofstream::trunc);
		this->m_ofstream.close();
		this->m_ofstream.open(this->m_data_file_name.c_str());
		return true;
	}else return false;
}
bool ClientList::closeDatabase(){
	this->m_fstream.close();
	return true;
}


