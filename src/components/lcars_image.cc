#include "lcars_image.hh"

#include <iostream>

LCARS::Image::Image(int x, int y, int w, int h, std::string imgfile) : Component((Rect) {x, y, w, h}) {
	m_imgfile		= imgfile;
	m_img_surface	= cairo_image_surface_create_from_png(imgfile.c_str());
	
	Reset();
}

LCARS::Image::~Image() {
	cairo_surface_destroy(m_img_surface);
}

void LCARS::Image::Resize(int w, int h) {
	m_resize_w = w;
	m_resize_h = h;
}

void LCARS::Image::Reset() {
	m_resize_w = cairo_image_surface_get_width(m_img_surface);
	m_resize_h = cairo_image_surface_get_height(m_img_surface);
}

bool LCARS::Image::PointInHitbox(int x, int y) {
	Rect abs = GetAbsoluteBounds();
	return x >= abs.x && x <= abs.x + m_bounds.w && y >= abs.y && y <= abs.y + m_bounds.h;
}

void LCARS::Image::Paint(PaintContext * paintctx) {
	paintctx->DrawImage(0, 0, m_resize_w, m_resize_h, m_img_surface);
}

void LCARS::Image::HandleSDLEvent(XEvent * ev) {

}

void LCARS::Image::OnUnhandledSDLEvent(XEvent * ev) {

}

void LCARS::Image::HandleCMPEvent(CMP_EVT_TYPE type) {
	
}

void * LCARS::Image::LoadImage(std::string image_path) {
	return nullptr;
}
