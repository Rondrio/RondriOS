#include "lcars_screen.hh"

#include <SDL2/SDL.h>
#include <X11/Xlib.h>

#include "lcars_interface.hh"

LCARS_Screen::LCARS_Screen(Display * dpy, int x, int y, int width, int height) {
	m_dpy		= dpy;
	m_width		= width;
	m_height	= height;

	m_sdl_window	= SDL_CreateWindow("LCARS SCREEN", x, y, width, height, SDL_WINDOW_SHOWN/*|SDL_WINDOW_FULLSCREEN*/);
	m_sdl_renderer	= SDL_CreateRenderer(m_sdl_window, -1, SDL_RENDERER_ACCELERATED);

	m_interface = nullptr;
}

LCARS_Screen::~LCARS_Screen() {
	SDL_DestroyRenderer	(m_sdl_renderer);
	SDL_DestroyWindow	(m_sdl_window);
}

void LCARS_Screen::Draw() {

	//SDL_RenderClear(m_sdl_renderer);
	/* --------------------------------------- */


	if(m_interface)
		m_interface->Draw(m_sdl_renderer);

	/*SDL_SetRenderDrawColor(m_sdl_renderer, 100, 100, 200, 255);
	SDL_Rect r = {0, 0, 400, 400};

	SDL_RenderFillRect(m_sdl_renderer, &r);*/

	SDL_SetRenderDrawColor(m_sdl_renderer, 0, 0, 0, 255);

	/* --------------------------------------- */
	//SDL_RenderPresent(m_sdl_renderer);
}

void LCARS_Screen::Remap() {
	m_interface->Remap();
}

void LCARS_Screen::SetInterface(LCARS_Interface * interface) {
	m_interface = interface;
	m_interface->AttachToScreen(this);
}

LCARS_Interface * LCARS_Screen::GetInterface() {
	return m_interface;
}

Display * LCARS_Screen::GetDisplay() {
	return m_dpy;
}

void LCARS_Screen::DispatchSDLEvents(SDL_Event * ev) {
	//TODO: Read TODO at lcars_screen.hh:15
	m_interface->DispatchSDLEvents(ev);
}
