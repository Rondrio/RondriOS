#include "lcars_label.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "../lcars_component.hh"

LCARS_Label::LCARS_Label(int x, int y, int w, int h, std::string font_path, std::string text) : LCARS_Component({x,y,w,h}) {
	m_font_color	= {255, 255, 255, 255};
	m_bg_color		= {  0,   0,   0, 255};

	m_draw_bg	= false;
	m_padding	= 0;
	m_vpadding	= 0;

	m_font_path = font_path;
	m_font		= TTF_OpenFont(font_path.c_str(), h);
	m_text		= nullptr;
	m_text_string = text;

	SetHeight(h);
}

LCARS_Label::~LCARS_Label() {
	if(m_text)
		PaintContext::DestroyText(m_text);

	if(m_font)
		TTF_CloseFont(m_font);
}

void LCARS_Label::SetFontColor(SDL_Color c) {
	m_font_color = c;
	SetNeedsRepaint(true);
}

SDL_Color LCARS_Label::GetFontColor() {
	return m_font_color;
}

void LCARS_Label::SetBGColor(SDL_Color c) {
	m_bg_color = c;
	SetNeedsRepaint(true);
}

SDL_Color LCARS_Label::GetBGColor() {
	return m_bg_color;
}

void LCARS_Label::SetFont(TTF_Font * font) {
	m_font = font;
	SetNeedsRepaint(true);
}

TTF_Font * LCARS_Label::GetFont() {
	return m_font;
}

void LCARS_Label::SetHeight(int h) {
	LCARS_Component::SetHeight(h);

	if(m_font)
		TTF_CloseFont(m_font);

	m_font = TTF_OpenFont(m_font_path.c_str(), h);

	int width, height;
	TTF_SizeText(m_font, m_text_string.c_str(), &width, &height);

	if(m_text) {
		m_text->bounds.h = h;
		m_text->bounds.w = width;
	}

	LCARS_Component::SetWidth(width);
}

bool LCARS_Label::PointInHitbox(int x, int y) {
	//TODO: Make this to a Field inside of the Component and recalculate it only when needed.
	SDL_Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS_Label::Paint(PaintContext * paintctx) {

	if(m_draw_bg) {
		paintctx->SetColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
		SDL_Rect r = {0, 0, m_bounds.w, m_bounds.h};
		paintctx->FillRect(&r);
	}

	if(!m_text) {

		paintctx->SetColor(m_font_color.r, m_font_color.g, m_font_color.b, m_font_color.a);
		paintctx->SetFont(m_font);

		m_text = paintctx->PrepareBlendedText(m_padding, m_vpadding, (char *) m_text_string.c_str());
	}

	if(m_text) {

		SDL_Rect src = {0, 0, m_text->bounds.w, m_text->bounds.h};
		SDL_Rect dst = {m_padding, m_vpadding, m_text->bounds.w, m_text->bounds.h};

		paintctx->DrawText(m_text, &src, &dst);
	}
}

void LCARS_Label::HandleSDLEvent(SDL_Event * ev) {

}

void LCARS_Label::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS_Label::HandleCMPEvent(CMP_EVT_TYPE type) {

}
