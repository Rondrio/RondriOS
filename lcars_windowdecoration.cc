#include "lcars_windowdecoration.hh"


LCARS_WindowDecoration::LCARS_WindowDecoration(LCARS_Window * lcars_window, Display * dpy, Window window_to_decorate) {
	m_lcars_window			= lcars_window;
	m_dpy					= dpy;
	m_window_to_decorate	= window_to_decorate;

	UpdateDecorationBounds();
}

LCARS_WindowDecoration::~LCARS_WindowDecoration() {
	// Empty
}

void LCARS_WindowDecoration::Draw(SDL_Renderer * renderer) {
	if(!m_lcars_window->IsFocused())
		SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
	else
		SDL_SetRenderDrawColor(renderer, 50, 100, 200, 255);

	SDL_Rect rect = {
			m_wa.x - c_BORDER_WIDTH,
			m_wa.y - c_TAB_HEIGHT,
			m_wa.width + c_BORDER_WIDTH*2,
			m_wa.height + c_TAB_HEIGHT + c_BORDER_WIDTH
	};

	SDL_RenderFillRect(renderer, &rect);
}

void LCARS_WindowDecoration::UpdateDecorationBounds() {
	XGetWindowAttributes(m_dpy, m_window_to_decorate, &m_wa);
}

bool LCARS_WindowDecoration::IsOnMove(int x, int y) {
	return (x >= m_wa.x - c_BORDER_WIDTH && x <= m_wa.x + m_wa.width + c_BORDER_WIDTH) &&
			(y >= m_wa.y - c_TAB_HEIGHT && y <= m_wa.y);
}

void LCARS_WindowDecoration::SnapToScreen(int x, int y, int * x_rtrn, int * y_rtrn) {
	if(y - c_TAB_HEIGHT < 0)
		*y_rtrn = c_TAB_HEIGHT;
	*x_rtrn = x;
}

void LCARS_WindowDecoration::AbsoluteToRelative(int x, int y, int * x_rtrn, int * y_rtrn) {
	*x_rtrn = x - m_wa.x;
	*y_rtrn = m_wa.y - y;
}

SDL_Rect LCARS_WindowDecoration::GetBounds() {
	SDL_Rect rect;
	rect.x = m_wa.x - c_BORDER_WIDTH;
	rect.y = m_wa.y - c_TAB_HEIGHT;
	rect.w = m_wa.width + c_BORDER_WIDTH*2;
	rect.h = m_wa.height + c_TAB_HEIGHT + c_BORDER_WIDTH;

	return rect;
}
