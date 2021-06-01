#include "lcars_label.hh"

#include <string>

#include "../lcars_component.hh"
#include "../lcars_font.hh"

LCARS::Label::Label(int x, int y, _font * font, std::string text) : Component((Rect) {x,y,0,0}) {
	m_bg_color = {  0,   0,   0, 255};
	m_fg_color = {255, 255, 255, 255};

	m_draw_bg = false;
	
	m_text_str	= text;
	m_text.font	= font;
	m_text.str	= (char *) m_text_str.c_str();
	
	LCARS::TextSize(&m_text);
	m_bounds.h = m_text.h;
	m_bounds.w = m_text.w;
	
	m_original_bounds = {x, y, m_bounds.w, m_bounds.h};
	SetPadding(0, 0, 0, 0);
}

LCARS::Label::~Label() {
	
}

void LCARS::Label::SetFontColor(_color c) {
	m_fg_color = c;
	SetNeedsRepaint(true);
}

_color LCARS::Label::GetFontColor() {
	return m_fg_color;
}

void LCARS::Label::SetBGColor(_color c) {
	m_bg_color = c;
	SetNeedsRepaint(true);
}

_color LCARS::Label::GetBGColor() {
	return m_bg_color;
}

void LCARS::Label::SetFont(_font * font) {
	m_text.font = font;
	SetNeedsRepaint(true);
}

void LCARS::Label::SetPadding(int top, int right, int bottom, int left) {
	m_padding.x = left;
	m_padding.y = top;
	m_padding.w = right;
	m_padding.h = bottom;
	
	SetHeight(m_original_bounds.h + m_padding.y + m_padding.h);
	SetWidth(m_original_bounds.w + m_padding.x + m_padding.w);
}

bool LCARS::Label::PointInHitbox(int x, int y) {
	Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::Label::Paint(PaintContext * paintctx) {

	if(m_draw_bg) {
		paintctx->SetColor(255, m_bg_color.g, m_bg_color.b, 255);
		paintctx->FillRect(0, 0, m_bounds.w, m_bounds.h);
	}

	paintctx->SetColor(m_fg_color.r, m_fg_color.g ,m_fg_color.b, m_fg_color.a);
	paintctx->DrawText(m_padding.x, m_padding.y, &m_text);
}

void LCARS::Label::HandleSDLEvent(XEvent * ev) {

}

void LCARS::Label::OnUnhandledSDLEvent(XEvent * ev) {

}

void LCARS::Label::HandleCMPEvent(CMP_EVT_TYPE type) {

}
