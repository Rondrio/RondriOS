#include "lcars_button.hh"

#include <cstring>
#include <cstdio>

LCARS::Button::Button(SDL_Rect bounds) : Component(bounds) {
	m_pressed	= false;
	m_colors	= (SDL_Color *) malloc(sizeof(SDL_Color) * 8);
	memset(m_colors, 0, sizeof(SDL_Color)*8);
}

LCARS::Button::Button(SDL_Rect bounds, std::string fontpath, std::string text) : Button(bounds) {
	m_text = new Text(text, fontpath, bounds.h);
}

LCARS::Button::Button(SDL_Rect bounds, Font * font, std::string text) : Button(bounds) {
	m_text = new Text(text, font);
}

LCARS::Button::~Button() {
	delete m_text;
	free(m_colors);
}

void LCARS::Button::HandleSDLEvent(SDL_Event * ev) {
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

void LCARS::Button::HandleCMPEvent(CMP_EVT_TYPE type) {
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

void LCARS::Button::OnMouseDown(SDL_MouseButtonEvent * ev) {
	m_pressed = true;
	SetNeedsRepaint(true);
}

void LCARS::Button::OnMouseUp(SDL_MouseButtonEvent * ev) {
	m_pressed = false;

	for(int i = 0; i < m_action_listeners.Size(); ++i)
		m_action_listeners[i]();

	SetNeedsRepaint(true);
}

void LCARS::Button::OnMouseMove(SDL_MouseMotionEvent * ev) {
	
}

void LCARS::Button::AddActionListener(action_listener listener) {
	m_action_listeners += listener;
}

void LCARS::Button::RemActionListener(action_listener listener) {
	m_action_listeners -= listener;
}

void LCARS::Button::SetFont(Font * font) {
	m_text->SetFont(font);
}

void LCARS::Button::SetText(std::string text) {
	m_text->SetText(text);
}

LCARS::Text * LCARS::Button::GetText() {
	return m_text;
}

void LCARS::Button::SetColor(BTN_COLOR_TYPE colortype, SDL_Color color) {
	m_colors[(int) colortype] = color;
}

SDL_Color LCARS::Button::GetColor(BTN_COLOR_TYPE colortype) {
	return m_colors[(int) colortype];
}
