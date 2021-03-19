#include "lcars_interface.hh"

#include <iostream>

LCARS_Interface::LCARS_Interface(int x, int y, int width, int height) {
	m_bounds.x = x;
	m_bounds.y = y;
	m_bounds.w = width;
	m_bounds.h = height;

	m_lcars_screen	= nullptr;
	m_cmp_kb_focus	= nullptr;
	m_cmp_pd_focus	= nullptr;
	m_wnd_focus		= nullptr;

	m_windows		= new smp::list<LCARS_Window *>();
	m_components	= new smp::list<LCARS_Component *>();

	m_window_move.moving_enabled = false;
	m_window_move.window = nullptr;
	m_window_move.x_offset = 0;
	m_window_move.y_offset = 0;
}

int repaints = 0;

void LCARS_Interface::Draw(SDL_Renderer * renderer) {

	if(NeedsRepaint()) {
		SDL_RenderClear(renderer);

		/* PAINT Components */
		for(int i = 0; i < m_components->Size(); ++i) {
			LCARS_Component * cmp = (*m_components)[i];
			cmp->Draw(renderer);
		}

		/* DRAW Window Decorations */
			for(int i = 0; i < m_windows->Size(); ++i) {
				LCARS_Window * lcars_window = (*m_windows)[i];

				if(lcars_window->GetDecoration())
					lcars_window->GetDecoration()->Draw(renderer);
			}

		SetNeedsRepaint(false);
		SDL_RenderPresent(renderer);
	}
}

void LCARS_Interface::AttachToScreen(LCARS_Screen * screen) {
	m_lcars_screen = screen;
}

void LCARS_Interface::AddWindow(Window w) {
	if(m_lcars_screen) {
		LCARS_Window * lcars_window = new LCARS_Window(this, m_lcars_screen->GetDisplay(), w);
		*m_windows += lcars_window;
	}
}

void LCARS_Interface::RemWindow(Window w) {
	for(int i = 0; i < m_windows->Size(); ++i) {
		if((*m_windows)[i]->GetXWindow() == w) {
			*m_windows -= (*m_windows)[i];
			break;
		}
	}
}

void LCARS_Interface::AddComponent(LCARS_Component * cmp) {
	*m_components += cmp;
	cmp->SetParent(this);
}

void LCARS_Interface::RemComponent(LCARS_Component * cmp) {
	*m_components -= cmp;
}

void LCARS_Interface::SetNeedsRepaint(bool b) {
	m_needs_repaint = b;
}

void LCARS_Interface::SetFocusedWindow(Window w) {
	for(int i = 0; i < m_windows->Size(); ++i) {
		if((*m_windows)[i]->GetXWindow() == w) {
			SetFocusedWindow((*m_windows)[i]);
			break;
		}
	}
}

void LCARS_Interface::SetFocusedWindow(LCARS_Window * w) {
	m_wnd_focus = w;
}

LCARS_Window * LCARS_Interface::GetFocusedWindow() {
	return m_wnd_focus;
}

LCARS_Window * LCARS_Interface::IsOnLCARS_Window(int x, int y) {
	for(int i = 0; i < m_windows->Size(); ++i) {
		if((*m_windows)[i]->GetDecoration()->IsOnMove(x, y)) {
			return (*m_windows)[i];
		}
	}

	return nullptr;
}

LCARS_Component * LCARS_Interface::ComponentAt(int x, int y) {
	for(int i = 0; i < m_components->Size(); ++i) {
		LCARS_Component * cmp = (*m_components)[i];
		LCARS_Component * cmp_at;

		if((cmp_at = cmp->ComponentAt(x, y)))
			return cmp_at;
	}

	return nullptr;
}

void LCARS_Interface::DispatchSDLEvents(SDL_Event * ev) {

	if(ev->type == SDL_MOUSEBUTTONDOWN) {
		SDL_MouseButtonEvent mbe = ev->button;

		/* Check for left Mouse Button Down */
		if(mbe.button == SDL_BUTTON_LEFT) {

			LCARS_Window * lcars_window = IsOnLCARS_Window(mbe.x, mbe.y);

			int click_x = mbe.x;
			int click_y = mbe.y;

			/* If there is a Window at the Mouse Button Down Position, enable move */
			if(lcars_window) {

				/* Enable Move */
				m_wnd_focus = lcars_window;
				m_window_move.moving_enabled = true;
				m_window_move.window = lcars_window;

				/* Raise to front and update xy-offsets */
				/* The Offsets are meant to calculate the Position of the Window, when moving the Mouse */
				m_window_move.window->GetDecoration()->AbsoluteToRelative(click_x, click_y, &(m_window_move.x_offset), &(m_window_move.y_offset));
				m_window_move.window->Raise();

			} else {

				/* There is no Window under the Pointer, so no active Window == nullptr */
				m_wnd_focus = nullptr;

				/* I'm not sure if this is a good Idea but we'll see */
				m_window_move.window = 0;

				/* The Window that's going to be retrieved by SDL_GetWindowFromID hopefully is */
				/* the LCARS's Main Window */
				SDL_SetWindowInputFocus(SDL_GetWindowFromID(mbe.windowID));
			}
		}

	} else if(ev->type == SDL_MOUSEBUTTONUP) {

		/* Disable Window moving as soon as the Mouse is no longer held down */
		m_window_move.moving_enabled = false;
	}

	switch(ev->type) {
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
		case SDL_MOUSEMOTION: {

			/* Set, who is going to have the FOCUS */
			if(ev->type == SDL_MOUSEMOTION) {
				SDL_MouseMotionEvent * mme = &(ev->motion);
				LCARS_Component * cmp_mouse_over = ComponentAt(mme->x, mme->y);


				/* REMOVE PD Focus from the currently focused Compnent */
				if(m_cmp_pd_focus && cmp_mouse_over != m_cmp_pd_focus) {
					m_cmp_pd_focus->SetPDFocus(false);
					m_cmp_pd_focus->HandleCMPEvent(CMP_EVT_TYPE::PD_BLUR);
					m_cmp_pd_focus->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::PD_BLUR}
					);
				}

				/* SET PD Focus to the Component with Mouse Over */
				if(cmp_mouse_over && cmp_mouse_over != m_cmp_pd_focus) {
					cmp_mouse_over->SetPDFocus(true);
					cmp_mouse_over->HandleCMPEvent(CMP_EVT_TYPE::PD_FOCUS);
					cmp_mouse_over->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::PD_FOCUS}
					);
				}

				/* Update Interface's Pointer to the focused Component */
				m_cmp_pd_focus = cmp_mouse_over;

				/* Move Window if Mouse Button held down */
				if(m_window_move.moving_enabled && m_window_move.window) {

					m_window_move.window->Move(
							mme->x - m_window_move.x_offset,
							mme->y + m_window_move.y_offset
						);

					/* To not screw the whole Background, a full repaint is done for every Tick, the Window */
					/* is moved  */
					SetNeedsRepaint(true);
				}

			} else if(ev->type == SDL_MOUSEBUTTONDOWN) {

				/* All of the Code in this Section is there to correctly KB focus Components */

				SDL_MouseButtonEvent *	mbe				= &(ev->button);
				LCARS_Component *		cmp_mouse_down	= ComponentAt(mbe->x, mbe->y);

				/* Remove KB Focus from the currently focused Component IF there was one AND IF it is */
				/* not the same as the one fetched above */
				if(m_cmp_kb_focus && cmp_mouse_down != m_cmp_kb_focus) {
					m_cmp_kb_focus->SetKBFocus(false);
					m_cmp_kb_focus->HandleCMPEvent(CMP_EVT_TYPE::KB_BLUR);
					m_cmp_kb_focus->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::KB_BLUR}
					);
				}

				/*Set KB Focus to the Component that was clicked on IF there was a Component clicked on */
				if(cmp_mouse_down) {
					cmp_mouse_down->SetKBFocus(true);
					cmp_mouse_down->HandleCMPEvent(CMP_EVT_TYPE::KB_FOCUS);
					cmp_mouse_down->DispatchEvent(
							{EVENT_TYPE::CMP_EVENT, nullptr, CMP_EVT_TYPE::KB_FOCUS}
					);
				}

				/* Update Focus */
				m_cmp_kb_focus = cmp_mouse_down;
			}

			/* Dispatch SDL Mouse Events to Component with PD Focus */
			if(m_cmp_pd_focus) {
				m_cmp_pd_focus->HandleSDLEvent(ev);
				m_cmp_pd_focus->DispatchEvent(
						{EVENT_TYPE::SDL_EVENT, ev}
				);
			}

			break;
		}

		case SDL_TEXTEDITING:
		case SDL_TEXTINPUT:
		case SDL_KEYDOWN:
		case SDL_KEYUP: {

			/* Dispatch SDL Key Events to Component with KB Focus */
			if(m_cmp_kb_focus) {
				m_cmp_kb_focus->HandleSDLEvent(ev);
				m_cmp_kb_focus->DispatchEvent(
						{EVENT_TYPE::SDL_EVENT, ev}
				);
			}

			break;
		}
	}

}
