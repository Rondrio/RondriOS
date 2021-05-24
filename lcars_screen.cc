#include "lcars_screen.hh"

#include <SDL2/SDL.h>
#include <X11/Xlib.h>

#include "time.h"
#include "lcars_timer.hh"
#include "lcars_interface.hh"

LCARS::Monitor::Monitor(Display * dpy, int x, int y, int width, int height) {
	m_dpy		= dpy;
	m_width		= width;
	m_height	= height;

	m_sdl_window		= SDL_CreateWindow("LCARS SCREEN", x, y, width, height, SDL_WINDOW_SHOWN/*|SDL_WINDOW_FULLSCREEN*/);
	m_sdl_renderer		= SDL_CreateRenderer(m_sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

	m_interface = nullptr;
}

LCARS::Monitor::~Monitor() {
	SDL_DestroyRenderer	(m_sdl_renderer);
	SDL_DestroyWindow	(m_sdl_window);
}

void LCARS::Monitor::Draw() {

	//SDL_RenderClear(m_sdl_renderer);
	/* --------------------------------------- */


	if(m_interface) {
		m_interface->Draw(m_sdl_renderer);
	}

	if(m_timers.Size() > 0) {

		time_t ti;
		time(&ti);

		for(int i = 0; i < m_timers.Size(); ++i) {
			lcarstimer_t * timer = m_timers[i];
			bool decreased = false;

			if(timer->repeats != 0 && (ti >= timer->endtime)) {
				timer->repeats--;
				timer->action_func();

				decreased = true;
			}

			if(timer->repeats == 0) {
				m_timers -= timer;
				free(timer);

			} else if(decreased)
				timer->endtime += timer->runtime;
		}
	}

	/*SDL_SetRenderDrawColor(m_sdl_renderer, 100, 100, 200, 255);
	SDL_Rect r = {0, 0, 400, 400};

	SDL_RenderFillRect(m_sdl_renderer, &r);*/

	/* --------------------------------------- */
	//SDL_RenderPresent(m_sdl_renderer);
}

void LCARS::Monitor::Remap() {
	m_interface->Remap();
}

void LCARS::Monitor::SetInterface(Interface * interface) {
	m_interface = interface;
	m_interface->AttachToScreen(this);
}

LCARS::Interface * LCARS::Monitor::GetInterface() {
	return m_interface;
}

Display * LCARS::Monitor::GetDisplay() {
	return m_dpy;
}

void LCARS::Monitor::AddTimer(uint64_t runtime, int repeats, timer_action action_func) {

	time_t ti;
	time(&ti);

	lcarstimer_t * timer = (lcarstimer_t *) malloc(sizeof(lcarstimer_t));
	timer->endtime		= runtime + ti;
	timer->runtime		= runtime;
	timer->repeats		= repeats;
	timer->action_func	= action_func;

	m_timers += timer;
}

void LCARS::Monitor::DispatchSDLEvents(SDL_Event * ev) {
	//TODO: Read TODO at lcars_screen.hh:15
	m_interface->DispatchSDLEvents(ev);
}