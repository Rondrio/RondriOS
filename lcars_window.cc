#include "lcars_window.hh"

LCARS_Window::LCARS_Window(LCARS_Interface * lcars_interface, Display * dpy, Window window) {
	m_interface			= lcars_interface;
	m_dpy				= dpy;
	m_window			= window;
	m_window_decoration = new LCARS_WindowDecoration(this, dpy, window);
}

LCARS_Window::~LCARS_Window() {
	if(m_window_decoration)
		delete m_window_decoration;
}

Window LCARS_Window::GetXWindow() {
	return m_window;
};

LCARS_WindowDecoration * LCARS_Window::GetDecoration() {
	return m_window_decoration;
}

void LCARS_Window::Move(int x, int y) {
	int _x = x, _y = y;
	m_window_decoration->SnapToScreen(x, y, &_x, &_y);

	XMoveWindow(m_dpy, m_window, _x, _y);
	m_window_decoration->UpdateDecorationBounds();
}

void LCARS_Window::MoveRelative(int x, int y) {
	SDL_Rect bounds = m_window_decoration->GetBounds();

	XMoveWindow(m_dpy, m_window, bounds.x + x, bounds.y + y);
	m_window_decoration->UpdateDecorationBounds();
}

void LCARS_Window::Raise() {
	XRaiseWindow(m_dpy, m_window);
}

bool LCARS_Window::IsFocused() {
	return this == m_interface->GetFocusedWindow();
}
