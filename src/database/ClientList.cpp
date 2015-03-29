#include "ClientList.h"

ClientList::ClientList(){

}
ClientList::~ClientList(){

}
bool ClientList::addClient(uint32_t address, uint16_t port, std::string userName, std::string passwdHash, int peerId){

}
bool ClientList::removeClient(std::string userName){

}
bool ClientList::initDatabase(std::string fileName){
	this->m_data_file_name = fileName;
	this->m_ofstream.open(fileName.c_str());
	if(!this->m_ofstream){
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
	this->m_ofstream.close();
	return true;
}


