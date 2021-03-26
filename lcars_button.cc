#include "lcars_button.hh"

LCARS_Button::LCARS_Button(SDL_Rect bounds, TTF_Font * font, std::string text) : LCARS_Component(bounds) {
		m_pressed	= false;
		m_font		= font;
		m_text		= text;

		m_colors = (SDL_Color *) malloc(sizeof(SDL_Color) * 8);
}

LCARS_Button::~LCARS_Button() {
	free(m_colors);
}

void LCARS_Button::HandleSDLEvent(SDL_Event * ev) {
	switch(ev->type) {
		case SDL_MOUSEBUTTONDOWN:
			OnMouseDown(&(ev->button));
			break;
		case SDL_MOUSEBUTTONUP:
			OnMouseUp(&(ev->button));
			break;
		default:
			OnUnhandledSDLEvent(ev);
	}
}

void LCARS_Button::HandleCMPEvent(CMP_EVT_TYPE type) {
	switch(type) {
		case PD_FOCUS:
			SetNeedsRepaint(true);
			break;
		case PD_BLUR:
			SetNeedsRepaint(true);
			break;
		case KB_FOCUS:
			SetNeedsRepaint(true);
			break;
		case KB_BLUR:
			SetNeedsRepaint(true);
			break;
	}
}

void LCARS_Button::OnMouseDown(SDL_MouseButtonEvent * ev) {
	m_pressed = true;
	SetNeedsRepaint(true);
}

void LCARS_Button::OnMouseUp(SDL_MouseButtonEvent * ev) {
	m_pressed = false;
	SetNeedsRepaint(true);
}

void LCARS_Button::OnMouseMove(SDL_MouseMotionEvent * ev) {
	
}

void LCARS_Button::SetFont(TTF_Font * font) {
	m_font = font;
}

TTF_Font * LCARS_Button::GetFont() {
	return m_font;
}

void LCARS_Button::SetText(std::string text) {
	m_text = text;
}

std::string * LCARS_Button::GetText() {
	return &m_text;
}

void LCARS_Button::SetColor(BTN_COLOR_TYPE colortype, SDL_Color color) {
	m_colors[(int) colortype] = color;
}

SDL_Color * LCARS_Button::GetColor(BTN_COLOR_TYPE colortype) {
	return &(m_colors[(int) colortype]);
}
