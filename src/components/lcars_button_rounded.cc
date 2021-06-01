#include "lcars_button_rounded.hh"

#include <iostream>
#include <string>
#include <cstdio>

#include "../lcars_button.hh"
#include "../lcars_colors.hh"

static bool is_in_circle(int x, int y, int cx, int cy, int r) {
	int diffx = cx - x;
	int diffy = cy - y;

	return diffx*diffx + diffy*diffy <= r*r;
}

LCARS::ButtonRounded::ButtonRounded(int x, int y, int w, int h, _font * font, std::string text) : Button(x, y, w, h, font, text) {
	m_circle_radius	= m_bounds.h/2;
	
	m_round_left	= true;
	m_round_right	= true;
	
	m_off_left	= 0;
	m_off_right	= 0;
	
	m_original_bounds = {x, y, w, h};

	SetColor(BTN_COLOR_TYPE::COLOR_IDLE,	COL_AMBER	);
	SetColor(BTN_COLOR_TYPE::COLOR_HOVER,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_PRESS,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_ACTIVE,	COL_SKYBLUE	);

	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE,	{0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_HOVER,	{0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_PRESS,	{0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_ACTIVE,	{0, 0, 0, 255});
}

LCARS::ButtonRounded::~ButtonRounded() {

}


Rect LCARS::ButtonRounded::CalcInnerRect() {
	return {(uint16_t) (m_bounds.h/2*m_round_left), 0, (uint16_t) (m_bounds.w-m_bounds.h/2*m_round_left-m_bounds.h/2*m_round_right), m_bounds.h};
}


bool LCARS::ButtonRounded::IsInInnerRect(int x, int y, Rect abs) {

	Rect inner_rect = CalcInnerRect();

	return (x >= inner_rect.x + abs.x && x <= inner_rect.w + abs.x) &&
			(y >= inner_rect.y + abs.y && y <= inner_rect.h + abs.y);
}

bool LCARS::ButtonRounded::IsInCircles(int x, int y, Rect abs) {
	bool cl = is_in_circle(x, y, m_circle_radius + abs.x, m_circle_radius + abs.y, m_circle_radius);
	bool cr = is_in_circle(x, y, m_circle_radius + abs.x + m_bounds.w - m_bounds.h, m_circle_radius + abs.y, m_circle_radius);

	return (cl && m_round_left) || (cr && m_round_right);
}

bool LCARS::ButtonRounded::PointInHitbox(int x, int y) {
	Rect abs = GetAbsoluteBounds();
	return IsInInnerRect(x, y, abs) || IsInCircles(x, y, abs);
}


void LCARS::ButtonRounded::Paint(PaintContext * paintctx) {

	_color c;

	if(m_has_pd_focus)
		c = GetColor(BTN_COLOR_TYPE::COLOR_HOVER);
	else if(m_has_kb_focus)
		c = GetColor(BTN_COLOR_TYPE::COLOR_ACTIVE);
	else if(m_pressed)
		c = GetColor(BTN_COLOR_TYPE::COLOR_PRESS);
	else
		c = GetColor(BTN_COLOR_TYPE::COLOR_IDLE);

	paintctx->SetColor(c.r, c.g, c.b, c.a);
	
	int rx = (int)(m_circle_radius*c_ellipse_factor);

	if(m_round_left)
		paintctx->FillEllipse(rx, m_circle_radius, rx, m_circle_radius);

	if(m_round_right)
		paintctx->FillEllipse(m_bounds.w - rx, m_circle_radius, rx, m_circle_radius);
	
	int rect_x = m_round_left*(rx + m_off_left);
	int rect_w = m_bounds.w - m_round_right*(rx*2);
	
	// Blacken
	paintctx->SetColor(0, 0, 0, 255);
	paintctx->FillRect(rx + rx/2, 0, m_bounds.w - 2*(rx + rx/2), m_bounds.h);
	
	paintctx->SetColor(c.r, c.g, c.b, c.a);
	
	// Extensions to the Rounded Edges
	if(m_round_left) {
		paintctx->FillRect(rx, 0, rx/2, m_bounds.h);
	}
	
	if(m_round_right) {
		paintctx->FillRect(m_bounds.w - rx - rx/2, 0, rx/2, m_bounds.h);
	}
	
	// Inner Rect
	paintctx->FillRect(m_round_left*(rx + rx/2 + m_off_left), 0, m_bounds.w - m_round_right*(rx + rx/2 + m_off_right) - m_round_left*(rx + rx/2 + m_off_right), m_bounds.h);

	/* ----- TEXT ------ */

	if(m_has_pd_focus)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_HOVER);
	else if(m_has_kb_focus)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_ACTIVE);
	else if(m_pressed)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_PRESS);
	else
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE);

	int round_offset = (m_bounds.h/2+10)*m_round_right + 10*!m_round_right;
	int extra_offset = m_round_right*m_off_right;

	paintctx->SetColor(c.r, c.g, c.b, c.a);
	paintctx->DrawText(m_bounds.w - m_text->w - extra_offset - round_offset, m_bounds.h - m_text->h - m_bounds.h/6, m_text);
}


void LCARS::ButtonRounded::HandleSDLEvent(XEvent * ev) {
	Button::HandleSDLEvent(ev);
}


void LCARS::ButtonRounded::OnUnhandledSDLEvent(XEvent * ev) {

}

void LCARS::ButtonRounded::SetRoundLeft(bool b) {
	m_round_left = b;
}

void LCARS::ButtonRounded::SetRoundRight(bool b) {
	m_round_right = b;
}

void LCARS::ButtonRounded::SetOffset(int left, int right) {
	m_off_left	= left;
	m_off_right	= right;
	
	m_bounds.x = m_original_bounds.x - left;
	m_bounds.w = m_original_bounds.w + left + right;
	
	SetNeedsRepaint(true);
}
