
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>

#include "../lcars_button.hh"
#include "lcars_button_rounded.hh"


LCARS_Button_Rounded::LCARS_Button_Rounded(int x, int y, int w, int h) : LCARS_Button({x,y,w,h}) {
	m_circle_radius = h/2;
}

bool LCARS_Button_Rounded::PointInHitbox(int x, int y) {
	return x >= m_bounds.x && x <= (m_bounds.x + m_bounds.w) && y >= m_bounds.y && y <= (m_bounds.h + m_bounds.y);
}

void LCARS_Button_Rounded::Paint(SDL_Renderer * renderer) {
	if(m_pressed)
		SDL_SetRenderDrawColor(renderer, 99, 250 , 76, 255);
	else if(m_has_kb_focus)
		SDL_SetRenderDrawColor(renderer, 14, 77, 200, 255);
	else if(m_has_pd_focus)
		SDL_SetRenderDrawColor(renderer, 50, 200, 200, 255);
	else
		SDL_SetRenderDrawColor(renderer, 100, 150, 100, 255);

	SDL_RenderFillRect(renderer, &m_bounds);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawRect(renderer, &m_bounds);

	ellipseRGBA(renderer, 300, 300, 100, 200, 100, 100, 100, 255);
}

void LCARS_Button_Rounded::HandleSDLEvent(SDL_Event * ev) {
	LCARS_Button::HandleSDLEvent(ev);
}

void LCARS_Button_Rounded::OnUnhandledEvent(SDL_Event * ev) {

}
