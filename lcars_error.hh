#ifndef LCARS_ERROR_HH_
#define LCARS_ERROR_HH_

#include <string>

namespace LCARS {

	extern int	LOG_Init	(std::string log_directory);
	extern void	LOG_Quit	();
	extern void	LOG			(std::string src, std::string reason);
	extern void	LOGEX		(std::string src, std::string reason, std::string extra);

}

#endif