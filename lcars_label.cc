#include "lcars_label.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../lcars_component.hh"

LCARS::Label::Label(int x, int y, int w, int h, std::string font_path, std::string text) : Component({x,y,w,h}) {
	m_bg_color		= {  0,   0,   0, 255};

	m_draw_bg	= false;
	m_padding	= 0;
	m_vpadding	= 0;

	m_content_text = new Text(text, font_path, h);
	SetHeight(h);
}

LCARS::Label::~Label() {
	delete m_content_text;
}

void LCARS::Label::SetFontColor(SDL_Color c) {
	m_content_text->SetColor(c);
	SetNeedsRepaint(true);
}

SDL_Color LCARS::Label::GetFontColor() {
	return {0, 0, 0, 0};
}

void LCARS::Label::SetBGColor(SDL_Color c) {
	m_bg_color = c;
	SetNeedsRepaint(true);
}

SDL_Color LCARS::Label::GetBGColor() {
	return m_bg_color;
}

void LCARS::Label::SetFont(std::string font_path) {
	m_content_text->SetFont(font_path);
	SetNeedsRepaint(true);
}

void LCARS::Label::SetHeight(int h) {
	Component::SetHeight(h);
	m_content_text->SetFontSize(h);
	Component::SetWidth(m_content_text->GetBounds().w);
}

bool LCARS::Label::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::Label::Paint(PaintContext * paintctx) {

	if(m_draw_bg) {
		paintctx->SetColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
		SDL_Rect r = {0, 0, m_bounds.w, m_bounds.h};
		paintctx->FillRect(&r);
	}

	paintctx->DrawText(m_padding, m_vpadding, m_content_text);
}

void LCARS::Label::HandleSDLEvent(SDL_Event * ev) {

}

void LCARS::Label::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS::Label::HandleCMPEvent(CMP_EVT_TYPE type) {

}
