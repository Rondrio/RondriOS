#ifndef LCARS_WINDOWDECORATION_HH_
#define LCARS_WINDOWDECORATION_HH_

#include <X11/Xlib.h>

#include "lcars_window.hh"

class LCARS_Window;

class LCARS_WindowDecoration {

	private:
		const int c_TAB_HEIGHT		= 20;
		const int c_BORDER_WIDTH	= 5;

		LCARS_Window *		m_lcars_window;
		XWindowAttributes	m_wa;
		Window				m_window_to_decorate;

		Display *			m_dpy;

	public:
		LCARS_WindowDecoration(LCARS_Window * lcars_window, Display * dpy, Window window_to_decorate);

		virtual ~ LCARS_WindowDecoration();

		virtual void Draw(SDL_Renderer * renderer);
		virtual void UpdateDecorationBounds();

		virtual bool IsOnMove		(int x, int y);
		virtual void SnapToScreen	(int x, int y, int * x_rtrn, int * y_rtrn);

		virtual void AbsoluteToRelative(int x, int y, int * x_rtrn, int * y_rtrn);

		virtual SDL_Rect GetBounds();
};


#endif /* LCARS_WINDOWDECORATION_HH_ */
