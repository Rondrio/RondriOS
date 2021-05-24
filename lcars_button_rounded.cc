
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>
#include <string>
#include <cstdio>

#include "../lcars_button.hh"
#include "lcars_button_rounded.hh"
#include "../lcars_colors.hh"

static bool is_in_circle(int x, int y, int cx, int cy, int r) {
	int diffx = cx - x;
	int diffy = cy - y;

	return diffx*diffx + diffy*diffy <= r*r;
}

LCARS::ButtonRounded::ButtonRounded(int x, int y, int w, int h, Font * font, std::string text) : Button({x,y,w,h}, font, text) {
	Init();
}

LCARS::ButtonRounded::ButtonRounded(int x, int y, int w, int h, std::string fontpath, std::string text) : LCARS::Button({x,y,w,h}, fontpath, text) {
	Init();
}

void LCARS::ButtonRounded::Init() {
	m_circle_radius	= m_bounds.h/2;
	
	m_round_left	= true;
	m_round_right	= true;

	SetColor(BTN_COLOR_TYPE::COLOR_IDLE,	COL_AMBER	);
	SetColor(BTN_COLOR_TYPE::COLOR_HOVER,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_PRESS,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_ACTIVE,	COL_SKYBLUE	);

	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE,	{255, 255, 255, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_HOVER,	{255, 255, 255, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_PRESS,	{255, 255, 255, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_ACTIVE,	{255, 255, 255, 255});
}

LCARS::ButtonRounded::~ButtonRounded() {

}


SDL_Rect LCARS::ButtonRounded::CalcInnerRect() {
	return {m_bounds.h/2*m_round_left, 0, m_bounds.w-m_bounds.h/2*m_round_left-m_bounds.h/2*m_round_right, m_bounds.h};
}


bool LCARS::ButtonRounded::IsInInnerRect(int x, int y, SDL_Rect abs) {

	SDL_Rect inner_rect = CalcInnerRect();

	return (x >= inner_rect.x + abs.x && x <= inner_rect.w + abs.x) &&
			(y >= inner_rect.y + abs.y && y <= inner_rect.h + abs.y);
}

bool LCARS::ButtonRounded::IsInCircles(int x, int y, SDL_Rect abs) {
	bool cl = is_in_circle(x, y, m_circle_radius + abs.x, m_circle_radius + abs.y, m_circle_radius);
	bool cr = is_in_circle(x, y, m_circle_radius + abs.x + m_bounds.w - m_bounds.h, m_circle_radius + abs.y, m_circle_radius);

	return (cl && m_round_left) || (cr && m_round_right);
}

bool LCARS::ButtonRounded::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return IsInInnerRect(x, y, abs) || IsInCircles(x, y, abs);
}


void LCARS::ButtonRounded::Paint(PaintContext * paintctx) {

	SDL_Color c;

	if(m_has_pd_focus)
		c = GetColor(BTN_COLOR_TYPE::COLOR_HOVER);
	else if(m_has_kb_focus)
		c = GetColor(BTN_COLOR_TYPE::COLOR_ACTIVE);
	else if(m_pressed)
		c = GetColor(BTN_COLOR_TYPE::COLOR_PRESS);
	else
		c = GetColor(BTN_COLOR_TYPE::COLOR_IDLE);

	paintctx->SetColor(c.r, c.g, c.b, c.a);

	SDL_Rect inner_rect = CalcInnerRect();
	paintctx->FillRect(&inner_rect);

	int rx = (int)(m_circle_radius*c_ellipse_factor);

	if(m_round_left)
		paintctx->FillEllipse(m_circle_radius, m_circle_radius, rx, m_circle_radius);

	if(m_round_right)
		paintctx->FillEllipse(m_circle_radius + m_bounds.w - m_bounds.h, m_circle_radius, rx, m_circle_radius);

	/* ----- TEXT ------ */

	if(m_has_pd_focus)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_HOVER);
	else if(m_has_kb_focus)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_ACTIVE);
	else if(m_pressed)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_PRESS);
	else
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE);

	m_text->SetColor(c);
	paintctx->DrawText(m_bounds.w - m_text->GetBounds().w - 10*!m_round_right, m_bounds.h - m_text->GetBounds().h, m_text);
}


void LCARS::ButtonRounded::HandleSDLEvent(SDL_Event * ev) {
	Button::HandleSDLEvent(ev);
}


void LCARS::ButtonRounded::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS::ButtonRounded::SetRoundLeft(bool b) {
	m_round_left = b;
}

void LCARS::ButtonRounded::SetRoundRight(bool b) {
	m_round_right = b;
}
