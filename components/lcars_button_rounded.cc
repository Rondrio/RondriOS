
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>

#include "../lcars_button.hh"
#include "lcars_button_rounded.hh"
#include "../lcars_colors.hh"

static bool is_in_circle(int x, int y, int cx, int cy, int r) {
	int diffx = cx - x;
	int diffy = cy - y;

	return diffx*diffx + diffy*diffy <= r*r;
}

LCARS_Button_Rounded::LCARS_Button_Rounded(int x, int y, int w, int h, TTF_Font * font, char * text) : LCARS_Button({x,y,w,h}, font, text) {
	m_circle_radius	= h/2;
	m_inner_rect	= {h/2, 0, w-h, h};
	m_button_text	= nullptr;

	m_round_left	= true;
	m_round_right	= true;

	SetColor(BTN_COLOR_TYPE::COLOR_IDLE,	COL_AMBER	);
	SetColor(BTN_COLOR_TYPE::COLOR_HOVER,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_PRESS,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_ACTIVE,	COL_SKYBLUE	);

	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE, {0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE, {0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE, {0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE, {0, 0, 0, 255});
	// TODO: WARN: Height may not be greater or equal to the width.
}

bool focus = false;

LCARS_Button_Rounded::~LCARS_Button_Rounded() {
	if(m_button_text)
		PaintContext::DestroyText(m_button_text);
}

bool LCARS_Button_Rounded::IsInInnerRect(int x, int y, SDL_Rect abs) {
	return (x >= m_inner_rect.x + abs.x && x <= (m_inner_rect.x + m_bounds.w - m_bounds.h/2*!m_round_right - m_bounds.h*m_round_right + abs.x)) &&
			(y >= m_inner_rect.y + abs.y && y <= (m_inner_rect.y + m_inner_rect.h + abs.y));
}

bool LCARS_Button_Rounded::IsInCircles(int x, int y, SDL_Rect abs) {
	bool cl = is_in_circle(x, y, m_circle_radius + abs.x, m_circle_radius + abs.y, m_circle_radius);
	bool cr = is_in_circle(x, y, m_circle_radius + abs.x + m_bounds.w - m_bounds.h, m_circle_radius + abs.y, m_circle_radius);

	return (cl && m_round_left) || (cr && m_round_right);
}

bool LCARS_Button_Rounded::PointInHitbox(int x, int y) {
	SDL_Rect abs = GetAbsoluteBounds();
	return IsInInnerRect(x, y, abs) || IsInCircles(x, y, abs);
}


void LCARS_Button_Rounded::Paint(PaintContext * paintctx) {

	SDL_Color * c;

	if(m_has_pd_focus) {
		c = GetColor(BTN_COLOR_TYPE::COLOR_HOVER);
	}
	else if(m_has_kb_focus)
		c = GetColor(BTN_COLOR_TYPE::COLOR_ACTIVE);
	else if(m_pressed)
		c = GetColor(BTN_COLOR_TYPE::COLOR_PRESS);
	else
		c = GetColor(BTN_COLOR_TYPE::COLOR_IDLE);

	paintctx->SetColor(c->r, c->g, c->b, c->a);
	paintctx->FillRect(&m_inner_rect);

	int rx = (int)(m_circle_radius*c_ellipse_factor);

	if(m_round_left)
		paintctx->FillEllipse(m_circle_radius, m_circle_radius, rx, m_circle_radius);

	if(m_round_right)
		paintctx->FillEllipse(m_circle_radius + m_bounds.w - m_bounds.h, m_circle_radius, rx, m_circle_radius);


	if(!m_button_text) {
		paintctx->SetColor(255, 255, 255, 255);
		paintctx->SetFont(m_font);

		m_button_text = paintctx->PrepareBlendedText(0, 0, m_text);

		if(!m_button_text) {
			std::cerr << "Button Text of Rounded Button could not be prepared!\n";
		}

		SDL_Rect * bounds = &m_button_text->bounds;
		bounds->x  = m_bounds.w - m_bounds.h/2 - bounds->w - 15*!m_round_right;
		bounds->y  = m_bounds.h - bounds->h;
	}

	/* When there is a Text applied, draw it here. */
	if(m_button_text) {
		SDL_Rect src = {0, 0, m_button_text->bounds.w, m_button_text->bounds.h};
		SDL_Rect dst = {m_button_text->bounds.x, m_button_text->bounds.y, m_button_text->bounds.w, m_button_text->bounds.h};

		paintctx->DrawText(m_button_text, &src, &dst);
	}
}


void LCARS_Button_Rounded::HandleSDLEvent(SDL_Event * ev) {
	LCARS_Button::HandleSDLEvent(ev);
}


void LCARS_Button_Rounded::OnUnhandledSDLEvent(SDL_Event * ev) {

}

void LCARS_Button_Rounded::SetRoundLeft(bool b) {
	m_round_left = b;

	if(b) {
		m_inner_rect.x = m_bounds.h/2;
		m_inner_rect.w = m_bounds.w - m_bounds.h;
	} else {
		m_inner_rect.x = 0;
		m_inner_rect.w = m_bounds.w - m_bounds.h/2;
	}
}

void LCARS_Button_Rounded::SetRoundRight(bool b) {
	m_round_right = b;

	if(b) {
		m_inner_rect.w = m_bounds.w - m_bounds.h;
	} else {
		m_inner_rect.w = m_bounds.w - m_bounds.h/2;
	}
}
