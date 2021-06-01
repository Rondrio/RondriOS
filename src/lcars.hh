/**
 * The LCARS module itself is a singleton as it is not required or admired to have more than one LCARS instance
 * per running Program.
 * */
#ifndef LCARS_HH_
#define LCARS_HH_

//#include <X11/Xlib.h>

#include "simple_list.hh"
#include "lcars_screen.hh"
#include "lcars_keylistener.hh"

namespace LCARS {

	#include <X11/Xlib.h>

	class LCARS {

		private:

			static	XErrorHandler		s_x_error_handler;
					Display 		*	m_dpy;
					Screen			*	m_screen;
					Monitor 		*	m_active_lcars_screen;
					Window				m_root;

					Window	m_focused_window;

					smp::list<Window>				m_windows;
					smp::list<KeyListener *> *		m_key_listeners;

			volatile bool m_running;

			static int ErrorHandler(Display * dpy, XErrorEvent * ev);

		public:
						LCARS()				= delete;
						LCARS(const LCARS&)	= delete;
						LCARS(Display * dpy);
			virtual ~	LCARS();

			void AddKeyListener(KeyListener * kl);

			/**
			 * Handles the Events from the X Server.
			* @param ev Pointer to the XEvent structure.
			* @return 0 if an error occurred.
			* */
			int HandleEventX	(XEvent * ev);

			int	GetWindowCount	();

			/**
			 * Frames the given Window adds an entry of the resulting Pair to the
			* internal Registry.
			* 
			* @param w The Window to reparent into a Frame.
			*/
			void FrameWindow	(Window w);

			/**
			 * Removes the Frame around a Window.
			* @param w The Window to unframe.
			*/
			void UnframeWindow	(Window w);

			Window GetFocusedWindow();

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
			Monitor * GetScreen();

			int Checkmod(XKeyEvent ke, KeyListener listener);

			LCARS& operator=(const LCARS&) = delete;
	};

}

#endif /* LCARS_HH_ */
