#include "lcars_error.hh"

#include <iostream>
#include <time.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


static int			logfd = -1;
static std::string	log_directory;
static std::string	log_file;

int LCARS::LOG_Init(std::string log_directory) {

	/*if(log_directory[log_directory.size() -1] != '/') {
		log_directory += '/';
	}

	time_t ti;
	time(&ti);

	char time_str[32];
	memset(time_str, '\0', 32);

	sprintf(time_str, "%d.txt", ti);

	log_file  = log_directory + "log_";
	log_file += time_str;

	logfd = open(log_file.c_str(), O_APPEND | O_CREAT);

	if(logfd == -1) {
		std::cerr << "ERROR at LOG_Init(): Could not open file descriptor of: " << log_file << std::endl;
		perror("logfd");
		return -1;
	}
*/
	return 0;
}

void LCARS::LOG_Quit() {
	if(logfd != -1) {
		close(logfd);
	}
}

void LCARS::LOG(std::string src, std::string reason) {
	if(logfd != -1) {
		dprintf(logfd, "%s: %s\n", src.c_str(), reason.c_str());
	}
	std::cerr << src << ": " << reason << std::endl;
}

void LCARS::LOGEX(std::string src, std::string reason, std::string extra) {
	if(logfd != -1) {
		dprintf(logfd, "%s: %s - %s\n", src.c_str(), reason.c_str(), extra.c_str());
	}
	std::cerr << src << ": " << reason << " - " << extra << std::endl;
}
