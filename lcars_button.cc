#include "lcars_button.hh"

void LCARS_Button::HandleSDLEvent(SDL_Event * ev) {
	switch(ev->type) {
		case SDL_MOUSEBUTTONDOWN:
			OnMouseDown(&(ev->button));
			break;
		case SDL_MOUSEBUTTONUP:
			OnMouseUp(&(ev->button));
			break;
		case SDL_MOUSEMOTION:
			OnMouseMove(&(ev->motion));
			break;
		default:
			OnUnhandledEvent(ev);
	}
}

void LCARS_Button::OnMouseDown(SDL_MouseButtonEvent * ev) {
	m_pressed = true;
}

void LCARS_Button::OnMouseUp(SDL_MouseButtonEvent * ev) {
	m_pressed = false;
}

void LCARS_Button::OnMouseMove(SDL_MouseMotionEvent * ev) {

}
