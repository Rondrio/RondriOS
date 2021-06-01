#include "paint_context.hh"

#include <iostream>
#include <string>
#include <X11/Xft/Xft.h>
#include <X11/Xutil.h>
#include <fontconfig/fontconfig.h>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include "paint_context_ex.hh"

enum _SHAPES {
	RECT, ELLIPSE, IMAGE, TEXT
};

enum STYLES {
	FILL, DRAW
};

struct _rect {
	uint8_t type;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	_color c;
	
	uint8_t fillstyle;
};

struct _ellipse {
	uint8_t type;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	_color c;
	
	uint16_t rx;
	uint16_t ry;
	
	uint8_t fillstyle;
};

struct _image {
	uint8_t type;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	_color c;
	
	cairo_surface_t * img_surface;
};

struct _text {
	uint8_t type;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	_color c;
	
	Text * text;
};

_rect * create_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t fillstyle, _color c) {
	_rect * r = (_rect *) malloc(sizeof(_rect));

	r->type			= RECT;
	r->x			= x;
	r->y			= y;
	r->w			= w;
	r->h			= h;
	r->c			= c;
	r->fillstyle	= fillstyle;

	return r;
}

_ellipse * create_ellipse(uint16_t x, uint16_t y, uint16_t rx, uint16_t ry, uint8_t fillstyle, _color c) {
	_ellipse * e = (_ellipse *) malloc(sizeof(_ellipse));

	e->type			= ELLIPSE;
	e->x			= x;
	e->y			= y;
	e->rx			= rx;
	e->ry			= ry;
	e->w			= 0;
	e->h			= 0;
	e->fillstyle	= fillstyle;
	e->c			= c;

	return e;
}

void create_xcolor(Display * dpy, _color c, XColor * c_out) {
		const int	X_COLOR_DEPTH	= 65535;
		const float	RGB_COLOR_DEPTH	= 255.0f;
	
		c_out->red		= c.r/RGB_COLOR_DEPTH * X_COLOR_DEPTH;
		c_out->green	= c.g/RGB_COLOR_DEPTH * X_COLOR_DEPTH;
		c_out->blue		= c.b/RGB_COLOR_DEPTH * X_COLOR_DEPTH;
		c_out->flags	= DoRed | DoGreen | DoBlue;
		
		XAllocColor(dpy, DefaultColormap(dpy, 0), c_out);
}

void destroy_xcolor(Display * dpy, XColor * c) {
	XFreeColors(dpy, DefaultColormap(dpy, 0), &(c->pixel), 1, 0);
}

PaintContext::PaintContext(Display * dpy, Window w, cairo_surface_t * canvas_surface, Rect bounds) {
	m_dpy		= dpy;
	m_xwindow	= w;
	m_bounds	= bounds;
	m_canvas_surface = canvas_surface;
	
	m_drawqueue = new smp::list<_shape *>();
	
	m_flipx = false;
	m_flipy = false;
	
	m_gc		= XCreateGC(m_dpy, w, 0, &m_gcvalues);
	m_xftdraw	= XftDrawCreate(m_dpy, m_xwindow, DefaultVisual(m_dpy, 0), DefaultColormap(m_dpy, 0));
}

PaintContext::~PaintContext() {
	if(m_drawqueue)
		delete m_drawqueue;
		
	XFreeGC(m_dpy, m_gc);
	XftDrawDestroy(m_xftdraw);
}

void PaintContext::FlipX() {
	m_flipx = true;
}

void PaintContext::FlipY() {
	m_flipy = true;
}

void PaintContext::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

void PaintContext::SetFont() {
	
}

void PaintContext::FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	_rect * r	 = create_rect(x, y, w, h, FILL, m_color);
	*m_drawqueue	+= (_shape *) r;
}

void PaintContext::DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	_rect * r	 = create_rect(x, y, w, h, DRAW, m_color);
	*m_drawqueue	+= (_shape *) r;
}

void PaintContext::FillCircle(int16_t x, int16_t y, int16_t rad) {
	_ellipse * e = create_ellipse(x, y, rad, rad, FILL, m_color);
	*m_drawqueue += (_shape *) e;
}

void PaintContext::DrawCircle(int16_t x, int16_t y, int16_t rad) {
	_ellipse * e = create_ellipse(x, y, rad, rad, DRAW, m_color);
	*m_drawqueue += (_shape *) e;
}

void PaintContext::FillEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry) {
	_ellipse * e = create_ellipse(x, y, rx, ry, FILL, m_color);
	*m_drawqueue += (_shape *) e;
}

void PaintContext::DrawEllipse(int16_t x, int16_t y, int16_t rx, int16_t ry) {
	_ellipse * e = create_ellipse(x, y, rx, ry, DRAW, m_color);
	*m_drawqueue += (_shape *) e;
}

void PaintContext::DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, cairo_surface_t * img) {
	_image * i = (_image *) malloc(sizeof(_image));
	i->type = IMAGE;
	i->x = x;
	i->y = y;
	i->w = w;
	i->h = h;
	i->c = m_color;
	i->img_surface = img;
	
	*m_drawqueue += (_shape *) i;
}

void PaintContext::DrawText(uint16_t x, uint16_t y, Text * text) {
	
	_text * t = (_text *) malloc(sizeof(_text));
	t->type = TEXT;
	t->x	= x;
	t->y	= y ;
	t->w	= text->w;
	t->h	= text->h;
	t->c	= m_color;
	t->text = text;
	
	*m_drawqueue += (_shape *) t;
}

void PaintContext::PaintScreen() {

	cairo_surface_t * surface = cairo_surface_create_for_rectangle(m_canvas_surface, m_bounds.x, m_bounds.y, m_bounds.w, m_bounds.h);
	cairo_t * cr = cairo_create(surface);
	
	/* Create Area on Surface to draw on */
	cairo_push_group(cr);
	
	/* Draw all Shapes excluding TEXT-Shapes, as they are drawn via Xft and thus must be drawn seperatly. */
	for(int i = 0; i < m_drawqueue->Size(); ++i) {

		_shape * shape = (*m_drawqueue)[i];

		if(m_flipx) {
			shape->x = m_bounds.w - shape->x - shape->w;
		}

		if(m_flipy) {
			shape->y = m_bounds.h - shape->y - shape->h;
		}

		cairo_set_source_rgba(cr, shape->c.r/255.0f, shape->c.g/255.0f, shape->c.b/255.0f, shape->c.a/255.0f);

		switch(shape->type) {
			case RECT: {

				_rect * r = (_rect *) shape;

				cairo_rectangle(cr, r->x, r->y, r->w, r->h);

				if(r->fillstyle == FILL)
					cairo_fill(cr);
				else
					cairo_stroke(cr);

				break;
			}

			case ELLIPSE: {

				_ellipse * e = (_ellipse *) shape;

				double fac = (double)e->rx/e->ry;

				cairo_save(cr);

				cairo_translate(cr, e->x, e->y);
				cairo_scale(cr, fac, 1);
				cairo_translate(cr, -e->x, -e->y);
				cairo_arc(cr, e->x, e->y, e->ry, 0.0, 2*3.14159);

				if(e->fillstyle == FILL)
					cairo_fill(cr);
				else
					cairo_stroke(cr);

				cairo_restore(cr);

				break;
			}
			
			case TEXT: {
				/* Skip the freeing and removing below as the Shape is needed afterwards to */
				/* then draw the Text over the Cairo-Surface. */
				continue;
			}
			
			case IMAGE: {
				
				_image * img = (_image *) shape;
				
				double resize_w = (double)shape->w/cairo_image_surface_get_width(img->img_surface);
				double resize_h = (double)shape->h/cairo_image_surface_get_height(img->img_surface);
				
				cairo_save(cr);
				
				cairo_scale(cr, resize_w, resize_h);
				
				cairo_set_source_surface(cr, img->img_surface, img->x, img->y);
				cairo_paint(cr);
				
				cairo_restore(cr);
				
				break;
			}
		}

		/* Free and remove the Shape from the List. */
		/* The iterator must be decreased as the List we are removing the shape from will otherwise skip */
		/* the other Shapes. */
		free(shape);
		*m_drawqueue -= shape;
		--i;
	}
	
	cairo_pop_group_to_source(cr);
	cairo_paint(cr);
	cairo_surface_flush(surface);
	
	for(int i = 0; i < m_drawqueue->Size(); ++i) {
		if((*m_drawqueue)[i]->type == TEXT) {
			_text * t = (_text *) (*m_drawqueue)[i];

				/* Prepare Color */
				const int	X_COLOR_DEPTH	= 65535;
				const float	RGB_COLOR_DEPTH	= 255.0f;

				XftColor		xftcolor;
				XRenderColor	rendercolor;
				rendercolor.alpha	= t->c.a/RGB_COLOR_DEPTH * X_COLOR_DEPTH;;
				rendercolor.red		= t->c.r/RGB_COLOR_DEPTH * X_COLOR_DEPTH;
				rendercolor.green	= t->c.g/RGB_COLOR_DEPTH * X_COLOR_DEPTH;;
				rendercolor.blue	= t->c.b/RGB_COLOR_DEPTH * X_COLOR_DEPTH;;

				/* Allocate Color */
				XftColorAllocValue(m_dpy, DefaultVisual(m_dpy, 0), DefaultColormap(m_dpy, 0), &rendercolor, &xftcolor);

				/* Draw Text to Screen */
				if(t && t->text && t->text->str && t->text->font && t->text->font->xftfont) {
					FcChar8 * fc8 = (FcChar8 *) (t->text->str);

					XftDrawStringUtf8(
						m_xftdraw, &xftcolor, t->text->font->xftfont, t->x + m_bounds.x, t->y + m_bounds.y + t->h, fc8, strlen(t->text->str)
					);
				}

				/* Free Color */
				XftColorFree(m_dpy, DefaultVisual(m_dpy, 0), DefaultColormap(m_dpy, 0), &xftcolor);
		}
	}

	m_drawqueue->Clear();
	XFlush(m_dpy);

	cairo_destroy(cr);
}