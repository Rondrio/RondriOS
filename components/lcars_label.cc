#include "lcars_label.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../lcars_component.hh"

LCARS_Label::LCARS_Label(int x, int y, int w, int h, std::string font_path, std::string text) : LCARS_Component({x,y,w,h}) {
	m_bg_color		= {  0,   0,   0, 255};

	m_draw_bg	= false;
	m_padding	= 0;
	m_vpadding	= 0;

	m_content_text = new LCARS_Text(text, font_path, h);
	SetHeight(h);
}

LCARS_Label::~LCARS_Label() {
	delete m_content_text;
}

void LCARS_Label::SetFontColor(SDL_Color c) {
	m_content_text->SetColor(c);
	SetNeedsRepaint(true);
}

SDL_Color LCARS_Label::GetFontColor() {
	return {0, 0, 0, 0};
}

void LCARS_Label::SetBGColor(SDL_Color c) {
	m_bg_color = c;
	SetNeedsRepaint(true);
}

SDL_Color LCARS_Label::GetBGColor() {
	return m_bg_color;
}

void LCARS_Label::SetFont(std::string font_path) {
	m_content_text->SetFont(font_path);
	SetNeedsRepaint(true);
}

void LCARS_Label::SetHeight(int h) {
	LCARS_Component::SetHeight(h);
	m_content_text->SetFontSize(h);
	LCARS_Component::SetWidth(m_content_text->GetBounds().w);
}

bool LCARS_Label::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS_Label::Paint(PaintContext * paintctx) {

	if(m_draw_bg) {
		paintctx->SetColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
		SDL_Rect r = {0, 0, m_bounds.w, m_bounds.h};
		paintctx->FillRect(&r);
	}

	paintctx->PrepareText	(m_content_text);
	paintctx->DrawText		(m_padding, m_vpadding, m_content_text);
}

void LCARS_Label::HandleSDLEvent(SDL_Event * ev) {

}

void LCARS_Label::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS_Label::HandleCMPEvent(CMP_EVT_TYPE type) {

}
