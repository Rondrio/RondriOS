/**
 * The LCARS module itself is a singleton as it is not required or admired to have more than one LCARS instance
 * per running Program.
 * */
#ifndef LCARS_HH_
#define LCARS_HH_

#include <X11/Xlib.h>

#include "simple_list.hh"
#include "lcars_screen.hh"
#include "lcars_keylistener.hh"

struct SCREEN_INIT {
	unsigned int screen_width;
	unsigned int screen_height;
};

class LCARS {
	private:

		static	XErrorHandler		s_x_error_handler;
				Display 		*	m_dpy;
				Screen			*	m_screen;
				LCARS_Screen 	*	m_active_lcars_screen;
				Window				m_root;

				smp::list<KeyListener *> * m_key_listeners;

		volatile bool m_running;

		static int ErrorHandler(Display * dpy, XErrorEvent * ev);

	public:
			LCARS() = delete;
			LCARS(Display * dpy);
		~	LCARS();

		void AddKeyListener(KeyListener * kl);

		/**
		 * Handles the Events from the X Server.
		 * @param ev Pointer to the XEvent structure.
		 * @return 0 if an error occurred.
		 * */
		int HandleEventX	(XEvent * ev);

		/**
		 * Handles the Events from the SDL Window representing the LCARS' Main Frame.
		 * @param ev Pointer to the SDL_Event structure.
		 * @return 0 if an error occurred.
		 * */
		int HandleEventSDL	(SDL_Event * ev);

		/**
		 * Initializes the LCARS.
		 * */
		int Init	();

		/**
		 * Starts the Event-Loop of the LCARS. There is no going back after this as it will lock the Thread in
		 * an endless loop.
		 * */
		int Run		();

		/**
		 * Cleans up allocated Memory, closes Connections and stops the execution of the LCARS.
		 * @param exitcode The Exitcode with which the Program will stop.
		 * @return The Exitcode given as parameter.
		 * */
		int Exit	(int exitcode);

		void CloseFocusedWindow();

		Display * GetDisplay();

		int Checkmod(XKeyEvent ke, KeyListener listener);
};

#endif /* LCARS_HH_ */
