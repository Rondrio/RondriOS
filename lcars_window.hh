/*
 * lcars_window.hh
 *
 *  Created on: 13.03.2021
 *      Author: cediw
 */

#ifndef LCARS_WINDOW_HH_
#define LCARS_WINDOW_HH_

#include <SDL2/SDL.h>
#include <X11/Xlib.h>

#include "lcars_interface.hh"
#include "lcars_windowdecoration.hh"

class LCARS_Interface;
class LCARS_WindowDecoration;

class LCARS_Window {

	private:
		Window 						m_window;
		LCARS_Interface			*	m_interface;
		LCARS_WindowDecoration	* 	m_window_decoration;

		Display * m_dpy;

	public:
			LCARS_Window(LCARS_Interface * lcars_interface, Display * dpy, Window window);
		~	LCARS_Window();

		Window GetXWindow();
		LCARS_WindowDecoration * GetDecoration();

		void Move			(int x, int y);
		void MoveRelative	(int x, int y);

		void Raise();

		bool IsFocused();
};



#endif /* LCARS_WINDOW_HH_ */
