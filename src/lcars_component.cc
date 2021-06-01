#include "lcars_component.hh"

#include <iostream>

LCARS::Component::Component(Rect rect) : ICP(false) {
		m_bounds = rect;
		m_has_pd_focus = false;
		m_has_kb_focus = false;
}

LCARS::Component::~Component() {

}

Rect LCARS::Component::GetAbsoluteBounds() {
	Rect pos = {0, 0, 0, 0};

	if(m_parent) {
		pos = m_parent->GetAbsoluteBounds();
	}

	pos.w = m_bounds.w;
	pos.h = m_bounds.h;

	pos.x += m_bounds.x;
	pos.y += m_bounds.y;

	return pos;
}

void LCARS::Component::SetNeedsRepaint(bool b) {
	m_needs_repaint = b;

	if(m_parent)
		m_parent->SetNeedsRepaint(b);
}

void LCARS::Component::SetPosX(int x) {
	m_bounds.x = x;
	SetNeedsRepaint(true);
}

int LCARS::Component::GetPosX() {
	return m_bounds.x;
}

void LCARS::Component::SetPosY(int y) {
	m_bounds.y = y;
	SetNeedsRepaint(true);
}

int LCARS::Component::GetPosY() {
	return m_bounds.y;
}

void LCARS::Component::SetHeight(int h) {
	m_bounds.h = h;
	SetNeedsRepaint(true);
}

int LCARS::Component::GetHeight() {
	return m_bounds.h;
}

void LCARS::Component::SetWidth(int w) {
	m_bounds.w = w;
	SetNeedsRepaint(true);
}

int LCARS::Component::GetWidth() {
	return m_bounds.w;
}

void LCARS::Component::SetInterface(Interface * interface) {
	m_interface = interface;

	for(int i = 0; i < m_children.Size(); ++i) {
		m_children[i]->SetInterface(interface);
	}
}

void LCARS::Component::Draw(Display * dpy, Window w, cairo_surface_t * interface_surface) {

		/* We have to redraw components, that do not themselves need a repaint but otherwise */
		/* siblings of components that need redraws may get overdrawn by their parent. */
		//if(!NeedsRepaint()) return;

		Rect abs = GetAbsoluteBounds();

		PaintContext ctx(dpy, w, interface_surface, abs);
		Paint(&ctx);
		ctx.PaintScreen();

		for(int i = 0; i < m_children.Size(); ++i) {
			m_children[i]->Draw(dpy, w, interface_surface);
		}

		m_needs_repaint = false;
}

LCARS::Component * LCARS::Component::ComponentAt(int x, int y) {
	for(int i = 0; i < m_children.Size(); ++i) {
		if(m_children[i]->ComponentAt(x, y)) {
			return m_children[i];
		}
	}

	if(PointInHitbox(x, y))
		return this;
	return nullptr;
}

void LCARS::Component::AddChild(Component * cmp) {
	m_children.Add(cmp);
	cmp->SetParent(this);
	cmp->SetInterface(m_interface);
}

void LCARS::Component::RemChild(Component * cmp) {
	cmp->SetParent(nullptr);
	m_children.Rem(cmp);
}

void LCARS::Component::AddEventListener(event_listener listener) {
	if(listener)
		m_ev_listeners += listener;
}

void LCARS::Component::RemEventListener(event_listener listener) {
	if(listener)
		m_ev_listeners -= listener;
}

void LCARS::Component::DispatchEvent(Event event) {
	for(int i = 0; i < m_ev_listeners.Size(); ++i) {
		m_ev_listeners[i](&event);
	}
	
	if(!event.is_cancelled) {
		if(event.type == LCARS::EVENT_TYPE::X_EVENT)
			HandleSDLEvent(event.xevent);
		else
			HandleCMPEvent(event.cmp_event);
	}
}

void LCARS::Component::SetPDFocus(bool b) {
	m_has_pd_focus = b;
}

void LCARS::Component::SetKBFocus(bool b) {
	m_has_kb_focus = b;
}

void LCARS::Component::SetParent(ICP * parent) {
	m_parent = parent;
}
