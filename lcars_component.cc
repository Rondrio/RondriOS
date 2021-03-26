#include "lcars_component.hh"

#include <iostream>
#include <SDL2/SDL.h>

LCARS_Component::LCARS_Component(SDL_Rect rect) : LCARS_ICP(false) {
		m_bounds = rect;
		m_has_pd_focus = false;
		m_has_kb_focus = false;
}

LCARS_Component::~LCARS_Component() {

}

SDL_Rect LCARS_Component::GetAbsoluteBounds() {
	SDL_Rect pos = {0, 0, 0, 0};

	if(m_parent) {
		pos = m_parent->GetAbsoluteBounds();
	}

	pos.w = m_bounds.w;
	pos.h = m_bounds.h;

	pos.x += m_bounds.x;
	pos.y += m_bounds.y;

	return pos;
}

void LCARS_Component::SetNeedsRepaint(bool b) {
	m_needs_repaint = b;

	if(m_parent)
		m_parent->SetNeedsRepaint(b);
}

void LCARS_Component::SetPosX(int x) {
	m_bounds.x = x;
	SetNeedsRepaint(true);
}

int LCARS_Component::GetPosX() {
	return m_bounds.x;
}

void LCARS_Component::SetPosY(int y) {
	m_bounds.y = y;
	SetNeedsRepaint(true);
}

int LCARS_Component::GetPosY() {
	return m_bounds.y;
}

void LCARS_Component::SetHeight(int h) {
	m_bounds.h = h;
	SetNeedsRepaint(true);
}

int LCARS_Component::GetHeight() {
	return m_bounds.h;
}

void LCARS_Component::SetWidth(int w) {
	m_bounds.w = w;
	SetNeedsRepaint(true);
}

int LCARS_Component::GetWidth() {
	return m_bounds.w;
}

void LCARS_Component::SetInterface(LCARS_Interface * interface) {
	m_interface = interface;

	for(int i = 0; i < m_children.Size(); ++i) {
		m_children[i]->SetInterface(interface);
	}
}

void LCARS_Component::Draw(SDL_Renderer * renderer, SDL_Texture * buffer) {
		
		if(!NeedsRepaint()) return;
		
		SDL_Rect abs = GetAbsoluteBounds();

		PaintContext ctx(renderer, buffer, abs);
		Paint(&ctx);
		ctx.PaintScreen();

		for(int i = 0; i < m_children.Size(); ++i) {
			m_children[i]->Draw(renderer, buffer);
		}

		m_needs_repaint = false;
}

LCARS_Component * LCARS_Component::ComponentAt(int x, int y) {
	for(int i = 0; i < m_children.Size(); ++i) {
		if(m_children[i]->ComponentAt(x, y)) {
			return m_children[i];
		}
	}

	if(PointInHitbox(x, y))
		return this;
	return nullptr;
}

void LCARS_Component::AddChild(LCARS_Component * cmp) {
	m_children.Add(cmp);
	cmp->SetParent(this);

	for(int i = 0; i < m_children.Size(); ++i) {
		m_children[i]->SetInterface(m_interface);
	}
}

void LCARS_Component::RemChild(LCARS_Component * cmp) {
	cmp->SetParent(nullptr);
	m_children.Rem(cmp);
}

void LCARS_Component::AddEventListener(event_listener listener) {
	if(listener)
		m_ev_listeners += listener;
}

void LCARS_Component::RemEventListener(event_listener listener) {
	if(listener)
		m_ev_listeners -= listener;
}

void LCARS_Component::DispatchEvent(Event event) {
	for(int i = 0; i < m_ev_listeners.Size(); ++i) {
		m_ev_listeners[i](event);
	}
}

void LCARS_Component::SetPDFocus(bool b) {
	m_has_pd_focus = b;
}

void LCARS_Component::SetKBFocus(bool b) {
	m_has_kb_focus = b;
}

void LCARS_Component::SetParent(LCARS_ICP * parent) {
	m_parent = parent;
}
