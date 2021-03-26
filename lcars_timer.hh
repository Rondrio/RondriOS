#ifndef _LCARS_TIMER_HH_
#define _LCARS_TIMER_HH_

#include <cstdint>

typedef void (*timer_action)(void);

struct lcarstimer_t {
	uint64_t		endtime;
	uint64_t		runtime;
	int				repeats;
	timer_action	action_func;
};

#endif