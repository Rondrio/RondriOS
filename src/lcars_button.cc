#include "lcars_button.hh"

#include <cstring>
#include <cstdio>

#include "lcars_font.hh"
#include "lcars_colors.hh"

LCARS::Button::Button(int x, int y, int w, int h, _font * font, std::string text) : Component({x, y, w, h}) {

	m_pressed	= false;
	m_colors	= (_color *) malloc(sizeof(_color) * 8);
	memset(m_colors, 0, sizeof(_color)*8);

	m_text = nullptr;
	
	m_capped_left	= false;
	m_capped_right	= false;
	
	m_cap_offset_left	= 0;
	m_cap_offset_right	= 0;

	m_base_bounds = {x, y, w, h};

	SetText(text);
	SetFont(font);

	LCARS::TextSize(m_text);
	
	SetColor(BTN_COLOR_TYPE::COLOR_IDLE,	COL_AMBER	);
	SetColor(BTN_COLOR_TYPE::COLOR_HOVER,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_PRESS,	COL_SKYBLUE	);
	SetColor(BTN_COLOR_TYPE::COLOR_ACTIVE,	COL_SKYBLUE	);

	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE,	{0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_HOVER,	{0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_PRESS,	{0, 0, 0, 255});
	SetColor(BTN_COLOR_TYPE::TEXT_COLOR_ACTIVE,	{0, 0, 0, 255});
}

LCARS::Button::~Button() {
	if(m_text) {
		free(m_text->str);
		free(m_text);
	}

	free(m_colors);
}

int LCARS::Button::CalcCapRadius() {
	return c_ellipse_factor*m_base_bounds.h/2;
}

void LCARS::Button::Rebound() {
	
	int cap_radius		= CalcCapRadius();
	
	int cap_offset_l	= m_capped_left*(m_cap_offset_left + (m_cap_offset_left > 0)*cap_radius);
	int cap_offset_r	= m_capped_right*(m_cap_offset_right + (m_cap_offset_right > 0)*cap_radius);
	
	m_bounds.w = m_base_bounds.w + m_capped_left*(cap_radius + cap_offset_l) + m_capped_right*(cap_radius + cap_offset_r);
	m_bounds.h = m_base_bounds.h;
	
	m_bounds.x = m_base_bounds.x - cap_offset_l - m_capped_left*cap_radius;
	m_bounds.y = m_base_bounds.y;
}

void LCARS::Button::OnMouseDown(XButtonPressedEvent * ev) {
	m_pressed = true;
	SetNeedsRepaint(true);
}

void LCARS::Button::OnMouseUp(XButtonReleasedEvent * ev) {
	m_pressed = false;

	for(int i = 0; i < m_action_listeners.Size(); ++i)
		m_action_listeners[i]();

	SetNeedsRepaint(true);
}

void LCARS::Button::OnMouseMove(XMotionEvent * ev) {
	
}

void LCARS::Button::AddActionListener(action_listener listener) {
	m_action_listeners += listener;
}

void LCARS::Button::RemActionListener(action_listener listener) {
	m_action_listeners -= listener;
}

void LCARS::Button::SetFont(_font * font) {
	m_text->font = font;
	SetNeedsRepaint(true);
}

void LCARS::Button::SetText(std::string text) {

	if(m_text)
		free(m_text->str);
	else
		m_text = (Text *) malloc(sizeof(Text));

	m_text->str = (char *) malloc(text.length());
	strcpy(m_text->str, text.c_str());

	SetNeedsRepaint(true);
}

Text * LCARS::Button::GetText() {
	return m_text;
}

void LCARS::Button::SetColor(BTN_COLOR_TYPE colortype, _color color) {
	m_colors[(int) colortype] = color;
}

_color LCARS::Button::GetColor(BTN_COLOR_TYPE colortype) {
	return m_colors[(int) colortype];
}

void LCARS::Button::SetCappedLeft(bool b) {
	m_capped_left = b;
	
	Rebound();
	SetNeedsRepaint(true);
}

void LCARS::Button::SetCappedRight(bool b) {
	m_capped_right = b;
	
	Rebound();
	SetNeedsRepaint(true);
}

void LCARS::Button::SetCapOffsetLeft(int amount) {
	m_cap_offset_left	= amount;
	
	Rebound();
	SetNeedsRepaint(true);
}

void LCARS::Button::SetCapOffsetRight(int amount) {
	m_cap_offset_right = amount;
	
	Rebound();
	SetNeedsRepaint(true);
}

void LCARS::Button::SetPosX(int x) {
	m_base_bounds.x = x;
	Rebound();
}

int LCARS::Button::GetPosX() {
	return m_base_bounds.x;
}

void LCARS::Button::SetPosY(int y) {
	m_base_bounds.y = y;
	Rebound();
}

int LCARS::Button::GetPosY() {
	return m_base_bounds.y;
}


void LCARS::Button::SetWidth(int width) {
	m_base_bounds.w = width;
	
	Rebound();
	SetNeedsRepaint(true);
}

int LCARS::Button::GetWidth() {
	return m_base_bounds.w;
}

bool LCARS::Button::PointInHitbox(int x, int y) {
	Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + abs.w && y >= abs.y && y <= abs.y + abs.h;
}

void LCARS::Button::Paint(PaintContext * paintctx) {
	
	_color c;
	
	int cap_offset_l = m_capped_left*(m_cap_offset_left + (m_cap_offset_left > 0)*m_bounds.h/2);
	int cap_offset_r = m_capped_right*(m_cap_offset_right + (m_cap_offset_right > 0)*m_bounds.h/2);
	
	if(m_has_pd_focus)
		c = GetColor(BTN_COLOR_TYPE::COLOR_HOVER);
	else if(m_has_kb_focus)
		c = GetColor(BTN_COLOR_TYPE::COLOR_ACTIVE);
	else if(m_pressed)
		c = GetColor(BTN_COLOR_TYPE::COLOR_PRESS);
	else
		c = GetColor(BTN_COLOR_TYPE::COLOR_IDLE);
	
	paintctx->SetColor(c.r, c.g, c.b, c.a);
	paintctx->FillRect(m_base_bounds.x - m_bounds.x, 0, m_base_bounds.w, m_base_bounds.h);
	
	if(m_capped_left) {
		paintctx->FillEllipse(CalcCapRadius(), m_bounds.h/2, CalcCapRadius(), m_bounds.h/2);
	
		if(m_cap_offset_left > 0) {
			paintctx->FillRect(CalcCapRadius(), 0, CalcCapRadius(), m_bounds.h);
		}
	}
	
	if(m_capped_right) {
		paintctx->FillEllipse(m_bounds.w - CalcCapRadius(), m_bounds.h/2, CalcCapRadius(), m_bounds.h/2);
		
		if(m_cap_offset_right > 0) {
			paintctx->FillRect(m_bounds.w - 2*CalcCapRadius(), 0, CalcCapRadius(), m_bounds.h);
		}
	}
	
	if(m_has_pd_focus)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_HOVER);
	else if(m_has_kb_focus)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_ACTIVE);
	else if(m_pressed)
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_PRESS);
	else
		c = GetColor(BTN_COLOR_TYPE::TEXT_COLOR_IDLE);
	
	if(m_text) {
		paintctx->SetColor(c.r, c.g, c.b, c.a);
		paintctx->DrawText(m_base_bounds.x - m_bounds.x + m_base_bounds.w - m_text->w - 10, m_bounds.h - m_text->h - m_bounds.h/6, m_text);
	}
}

void LCARS::Button::OnUnhandledSDLEvent(XEvent * ev) {
	
}

void LCARS::Button::HandleSDLEvent(XEvent * ev) {	
	switch(ev->type) {
		case ButtonPress:
			OnMouseDown(&(ev->xbutton));
			break;
		case ButtonRelease:
			OnMouseUp(&(ev->xbutton));
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