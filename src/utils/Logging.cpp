#include "Logging.h"

Logging* Logging::m_pInstance = NULL;

Logging::Logging(){
	this->error_enb = false;
	this->fatal_enb = false;
	this->info_enb = false;
	this->warning_enb = false;
}
Logging::~Logging(){
}
Logging* Logging::getInstance(){
	if(m_pInstance == NULL){
		m_pInstance = new Logging();
		return m_pInstance;
	}else{
		return m_pInstance;
	}
}
void Logging::setLevel(int level, bool val){
	if(level == Logging::INFO)
		this->info_enb = val;
	else if(level == Logging::ERROR)
		this->error_enb = val;
	else if(level == Logging::WARNING)
		this->warning_enb = val;
	else if(level == Logging::FATAL)
		this->fatal_enb = val;
}
bool Logging::isLevelEnabled(int level){
	if(level == Logging::INFO)
		return this->info_enb;
	else if(level == Logging::WARNING)
		return this->warning_enb;
	else if(level == Logging::ERROR)
		return this->error_enb;
	else if(level == Logging::FATAL)
		return this->fatal_enb;
	return false;
}
