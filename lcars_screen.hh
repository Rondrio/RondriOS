#ifndef LCARS_SCREEN_HH_
#define LCARS_SCREEN_HH_

#include <SDL2/SDL.h>
#include <X11/Xlib.h>

#include "lcars_interface.hh"
#include "simple_list.hh"
#include "lcars_timer.hh"

namespace LCARS {

	/* Forward declaration of the LCARS_Interface */
	class Interface;

	class Monitor {

		private:
			Interface	*	m_interface; //TODO: Maybe add support for multiple Interfaces.
			Display			*	m_dpy;

			int					m_width;
			int					m_height;

			SDL_Window		*	m_sdl_window;
			SDL_Renderer	*	m_sdl_renderer;

			smp::list<lcarstimer_t *>	m_timers;

		public:
						Monitor	()						= delete;
						Monitor	(const Monitor&)	= delete;
						Monitor	(Display * dpy, int x, int y, int width, int height);
			virtual ~	Monitor	();


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

			void				SetInterface(Interface * interface);
			Interface * 	GetInterface();

			Display * GetDisplay();

			/**
			 * Adds a Timer. The Timer will wait for _runtime_ Seconds and
			* then call the timer_action Function. If the repeat Flag is set to 1,
			* the Timer will run one time. Is the repeat Flag set higher then 1,
			* the Timer will run as many times as the Flag states.
			* To let the Timer run indefinetly, set the Flag to -1.
			*/
			void AddTimer(uint64_t runtime, int repeats, timer_action action_func);

			void DispatchSDLEvents(SDL_Event * ev);

			Monitor& operator=(const Monitor&) = delete;
	};

}

#endif /* LCARS_SCREEN_HH_ */
