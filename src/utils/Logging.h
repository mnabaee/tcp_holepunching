#ifndef LOGGING_H_
#define LOGGING_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdarg.h>

class Logging{
private:
	static Logging* m_pInstance;
	Logging();
	bool info_enb;
	bool warning_enb;
	bool error_enb;
	bool fatal_enb;
public:
	enum LogLevel{
		INFO,
		WARNING,
		ERROR,
		FATAL
	};
	static Logging* getInstance();
	~Logging();
	void setLevel(int level, bool val);
	bool isLevelEnabled(int level);
	#define LOG_MESSAGE(prio, msg, ...) do {\
							const char* prio_str = "NULL"; \
							if(prio == Logging::INFO) prio_str = "INFO"; \
							if(prio == Logging::WARNING) prio_str = "WARNING"; \
							if(prio == Logging::ERROR) prio_str = "ERROR"; \
							if(prio == Logging::FATAL) prio_str = "FATAL"; \
							printf("%s:: %s (%d): "msg" \n", \
									prio_str, __FILE__, __LINE__, ##__VA_ARGS__);\
						} while (false)
	#define LogEvt(prio,msg,...) do{\
								if(Logging::getInstance()->isLevelEnabled(prio)) LOG_MESSAGE(prio,msg,...) \
								}while(false)


};
#endif /* LOGGING_H_ */
