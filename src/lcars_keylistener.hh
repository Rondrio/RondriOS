#ifndef LCARS_KEYLISTENER_HH_
#define LCARS_KEYLISTENER_HH_

#include <SDL2/SDL.h>

#include "lcars.hh"

namespace LCARS {

class LCARS;

	typedef struct KeyListener KeyListener;
	typedef struct Arg Arg;
	typedef void (*key_listener_func)(LCARS * lcars, Arg * arg);

	struct Arg {
		int				i;
		unsigned int	ui;
		double			d;
		void *			v;
	};

	struct KeyListener {
			KeySym				keysym;		// keysym of key
			unsigned int 		modmask;	// mod keys
			key_listener_func	func;		// what shall be executed
			Arg arg;						// parameters for listener function
	};
}

#endif /* LCARS_KEYLISTENER_HH_ */
