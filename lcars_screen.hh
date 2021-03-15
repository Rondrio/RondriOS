#ifndef LCARS_SCREEN_HH_
#define LCARS_SCREEN_HH_

#include <SDL2/SDL.h>
#include <X11/Xlib.h>

#include "lcars_interface.hh"

/* Forward declaration of the LCARS_Interface */
class LCARS_Interface;

class LCARS_Screen {

	private:
		LCARS_Interface	*	m_interface; //TODO: Maybe add support for multiple Interfaces.
		Display			*	m_dpy;

		int					m_width;
		int					m_height;

		SDL_Window		*	m_sdl_window;
		SDL_Renderer	*	m_sdl_renderer;

	public:
			LCARS_Screen	(Display * dpy, int x, int y, int width, int height);
		~	LCARS_Screen	();


		/**
		 * The Screen will be drawn. Draws also the connected LCARS_Interface. If there is no LCARS_Interface
		 * connected, only the Screen itself will be drawn.
		 * */
		void Draw();

		/**
		 * Convenience Method. Redirects this to the current LCARS_Interface, which will then remap the Windows.
		 * This Method will do nothing, if there is currently no LCARS_Interface connected to this Screen. To connect
		 * an LCARS_Interface to the Screen, use LCARS_Screen::SetInterface(LCARS_Interface *).
		 * */
		void Remap();

		void				SetInterface(LCARS_Interface * interface);
		LCARS_Interface * 	GetInterface();

		Display * GetDisplay();

		void DispatchSDLEvents(SDL_Event * ev);
};



#endif /* LCARS_SCREEN_HH_ */
