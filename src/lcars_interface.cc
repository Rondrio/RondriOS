#include "lcars_interface.hh"

#include <iostream>

LCARS::Interface::Interface(int x, int y, int width, int height) : ICP(true) {
	m_bounds.x = x;
	m_bounds.y = y;
	m_bounds.w = width;
	m_bounds.h = height;

	m_lcars_screen	= nullptr;
	m_cmp_kb_focus	= nullptr;
	m_cmp_pd_focus	= nullptr;

	m_needs_buffer_repaint	= false;

	hp = height/1000.0;
	wp = width/1000.0;
	
	m_interface_surface = nullptr;
}

LCARS::Interface::~Interface() {
	if(m_interface_surface)
		cairo_surface_destroy(m_interface_surface);
}

void LCARS::Interface::Init(Display * dpy, Window w) {
	m_interface_surface = cairo_xlib_surface_create(dpy, w, DefaultVisual(dpy, 0), m_bounds.w, m_bounds.h);
}

void LCARS::Interface::Draw(Display * dpy, Window w, Pixmap buffer) {
	
	if(NeedsRepaint()) {

		/* PAINT Components */
		for(int i = 0; i < m_components.Size(); ++i) {
			Component * cmp = m_components[i];
			
			if(cmp->NeedsRepaint())
				cmp->Draw(dpy, w, m_interface_surface);
		}
		

		XGCValues gcvalues;
		GC gc = XCreateGC(dpy, w, 0, &gcvalues);
		
		XCopyArea(dpy, w, buffer, gc, 0, 0, m_bounds.w, m_bounds.h, 0, 0);
		m_needs_repaint = false;
		
		//cairo_surface_flush(m_interface_surface);
		XFlush(dpy);
		XSync(dpy, 0);
	}

	/* Repaint when a Window got moved */
	/* The strategy is that the Screen Buffer (all LCARS_Components, for example) */
	/* is redrawn and on top of that the Window Decoration is Drawn seperatly. */
	if(m_needs_buffer_repaint) {
		
		XGCValues gcvalues;
		GC gc = XCreateGC(dpy, w, 0, &gcvalues);

		XCopyArea(dpy, buffer, w, gc, 0, 0, m_bounds.w, m_bounds.h, 0, 0);
		m_needs_buffer_repaint = false;
	}

	/* Priority Redraws. (This implementation allows for one Priority Redraw per "Frame") */
	/* This may (or should) be changed in the future. */
	if(m_priority_repaints.size() > 0) {
		Component * cmp = m_priority_repaints.back();
		m_priority_repaints.pop();

		std::cout << "Priority Redraw!\n";

		cmp->Draw(dpy, w, m_interface_surface);
	}

}

void LCARS::Interface::AttachToScreen(Monitor * screen) {
	m_lcars_screen = screen;
}

void LCARS::Interface::AddComponent(Component * cmp) {
	m_components += cmp;
	cmp->SetParent(this);

	cmp->SetInterface(this);
}

void LCARS::Interface::RemComponent(Component * cmp) {
	m_components -= cmp;

	cmp->SetInterface(nullptr);
}

void LCARS::Interface::AddPriorityRepaint(Component * cmp) {
	m_priority_repaints.push(cmp);
}

void LCARS::Interface::SetNeedsRepaint(bool b) {
	m_needs_repaint = b;
}

void LCARS::Interface::SetNeedsBufferRepaint(bool b) {
	m_needs_buffer_repaint = true;
}

LCARS::Monitor * LCARS::Interface::GetScreen() {
	return m_lcars_screen;
}

LCARS::Component * LCARS::Interface::ComponentAt(int x, int y) {
	for(int i = 0; i < m_components.Size(); ++i) {
		Component * cmp = m_components[i];
		Component * cmp_at;

		if((cmp_at = cmp->ComponentAt(x, y)))
			return cmp_at;
	}

	return nullptr;
}

void LCARS::Interface::DispatchSDLEvents(XEvent * ev) {

	switch(ev->type) {
		case MotionNotify:
		case ButtonPress:
		case ButtonRelease: {

			/* Set, who is going to have the FOCUS */
			if(ev->type == MotionNotify) {
				XMotionEvent xme = ev->xmotion;
				Component * cmp_mouse_over = ComponentAt(xme.x, xme.y);


				/* REMOVE PD Focus from the currently focused Compnent */
				if(m_cmp_pd_focus && cmp_mouse_over != m_cmp_pd_focus) {
					m_cmp_pd_focus->SetPDFocus(false);
					//m_cmp_pd_focus->HandleCMPEvent(CMP_EVT_TYPE::PD_BLUR);
					m_cmp_pd_focus->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::PD_BLUR, false}
					);
				}

				/* SET PD Focus to the Component with Mouse Over */
				if(cmp_mouse_over && cmp_mouse_over != m_cmp_pd_focus) {
					cmp_mouse_over->SetPDFocus(true);
					//cmp_mouse_over->HandleCMPEvent(CMP_EVT_TYPE::PD_FOCUS);
					cmp_mouse_over->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::PD_FOCUS, false}
					);
				}

				/* Update Interface's Pointer to the focused Component */
				m_cmp_pd_focus = cmp_mouse_over;

			} else if(ev->type == ButtonPress) {

				/* All of the Code in this Section is there to correctly KB focus Components */

				XButtonPressedEvent xbe = ev->xbutton;
				Component *		cmp_mouse_down	= ComponentAt(xbe.x, xbe.y);

				/* Remove KB Focus from the currently focused Component IF there was one AND IF it is */
				/* not the same as the one fetched above */
				if(m_cmp_kb_focus && cmp_mouse_down != m_cmp_kb_focus) {
					m_cmp_kb_focus->SetKBFocus(false);
					//m_cmp_kb_focus->HandleCMPEvent(CMP_EVT_TYPE::KB_BLUR);
					m_cmp_kb_focus->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::KB_BLUR, false}
					);
				}

				/*Set KB Focus to the Component that was clicked on IF there was a Component clicked on */
				if(cmp_mouse_down) {
					cmp_mouse_down->SetKBFocus(true);
					//cmp_mouse_down->HandleCMPEvent(CMP_EVT_TYPE::KB_FOCUS);
					cmp_mouse_down->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::KB_FOCUS, false}
					);
				}

				/* Update Focus */
				m_cmp_kb_focus = cmp_mouse_down;
			}

			/* Dispatch SDL Mouse Events to Component with PD Focus */
			if(m_cmp_pd_focus) {
				//m_cmp_pd_focus->HandleSDLEvent(ev);
				m_cmp_pd_focus->DispatchEvent(
						{EVENT_TYPE::X_EVENT, ev, (LCARS::CMP_EVT_TYPE) -1, false}
				);
			}

			break;
		}

		case KeyPress:
		case KeyRelease:{

			/* Dispatch SDL Key Events to Component with KB Focus */
			if(m_cmp_kb_focus) {
				//m_cmp_kb_focus->HandleSDLEvent(ev);
				m_cmp_kb_focus->DispatchEvent(
						{EVENT_TYPE::X_EVENT, ev, (LCARS::CMP_EVT_TYPE) -1, false}
				);
			}

			break;
		}
	}

}
