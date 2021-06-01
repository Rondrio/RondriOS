#ifndef PAINT_CONTEXT_HH_
#define PAINT_CONTEXT_HH_

#include <X11/Xft/Xft.h>
#include <X11/Xlib.h>
#include <cairo/cairo.h>

#include <string>

#include "simple_list.hh"

struct bezier_t {
	signed short * vx;
	signed short * vy;
	int n;
	int s;
};

struct Rect {
	int32_t x, y, w, h;
};

struct _font {
	char * family;
	double ptsize;
	XftFont * xftfont;
};

struct Text {
	char * str;
	_font * font;
	uint16_t w, h;
};

struct _color {
	uint8_t r, g, b, a;
};

struct _shape {
	uint8_t type;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	_color c;
};

class PaintContext {
	private:
		Display *	m_dpy;
		Window		m_xwindow;
		XftDraw *	m_xftdraw;
		GC			m_gc;
		XGCValues	m_gcvalues;
		
		cairo_surface_t * m_canvas_surface;
		
		Rect		m_bounds;
		bool		m_flipx;
		bool		m_flipy;
		
		smp::list<_shape *> *		m_drawqueue;
		_color						m_color;

	public:
			PaintContext()						= delete;
			PaintContext(const PaintContext&)	= delete;
			PaintContext(Display * dpy, Window w, cairo_surface_t * canvas_surface, Rect bounds);
		~	PaintContext();

		void FlipX();
		void FlipY();

		void SetColor	(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetFont	();

		void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

		void FillCircle(int16_t x, int16_t y, int16_t rad);
		void DrawCircle(int16_t x, int16_t y, int16_t rad);

		void FillEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry);
		void DrawEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry);

		void DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, cairo_surface_t * img);

		void PaintScreen();

		void DrawText(uint16_t x, uint16_t y, Text * text);

		PaintContext& operator=(const PaintContext&) = delete;
};


#endif /* PAINT_CONTEXT_HH_ */
